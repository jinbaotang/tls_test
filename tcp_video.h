void tcpserver_init(int *sockfd)
{
    socklen_t len;
    struct sockaddr_in my_addr;
    unsigned int myport, lisnum;
 
    myport = 7838;
    lisnum = 1;
 
    if ((*sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("socket");
        exit(1);
    }
    else
        printf("socket created\n");
 
    bzero(&my_addr, sizeof(my_addr));
    my_addr.sin_family = PF_INET;
    my_addr.sin_port = htons(myport);
    my_addr.sin_addr.s_addr = INADDR_ANY;
 
    if(bind(*sockfd, (struct sockaddr *) &my_addr, sizeof(struct sockaddr)) == -1)
    {
        perror("bind");
        exit(1);
    }
    else
        printf("binded\n");
 
    if (listen(*sockfd, lisnum) == -1)
    {
        perror("listen");
        exit(1);
    }
    else
        printf("begin listen\n");
}
void tcp_accept(int sockfd,int *new_fd)
{
    struct sockaddr_in their_addr;
    socklen_t len;
    len = sizeof(struct sockaddr);
    if ((*new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &len)) == -1)
    {
        perror("accept");
        exit(errno);
    }
    else
        printf("server: got connection from %s, port %d, socket %d\n",inet_ntoa(their_addr.sin_addr), ntohs(their_addr.sin_port), *new_fd);
    int flags;
    flags = fcntl (sockfd, F_GETFL); 
    if (flags & O_NONBLOCK)
    {
        //fcntl (sockfd, F_SETFL, flags-O_NONBLOCK);
        fcntl(sockfd,F_SETFL,flags&(~O_NONBLOCK));
    }
}
struct sockaddr_in tcpclient_init(int *sockfd)
{
    int len;
    struct sockaddr_in dest;
 
    char parainfo[3][20];
    printf("input server IP:\n");
    scanf("%s",parainfo[0]);
    printf("input server port:\n");
    scanf("%s",parainfo[1]);
    if((*sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Socket");
        exit(errno);
    }
    printf("socket created\n");
 
    bzero(&dest, sizeof(dest));
    dest.sin_family = AF_INET;
    dest.sin_port = htons(atoi(parainfo[1]));
    if (inet_aton(parainfo[0], (struct in_addr *) &dest.sin_addr.s_addr) == 0)
    {
        perror(parainfo[0]);
        exit(errno);
    }
    printf("address created\n");
    return dest;
}
void tcp_connect(int *sockfd,struct sockaddr_in dest)
{
    if (connect(*sockfd, (struct sockaddr *) &dest, sizeof(dest)) != 0)
    {
        perror("Connect ");
        exit(errno);
    }
    printf("server connected\n");
    int flags;
    flags = fcntl (sockfd, F_GETFL); 
    if (flags & O_NONBLOCK)
    {
        //fcntl (sockfd, F_SETFL, flags-O_NONBLOCK);
        fcntl(sockfd,F_SETFL,flags&(~O_NONBLOCK));
    }
}