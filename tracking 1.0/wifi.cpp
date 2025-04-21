#include <iostream>
#include <stdexcept>
#include <cstring>
#include <unistd.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <wiringPi.h>
#include <sys/time.h>

const int BUFSIZE = 512;

// 电机控制类
class MotorController {
public:
    MotorController() {
        wiringPiSetup();
        pinMode(1, OUTPUT); // IN1
        pinMode(4, OUTPUT); // IN2
        pinMode(5, OUTPUT); // IN3
        pinMode(6, OUTPUT); // IN4


        // 初始化输出
        digitalWrite(1, HIGH);
        digitalWrite(4, HIGH);
        digitalWrite(5, HIGH);
        digitalWrite(6, HIGH);
    }

    void goForward() {
        digitalWrite(1, HIGH);
        digitalWrite(4, LOW);
        digitalWrite(5, HIGH);
        digitalWrite(6, LOW);
        std::cout << "forward" << std::endl;
    }

    void goBack() {
        digitalWrite(4, HIGH);
        digitalWrite(1, LOW);
        digitalWrite(6, HIGH);
        digitalWrite(5, LOW);
        std::cout << "back" << std::endl;
    }

    void goLeft() {
        digitalWrite(4, HIGH);
        digitalWrite(1, LOW);
        digitalWrite(5, HIGH);
        digitalWrite(6, LOW);
        std::cout << "left" << std::endl;
    }

    void goRight() {
        digitalWrite(1, HIGH);
        digitalWrite(4, LOW);
        digitalWrite(6, HIGH);
        digitalWrite(5, LOW);
        std::cout << "right" << std::endl;
    }

    void stop() {
        digitalWrite(1, LOW);
        digitalWrite(4, LOW);
        digitalWrite(5, LOW);
        digitalWrite(6, LOW);
        digitalWrite(3, LOW);
        std::cout << "stop" << std::endl;
    }
};

// 客户端类
class Client {
public:
    int fd;
    struct sockaddr_in addr;

    Client() : fd(-1) {}
};

// 服务器类
class Server {
private:
    int listenfd;
    int portnumber;
    MotorController motorController;
    Client clients[FD_SETSIZE];
    fd_set rset, allset;
    int maxfd;
    int maxi;

    void setupSocket() {
        listenfd = socket(PF_INET, SOCK_STREAM, 0);
        if (listenfd == -1) {
            throw std::runtime_error("Socket Error!");
        }

        struct sockaddr_in server_addr;
        std::memset(&server_addr, 0, sizeof(server_addr));
        server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
        server_addr.sin_port = htons(portnumber);

        if (bind(listenfd, reinterpret_cast<struct sockaddr*>(&server_addr), sizeof(server_addr)) == -1) {
            throw std::runtime_error("Bind Error!");
        }

        if (listen(listenfd, 128) == -1) {
            throw std::runtime_error("Listen Error!");
        }
    }

    void handleNewConnection() {
        struct sockaddr_in client_addr;
        socklen_t len = sizeof(client_addr);
        int connectfd = accept(listenfd, reinterpret_cast<struct sockaddr*>(&client_addr), &len);
        if (connectfd == -1) {
            std::cerr << "accept() error" << std::endl;
            return;
        }

        for (int i = 0; i < FD_SETSIZE; ++i) {
            if (clients[i].fd < 0) {
                clients[i].fd = connectfd;
                clients[i].addr = client_addr;
                std::cout << "You got a connection from " << inet_ntoa(clients[i].addr.sin_addr) << std::endl;
                break;
            }
        }

        if (connectfd > maxfd) {
            maxfd = connectfd;
        }

        if (connectfd > maxi) {
            maxi = connectfd;
        }

        FD_SET(connectfd, &allset);
    }

    void handleClientData(int i) {
        char buf[BUFSIZE] = {0xff, 0x00, 0x00, 0x00, 0xff};
        ssize_t z = read(clients[i].fd, buf, sizeof(buf));
        if (z > 0) {
            buf[z] = '\0';
            if (z == 5) {
                if (buf[1] == 0x00) {
                    switch (buf[2]) {
                        case 0x01:
                            motorController.goForward();
                            break;
                        case 0x02:
                            motorController.goBack();
                            break;
                        case 0x03:
                            motorController.goLeft();
                            break;
                        case 0x04:
                            motorController.goRight();
                            break;
                        case 0x00:
                            motorController.stop();
                            break;
                        default:
                            break;
                    }
                } else {
                    motorController.stop();
                }
            } else if (z == 6) {
                if (buf[2] == 0x00) {
                    switch (buf[3]) {
                        case 0x01:
                            motorController.goForward();
                            break;
                        case 0x02:
                            motorController.goBack();
                            break;
                        case 0x03:
                            motorController.goLeft();
                            break;
                        case 0x04:
                            motorController.goRight();
                            break;
                        case 0x00:
                            motorController.stop();
                            break;
                        default:
                            break;
                    }
                } else {
                    motorController.stop();
                }
            } else {
                motorController.stop();
            }
        } else {
            std::cout << "disconnected by client!" << std::endl;
            close(clients[i].fd);
            FD_CLR(clients[i].fd, &allset);
            clients[i].fd = -1;
        }
    }

public:
    Server(int port) : portnumber(port), maxfd(-1), maxi(-1) {
        setupSocket();

        for (int i = 0; i < FD_SETSIZE; ++i) {
            clients[i].fd = -1;
        }

        FD_ZERO(&allset);
        FD_SET(listenfd, &allset);
        maxfd = listenfd;

        std::cout << "waiting for the client's request..." << std::endl;
    }

    ~Server() {
        close(listenfd);
    }

    void run() {
        while (true) {
            rset = allset;
            struct timeval tv;
            tv.tv_sec = 0;
            tv.tv_usec = 1;

            int ret = select(maxfd + 1, &rset, nullptr, nullptr, &tv);
            if (ret == 0) {
                continue;
            } else if (ret < 0) {
                throw std::runtime_error("select failed!");
            }

            if (FD_ISSET(listenfd, &rset)) {
                handleNewConnection();
            } else {
                for (int i = 0; i <= maxi; ++i) {
                    if (clients[i].fd < 0) {
                        continue;
                    }

                    if (FD_ISSET(clients[i].fd, &rset)) {
                        handleClientData(i);
                    }
                }
            }
        }
    }
};

int main(int argc, char *argv[]) {
    try {
        if (argc != 2) {
            throw std::invalid_argument("Please add portnumber!");
        }

        int portnumber = std::atoi(argv[1]);
        if (portnumber < 0) {
            throw std::invalid_argument("Enter Error!");
        }

        Server server(portnumber);
        server.run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}
