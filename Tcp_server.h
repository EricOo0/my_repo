#ifndef __TCP_SERVER__
#define __TCP_SERVER__


#include<iostream>
#include<stdio.h>
#include<string.h>
#include<unistd.h> //基本库 定义通用的文件目录等操作
#include<errno.h>//错误定义
#include<pthread.h>//线程

#include<sys/types.h>//数据类型的定义
#include<sys/socket.h>//socket函数及数据结构
#include<netinet/in.h>//定义数据结构sockadd_in
#include<arpa/inet.h>//定义ip地址转换的函数
#include<sys/poll.h>
#include<sys/epoll.h>

#include<set>
#include<string>
#include<thread>
#include<mutex>//锁

#define MAX_FD_SIZE 1024
#define BUFFER_SIZE 1024
#define DEFAULTIP INADDR_ANY+1
class Tcp_server{
    private:
        int serverfd;
        int httpfd;
        in_addr_t ip_addr;
        int serverport;
        int httpport;
        //创建三个epoll reactor模型
        int epoll_accept_fd ;
        int epoll_listen_fd ;
        int epoll_http_fd ;
    public:
        std::set<int> clientset;
        std::mutex mtx;
        Tcp_server():
            ip_addr(DEFAULTIP),
            httpport(8080),
            serverport(10000)
            {
                //创建两个epoll reactor模型
                epoll_accept_fd = epoll_create(2);
                epoll_listen_fd = epoll_create(MAX_FD_SIZE);
                epoll_http_fd = epoll_create(MAX_FD_SIZE);
            };
        Tcp_server(const in_addr_t &ip,const int &server_port,const int &http_port):
            ip_addr(ip),
            httpport(http_port),
            serverport(server_port)
            {
                //创建两个epoll reactor模型
                epoll_accept_fd = epoll_create(2);
                epoll_listen_fd = epoll_create(MAX_FD_SIZE);
                epoll_http_fd = epoll_create(MAX_FD_SIZE);
                if((serverfd = socket(AF_INET,SOCK_STREAM,0)) == -1){
                    perror("socket()");//打印错误
                    exit(1);
                }
                if((httpfd = socket(AF_INET,SOCK_STREAM,0)) == -1){
                    perror("socket()");//打印错误
                    exit(1);
                }
            };
        void bind_socket(){
            //socket init
            struct sockaddr_in server_socket,http_socket;
            server_socket.sin_family=AF_INET;
            server_socket.sin_port=htons(serverport);
            server_socket.sin_addr.s_addr = ip_addr;//inet_addr(MYIP);将一个点分十进制的IP转换成一个长整数型数
            memset(server_socket.sin_zero, 0, sizeof(server_socket.sin_zero));

            http_socket.sin_family=AF_INET;
            http_socket.sin_port=htons(httpport);
            http_socket.sin_addr.s_addr = ip_addr;//inet_addr(MYIP);将一个点分十进制的IP转换成一个长整数型数
            memset(http_socket.sin_zero, 0, sizeof(http_socket.sin_zero));

            //bind socket
            if(bind(serverfd,(struct sockaddr*) &server_socket,sizeof(sockaddr)) == -1)
            {
                perror("bind()");
                exit(1);        
            }
            std::cout<<"server_socket creat:"<<serverfd<< ";bind ok"<<std::endl;

            if(bind(httpfd,(struct sockaddr*) &http_socket,sizeof(sockaddr)) == -1)
            {
                perror("bind()");
                exit(1);        
            }
            std::cout<<"http_socket creat:"<<httpfd<< ";bind ok"<<std::endl;
        }
        void listen_sock(int& len){
            //len为可进入队列的连接请求最大数量,不会阻塞
            if(listen(serverfd,len) == -1){
                perror("listen()");
            } 
            std::cout<<"server listennig......"<<std::endl;


            if(listen(httpfd,len) == -1){
                perror("listen()");
            } 
            std::cout<<"http listennig......"<<std::endl;
        }
       
        #if 0
        void accept_sock(){
            struct sockaddr_in client;
            socklen_t client_len = 1;
            int clientfd ;
            if((clientfd = accept(socketfd,(struct sockaddr *) &client,&client_len)) == -1){
                perror("accept()");
                exit(1);
            }
            mtx.lock();
            clientset.insert(clientfd);//保存连接客户端，用于后面发送消息
            mtx.unlock();
            char welcome[BUFFER_SIZE] = {};
            sprintf(welcome,"client %d enter,welcome!\n",clientfd);
        }
        #endif

        void delete_client(int clientfd){
            struct epoll_event ev;
            close(clientfd);
            printf("client fd %d leave!\n",clientfd);
            printf("close fd %d\n",clientfd);
            ev.data.fd=clientfd;
            ev.events=EPOLLIN;
            epoll_ctl(epoll_listen_fd,EPOLL_CTL_DEL,clientfd,&ev);
            char leave[BUFFER_SIZE];
            sprintf(leave,"client%d leave!",clientfd);
            send2allclient(leave,clientfd);
        }
        void send2allclient(char *buffer,int &selffd){
            for(auto client:clientset){
                if(client !=selffd){
                std::cout<<"send to client:"<<client<<std::endl;
                int len;
                char buffer_new[BUFFER_SIZE]={};
                sprintf(buffer_new,"client%d: ",selffd);
                strcat(buffer_new,buffer);
                if(( len = send(client,buffer_new,strlen(buffer_new),0)) ==-1){
                    perror("send()");
                    mtx.lock();
                    clientset.erase(client);
                    delete_client(client);
                    mtx.unlock();
                }
                }
            }
        }
        
        void send_msg(){}
        void send_json_msg(std::string &dir){}
        void recv_msg(){}
        void http_recv(){

        }
        const int get_serverfd(){
            return serverfd;
        }
        const int get_httpfd(){
            return httpfd;
        }
        const in_addr_t get_ip(){
            return ip_addr;
        }
        const int get_serverport(){
            return serverport;
        }
        const int get_httpport(){
            return httpport;
        }
        const int get_accpetfd(){
            return epoll_accept_fd;
        }
        const int get_listenfd(){
            return epoll_listen_fd;
        }
        const int get_http_epoll_fd(){
            return epoll_http_fd;
        }
};

#endif