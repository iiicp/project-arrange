/**********************************
* File:     main_server.cpp.c
*
* Author:   caipeng
*
* Email:    iiicp@outlook.com
*
* Date:     2021/1/5
***********************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define BUF_SIZE 80

void error_handling(const char *msg) {
    printf("%s\n", msg);
    abort();
}

/**
 *  domain: 协议族信息, PF_INET, PF_INET6
 *  type: 套接字类型，同一个协议族，会有多种套接字传输方式，面向连接的套接字和面向消息的套接字
 *      面向连接的套接字: SOCK_STREAM 特点：数据不会丢失、按序传递、数据不存在边界
 *      面向消息的套接字: SOCK_DGRAM 特点：强调快，不是顺序，数据存在边界
 *
 *  // 成功时返回文件描述符，失败时返回-1
 *  int socket(int domain, int type, int protocol);
 *
 *  // 成功时返回0，失败时返回-1
 *  int bind(int sockfd, struct sockaddr *myaddr, socklen_t addrlen);
 *
 *  // 成功时返回0，失败时返回-1
 *  int listen(int sockfd, int backlog);
 *
 *  // 成功时返回文件描述符，失败时返回-1
 *  int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
 */

int main(int argc, char *argv[]) {

    if (argc != 2) {
        printf("use: server <port>\n");
        return 0;
    }

    /// 1. 创建socket套接字
    int server_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (server_sock == -1) {
        error_handling("socket() error");
    }

    /// 2. 绑定ip地址和端口
    ///    对于服务器来说，一般是绑定任意的ip地址, 绑定的端口需要开发出去（云服务器的话需要配置安全组）
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(atoi(argv[1]));

    if (bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        error_handling("bind() error");
    }

    /// 3. listen请求
    if (listen(server_sock, 5) == -1) {
        error_handling("listen() error");
    }

    /// 4. accept 接收客户端请求 (此处会阻塞等待客户端请求)
    struct sockaddr_in client_addr;
    memset(&client_addr, 0, sizeof(client_addr));
    socklen_t client_size = sizeof(client_addr);

    char message[100];

    int client_sock, strLen = 0;
    for (int i = 0; i < 5; ++i) {

        client_sock = accept(server_sock, (struct sockaddr *) &client_addr, &client_size);
        if (client_sock == -1) {
            error_handling("accept() error");
        }else {
            printf("Connected client %d\n", i+1);
        }

        /// 5. write && read
        while ((strLen = read(client_sock, message, BUF_SIZE)) != 0)
            write(client_sock, message, strLen);

        close(client_sock);
    }

    /// 6. close
    close(server_sock);

    return 0;
}