#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <resolv.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <arpa/inet.h>
 
#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/ssl.h>
#include "tcp_video.h"
 
#define MAXSIZE 1024 //每次最大数据传输量
 
#define PKEY_FILE "sslserverkey.pem"
#define CERT_FILE "sslservercert.pem"
 
int main()
{
    int sockfd,client_fd;
    socklen_t len;
 
    SSL_CTX *ctx;
  
    char serverbuf[MAXSIZE];
 
    ERR_load_BIO_strings();
    SSL_library_init();
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();
    ctx = SSL_CTX_new(SSLv23_server_method());
    if (ctx == NULL)
    {
        ERR_print_errors_fp(stdout);
        exit(1);
    }
    if (!SSL_CTX_use_certificate_file(ctx, CERT_FILE, SSL_FILETYPE_PEM))
    {
        ERR_print_errors_fp(stdout);
        exit(1);
    }
    if (!SSL_CTX_use_PrivateKey_file(ctx, PKEY_FILE, SSL_FILETYPE_PEM) )
    {
        ERR_print_errors_fp(stdout);
        exit(1);
    }
    if (!SSL_CTX_check_private_key(ctx))
    {
        ERR_print_errors_fp(stdout);
        exit(1);
    }
    int chose=0;
    signal(SIGPIPE,SIG_IGN);
    tcpserver_init(&sockfd);
    while(1)
    {
        printf("Please Chose Channel No.:\n");
        printf("1.:SSL Protocol Channel\n");
        printf("2.:TCP Protocol Channel\n");
        scanf("%d",&chose);
        if(chose==1)
        {
            SSL *ssl;
            tcp_accept(sockfd,&client_fd);
            ssl = SSL_new(ctx);
            SSL_set_fd(ssl, client_fd);
            if (SSL_accept(ssl) == -1)
            {
                perror("accept");
                close(client_fd);
                break;
            }
            // 接收消息
            bzero(serverbuf, MAXSIZE);
           
            len = SSL_read(ssl,serverbuf, MAXSIZE);
            if (len > 0)
                printf("接收消息成功:'%s'，共%d个字节的数据\n",serverbuf, len);
            else
                printf("消息接收失败！错误代码是%d，错误信息是'%s'\n",errno, strerror(errno));
           
            SSL_shutdown(ssl);
            SSL_free(ssl);
            close(client_fd);
        }
        else if(chose==2)
        {
            tcp_accept(sockfd,&client_fd);
            len=recv(client_fd,serverbuf, MAXSIZE,0);
            if (len > 0)
                printf("接收消息成功:'%s'，共%d个字节的数据\n",serverbuf, len);
            else
                printf("消息接收失败！错误代码是%d，错误信息是'%s'\n",errno, strerror(errno));
            close(client_fd);
        }
        chose=0;
    }
    close(sockfd);
    SSL_CTX_free(ctx);
    return 0;
 
}//main