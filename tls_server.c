#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <openssl/err.h>
#include <openssl/ssl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAXBUF 1024

int main(int argc, char **argv) {
    int sockfd, new_fd;
    socklen_t len;
    struct sockaddr_in server_addr, client_addr;
    char buf[MAXBUF + 1];
    SSL_CTX *ctx = NULL;

    SSL_library_init();
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();
    ctx = SSL_CTX_new(SSLv23_server_method());
    if (ctx == NULL) {
        ERR_print_errors_fp(stdout);
        exit(1);
    }
    printf("certificate file: '%s'.\n", argv[1]);
    if (SSL_CTX_use_certificate_file(ctx, argv[1], SSL_FILETYPE_PEM) <= 0) {
        ERR_print_errors_fp(stdout);
        exit(1);
    }
    printf("privatekey file: '%s'.\n", argv[1]);
    if (SSL_CTX_use_PrivateKey_file(ctx, argv[2], SSL_FILETYPE_PEM) <= 0) {
        ERR_print_errors_fp(stdout);
        exit(1);
    }
    if (!SSL_CTX_check_private_key(ctx)) {
        ERR_print_errors_fp(stdout);
        exit(1);
    }
    if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    } else
        printf("socket created\n");

    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = PF_INET;
    server_addr.sin_port = htons(7838);
    if (argv[3])
        server_addr.sin_addr.s_addr = inet_addr(argv[3]);
    else
        server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1) {
        perror("bind");
        exit(1);
    } else
        printf("binded\n");

    if (listen(sockfd, 3) == -1) {
        perror("listen");
        exit(1);
    } else
        printf("begin listen\n");

    while (1) {
        SSL *ssl;
        len = sizeof(struct sockaddr);

        if ((new_fd = accept(sockfd, (struct sockaddr *)&client_addr, &len)) == -1) {
            perror("accept");
            exit(errno);
        } else
            printf("server: got connection from '%s', port %d, socket %d\n", inet_ntoa(client_addr.sin_addr),
                   ntohs(client_addr.sin_port), new_fd);

        ssl = SSL_new(ctx);
        SSL_set_fd(ssl, new_fd);
        if (SSL_accept(ssl) == -1) {
            perror("accept");
            close(new_fd);
            break;
        }

        bzero(buf, MAXBUF + 1);
        strcpy(buf, "server->client");
        len = SSL_write(ssl, buf, strlen(buf));

        if (len <= 0) {
            printf("send '%s' failed, errno: %d, error infomation: '%s' \n", buf, errno, strerror(errno));
            goto finish;
        } else{
            printf("send %d bytes '%s' successfuly.\n", len, buf);
        }

        bzero(buf, MAXBUF + 1);
        len = SSL_read(ssl, buf, MAXBUF);
        if (len > 0)
            printf("recv %d bytes with '%s'.", len, buf);
        else
            printf("recvive error. errno: %d, error infomation:'%s'.\n", errno, strerror(errno));

    finish:
        SSL_shutdown(ssl);
        SSL_free(ssl);
        close(new_fd);
    }
    close(sockfd);
    SSL_CTX_free(ctx);
    return 0;
}