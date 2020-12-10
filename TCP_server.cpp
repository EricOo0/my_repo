
#include"Tcp_server.h"
#include<pthread.h>//线程
#include<sys/stat.h>
#include<fcntl.h> 

#define SERVER_PORT 10000
#define HTTP_PORT 8080
#define MYIP INADDR_ANY
#define MAX_LISTEN 1024
int filefd = open("./index.html",O_RDONLY);//只读打开文件
char test[2048]={} ;
int num=read(filefd,test,2048);
std::string firstline = "HTTP/1.0 200 OK\r\n";
std::string header = "Server: HHTWS Web Server\r\nConnection: close\r\nContent-Type:\r\ncharset: gbk\r\nContent-Length:"
            +std::to_string(strlen(test))+"\r\n\r\n";


void thread_accpet(Tcp_server &server){
    struct epoll_event ev1,ev2,ev;
    struct epoll_event events[MAX_FD_SIZE];
    int serverfd = server.get_serverfd();
    int httpfd = server.get_httpfd();

    int epollfd=server.get_accpetfd();
    int epoll_listen_fd=server.get_listenfd();
    int epoll_http_fd = server.get_http_epoll_fd();
    //监听聊天室和HTTP的连接请求
    ev1.data.fd=serverfd;
    ev1.events=EPOLLIN;
    epoll_ctl(epollfd,EPOLL_CTL_ADD,serverfd, &ev1); 
    ev2.data.fd=httpfd;
    ev2.events=EPOLLIN;
    epoll_ctl(epollfd,EPOLL_CTL_ADD,httpfd, &ev2); 
    while(true){
        int ret = epoll_wait(epollfd,events,MAX_FD_SIZE,-1);
        if(ret){
            for(int i=0;i<ret;i++){
                int eventfd = events[i].data.fd;
                //聊天室连接
                if(eventfd == serverfd && events[i].events == EPOLLIN){
                    //new connection
                    struct sockaddr_in client;
                    socklen_t client_len = 1;
                    int clientfd ;
                    if((clientfd = accept(serverfd,(struct sockaddr *) &client,&client_len)) == -1){
                         perror("accept()");
                         exit(1);
                    }
                    server.mtx.lock();
                    server.clientset.insert(clientfd);//保存连接客户端，用于后面发送消息
                    ev.data.fd=clientfd;
                    ev.events=EPOLLIN;
                    epoll_ctl(epoll_listen_fd,EPOLL_CTL_ADD,clientfd, &ev);//加入连接epoll
                    server.mtx.unlock();    
                    char welcome[BUFFER_SIZE];
                    sprintf(welcome,"client%d enter,welcome!",clientfd);
                    server.send2allclient(welcome,clientfd);
                    std::cout<<"client"<<clientfd<<"connected"<<std::endl;
                    memset(welcome,0,BUFFER_SIZE);
                }
                //http连接请求
                else if(eventfd == httpfd && events[i].events == EPOLLIN)
                {
                    /**/
                    //new connection
                    struct sockaddr_in httpclient;
                    socklen_t client_len = 1;
                    int httpclientfd ;
                    if((httpclientfd = accept(httpfd,(struct sockaddr *) &httpclient,&client_len)) == -1){
                         perror("accept()");
                         exit(1);
                    }
                    server.mtx.lock();
                    ev.data.fd=httpclientfd;
                    ev.events=EPOLLOUT;
                    epoll_ctl(epoll_http_fd,EPOLL_CTL_ADD,httpclientfd, &ev);//加入连接epoll
                    server.mtx.unlock();    
                    
                }
                
            }
        }
    }

}

void thread_listen(Tcp_server &server){
    
    struct epoll_event events[MAX_FD_SIZE];
    //int epollfd=server.get_accpetfd();
    int epoll_listen_fd=server.get_listenfd();
    while(true){
        int ret = epoll_wait(epoll_listen_fd,events,MAX_FD_SIZE,-1);
        if(ret){
            char buffer[BUFFER_SIZE] = {};
            for(int i=0;i<ret;i++){
                int clientfd = events[i].data.fd;
                if(events[i].events == EPOLLIN){
                    int len=recv(clientfd,buffer,1024,0);
                    if(len <= 0){
                        //返回出错
                        perror("recv()");
                        server.mtx.lock();
                        server.clientset.erase(clientfd);//删除这个client
                        server.delete_client(clientfd);
                        server.mtx.unlock();
                    }else if(len>0){
                        if(strcmp(buffer,"") == 0){
                            server.mtx.lock();
                            server.clientset.erase(clientfd);//client请求关闭，删除这个client,
                            //delete_client(epoll_listen_fd,clientfd);
                            server.mtx.unlock();
                            return;
                        }
                        std::cout<<buffer<<std::endl;
                        server.send2allclient(buffer,clientfd);
                        memset(buffer,0,BUFFER_SIZE);
                    }
                }
            }
        }
    }
}


void thread_http(Tcp_server &server){
    
    struct epoll_event ev,events[MAX_FD_SIZE];
    //int epollfd=server.get_accpetfd();
    int epoll__fd=server.get_http_epoll_fd();
    while(true){
        
        int ret = epoll_wait(epoll__fd,events,MAX_FD_SIZE,-1);
        //perror("epoll_wait");
        
        if(ret){
            char buffer[BUFFER_SIZE] = {};
             std::cout<<"http"<<ret<<std::endl;
            for(int i=0;i<ret;i++){
                std::cout<<"http"<<i<<std::endl;
                int httpclientfd = events[i].data.fd;
                if(events[i].events == EPOLLOUT){
                   // char buff[1024];
                   // strcat(buff, "HTTP/1.0 200 OK\r\n");
                   // strcat(buff, "Server: HHTWS Web Server\r\n");
                  //  strcat(buff, "Connection: close\r\n");
                  //  strcat(buff, "Content-length:100 \r\n");
                  //  strcat(buff, "Content-type: \r\n\r\n");
                  //std::string filecon="HTTP/1.0 200 OK\r\nServer: HHTWS Web Server\r\nConnection: close\r\nContent-length:100 \r\nContent-type: \r\n\r\n";
                    std::string filecon=firstline+header+test;
                    send(httpclientfd, filecon.data(), filecon.size(), 0);
                    ev.data.fd=httpclientfd;
                    ev.events=EPOLLOUT;
                    epoll_ctl(epoll__fd,EPOLL_CTL_DEL,httpclientfd,&ev);
                    close(httpclientfd);
                }
            }
        }
    }
}
int main(int argc,char* argv[]){
    std::cout<<test;
    in_addr_t ip = MYIP;
    int server_port = SERVER_PORT;
    int http_port = HTTP_PORT;
    Tcp_server server(ip,server_port,http_port);
    server.bind_socket();//bind
    int len=MAX_LISTEN;
    server.listen_sock(len);//listen

    std::thread t_accept(thread_accpet,std::ref(server));//accept线程
    std::thread t_listen(thread_listen,std::ref(server));//recv线程
    std::thread t_http(thread_http,std::ref(server));//recv线程
    t_accept.join();
    t_listen.join();
    t_http.join();
    return 0;




}
