#include<sys/socket.h>
#include<sys/types.h>
#include<arpa/inet.h>//定义ip地址转换的函数
#include<fcntl.h>
#include<errno.h>
#include<unistd.h> //基本库 定义通用的文件目录等操作
#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/epoll.h>
#include<thread>
#include<pthread.h>
#define MAX_FD_SIZE 1024
int create_listenfd(){
    //建立监听套接字的步骤 sbla
    int listenfd = socket(AF_INET,SOCK_STREAM, 0);
    //设置套接字选项
    //socket关闭之后并不会立即收回，而是要经历一个TIME_WAIT的阶段。
    //此时对这个端口进行重新绑定就会出错。要想立即绑定端口，需要先设置 SO_REUSEADDR.
    /*　sockfd：标识一个套接口的描述字。
　　 level：选项定义的层次；支持SOL_SOCKET、IPPROTO_TCP、IPPROTO_IP和IPPROTO_IPV6。
　　 optname：需设置的选项。
　　 optval：指针，指向存放选项值的缓冲区。
　　 optlen：optval缓冲区长度。
 　　 返回值：  成功返回0，失败返回 -1.  */
    int n=1;
    setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&n,sizeof(n));
    struct sockaddr_in addr;
    bzero(&addr,sizeof(addr));//先情空
    addr.sin_family=AF_INET;  
    addr.sin_port=htons(8080);
    addr.sin_addr.s_addr=INADDR_ANY;

    if(bind(listenfd,(struct sockaddr *)&addr,sizeof(sockaddr)) == -1){
        perror("bind()");
        exit(-1);
    }

    listen(listenfd,1024);
    std::cout<<"creatfd success!"<<std::endl;
    return listenfd;
}
void handle_con(int connfd){
    //处理请求
    std::cout<<"handle!"<<std::endl;
    char request[1024]={0};
    std::string msg;//test:接收大文件用这个，以免空间不够
    int offset =0;
    int nread=0;
    while(1){
        //char request[1024]={0};
        nread=recv(connfd,request+offset,100,0);
        //string +=request
        std::cout<<"reading offset:"<<offset<<std::endl;
        std::cout<<"reading content:"<<request<<std::endl;
        offset+=nread;
        if(nread <100){break;}
    }
   //int nread=read(connfd,request,sizeof(request));
    std::cout<<"request:"<<request<<std::endl;
    //解析文件
    //sscanf()字符串解析函数
    char filename[10]={};
    sscanf(request,"GET /%s",filename);
    
    std::cout<<"filename:"<<filename<<std::endl;

    //获得文件
    char *mime = NULL;
    if(strstr(filename,"html")){
        mime = "text/html";
    }
    else if(strstr(filename,"jpg")){
        mime = "imag/jpg";//这些是规定
    }
    else{
        std::cout<<"notfound"<<std::endl;
    }
    //打开文件 构建响应
    char response[1024*1024]={};
    sprintf(response,"HTTP/1.0 200 OK\r\nContent-Type: %s\r\n\r\n",mime);//状态行+响应头
    int headlen = strlen(response);
    int filefd = open(filename,O_RDONLY);
    int filelen=read(filefd,response+headlen,sizeof(response)-headlen);//响应内容

    //返回
    std::cout<<"send to :"<<connfd<<std::endl;
    send(connfd,response,headlen+filelen,0);
    std::cout<<"close!"<<std::endl;
    close(connfd);
    
}
void thread_accept(int sockfd,int epoll_accept_fd,int epoll_listen_fd){
    std::cout<<"listenfd"<<sockfd<<std::endl;
    struct epoll_event ev,events[MAX_FD_SIZE];
    ev.data.fd=sockfd;
    ev.events=EPOLLIN;
    epoll_ctl(epoll_accept_fd,EPOLL_CTL_ADD,sockfd,&ev);
    while(true){
        int ret = epoll_wait(epoll_accept_fd,events,MAX_FD_SIZE,-1);
        if(ret){
            for(int i=0;i<ret;i++){
                int eventfd = events[i].data.fd;
                //聊天室连接
                if(eventfd == sockfd && events[i].events == EPOLLIN){
                    //new connection
                    struct sockaddr_in client;
                    socklen_t client_len = 1;
                    int clientfd ;
                    if((clientfd = accept(sockfd,(struct sockaddr *) &client,&client_len)) == -1){
                         perror("accept()");
                         exit(1);
                    }
                    struct epoll_event ev2;
                    ev2.data.fd = clientfd;
                    ev2.events = EPOLLIN;
                    std::cout<<"new client connected! fd_num:"<<clientfd<<std::endl;
                    epoll_ctl(epoll_listen_fd,EPOLL_CTL_ADD,clientfd, &ev2);//加入连接epoll
                }
            }
        }
    }
}

void thread_handle(int epoll_listen_fd){
    struct epoll_event ev,events[MAX_FD_SIZE];
     while(true){
        int ret = epoll_wait(epoll_listen_fd,events,MAX_FD_SIZE,-1);
         for(int i=0;i<ret;i++){
                int clientfd = events[i].data.fd;
                std::cout<<"clientfd"<<clientfd<<std::endl;
                if(events[i].events == EPOLLIN){
                    handle_con(clientfd);
                    struct epoll_event ev2;
                    ev2.data.fd = clientfd;
                    ev2.events = EPOLLIN;
                    epoll_ctl(epoll_listen_fd,EPOLL_CTL_DEL,clientfd,&ev2);
                }
         }
     }
}
int main(){
    //创建两个epoll 一个负责接受连接一个负责发送
    int epoll_accept_fd = epoll_create(1);
    int epoll_listen_fd = epoll_create(MAX_FD_SIZE);
    int sockfd = create_listenfd();
    std::thread t_accept(thread_accept,sockfd,epoll_accept_fd,epoll_listen_fd);
    std::thread t_handle(thread_handle,epoll_listen_fd);
    t_accept.join();
    t_handle.join();
    
    close(sockfd);
    return 0;

}