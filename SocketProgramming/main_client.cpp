/**********************************
* File:     main_client.cpp
*
* Author:   caipeng
*
* Email:    iiicp@outlook.com
*
* Date:     2021/1/5
***********************************/

/**
 *  // 成功时返回socket描述符，失败时返回-1
 *  int socket(int domain, int type, int protocol);
 *
 *  // 成功时返回0， 失败时返回-1
 *  int connect(int sockfd, struct sockaddr *serv_addr, socklen_t addrlen);
 *
 *
 */
#include <iostream>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#define BUF_SIZE 100

using namespace std;

void error_handling(const char *msg) {
    printf("%s\n", msg);
    abort();
}

int main(int argc, char *argv[]) {

    if (argc != 3) {
        printf("use: client ip port!!!\n");
        return 0;
    }

    /// 1. 创建客户端套接字
    int sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        error_handling("socket() error");
    }

    /// 2. 连接服务器（时机是服务器调用listen之后）
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]); /// 客户端传输的IP地址
    serv_addr.sin_port = htons(atoi(argv[2])); /// 客户端传输的端口
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1) {
        error_handling("connect() error");
    }

    char message[100];
    ssize_t str_len, recv_len, recv_cnt;

    while (true) {
        fputs("Input message(Q to quit): ", stdout);
        fgets(message, BUF_SIZE, stdin);

        if (!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
            break;

        str_len = write(sock, message, strlen(message));
        recv_len = 0;
        while (recv_len < str_len) {
            recv_cnt = read(sock, &message[recv_len], BUF_SIZE - 1);
            if (recv_cnt == -1)
                error_handling("read() error");
            recv_len += recv_cnt;
        }
        message[recv_len] = 0;
        printf("message from server: %s\n", message);
    }
    close(sock);
    return 0;
}