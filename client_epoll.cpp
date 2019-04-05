#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define IPADDERESS "127.0.0.1"
#define PORT 6666
#define MAXSIZE 1024
#define FDSIZE 1024
#define EPOLLEVENTS 20

// 函数声明
// IO 多路复用epoll
void handle_connection(int sockfd);
// 事件处理函数
void handle_events(int epollfd, struct epoll_event *events, int num, int sockfd, char *buf);
// 读处理
void do_read(int epollfd, int fd, int sockfd, char *buf);
// 写处理
void do_write(int epollfd, int fd, int sockfd, char *buf);
// 添加事件
void add_event(int epollfd, int fd, int state);
// 修改事件
void modfiy_event(int epollfd, int fd, int state);
// 删除事件
void delete_event(int epollfd, int fd, int state);
int count = 0;
int main(int argc, char *argv[])
{
    int sockfd;
    struct sockaddr_in servaddr;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(IPADDERESS);
    servaddr.sin_port = htons(PORT);
    connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
    //处理连接
    handle_connection(sockfd);
    close(sockfd);
    return 0;
}

void handle_connection(int sockfd)
{
    int epollfd;
    struct epoll_event events[FDSIZE];
    char buf[MAXSIZE];
    int ret;
    epollfd = epoll_create(FDSIZE);
    add_event(epollfd, STDIN_FILENO, EPOLLIN);
    while (1)
    {
        ret = epoll_wait(epollfd, events, EPOLLEVENTS, -1);
        handle_events(epollfd, events, ret, sockfd, buf);
    }
    close(epollfd);
}

void handle_events(int epollfd, struct epoll_event *events, int num, int sockfd, char *buf)
{
    int i, fd;
    //进行选好遍历
    for (i = 0; i < num; ++i)
    {
        fd = events[i].data.fd;
        //根据描述符的类型和事件类型进行处理
        if (events[i].events & EPOLLIN)
        {
            do_read(epollfd, fd, sockfd, buf);
        }
        else if (events[i].events & EPOLLOUT)
        {
            do_write(epollfd, fd, sockfd, buf);
        }
    }
}

void do_read(int epollfd, int fd, int sockfd, char *buf)
{
    int nread;
    nread = read(fd, buf, MAXSIZE);
    if (nread == -1)
    {
        perror("read error:");
        close(fd);
    }
    else if (nread == 0)
    {
        fprintf(stderr, "server close.\n");
        close(fd);
    }
    else
    {
        if (fd == STDIN_FILENO)
        {
            add_event(epollfd, sockfd, EPOLLOUT);
        }
        else
        {
            delete_event(epollfd, sockfd, EPOLLIN);
            add_event(epollfd, STDOUT_FILENO, EPOLLOUT);
        }
    }
}

void do_write(int epollfd, int fd, int sockfd, char *buf)
{
    int nwrite;
    char temp[100];
    buf[strlen(buf) - 1] = '\0';
    snprintf(temp, sizeof(temp), "%s_%02d\n", buf, count++);
    nwrite = write(fd, temp, strlen(temp));
    if (nwrite == -1)
    {
        perror("write error:");
        close(fd);
    }
    else
    {
        if (fd == STDOUT_FILENO)
        {
            delete_event(epollfd, fd, EPOLLOUT);
        }
        else
        {
            modfiy_event(epollfd, fd, EPOLLIN);
        }
    }
    memset(buf, 0, MAXSIZE);
}

void add_event(int epollfd, int fd, int state)
{
    struct epoll_event ev;
    ev.events = state;
    ev.data.fd = fd;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev);
}

void modfiy_event(int epollfd, int fd, int state)
{
    struct epoll_event ev;
    ev.events = state;
    ev.data.fd = fd;
    epoll_ctl(epollfd, EPOLL_CTL_MOD, fd, &ev);
}

void delete_event(int epollfd, int fd, int state)
{
    struct epoll_event ev;
    ev.events = state;
    ev.data.fd = fd;
    epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, &ev);
}