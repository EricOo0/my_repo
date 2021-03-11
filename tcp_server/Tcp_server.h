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
#include <unordered_map>

#include<set>
#include<string>
#include<map>
#include<thread>
#include<mutex>//锁
#include <mysql/mysql.h>

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
        //创建mysql结构
        MYSQL mysql;
        //用户id和fd的对应map
        std::unordered_map<int , std::string>id_map;
        //创建三个epoll reactor模型
        int epoll_accept_fd ;
        int epoll_listen_fd ;
        int epoll_http_fd ;
        //std::map<int,std::string> name;//存储用户名和fd对应关系
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
           // char leave[BUFFER_SIZE];
            //sprintf(leave,"#msg#usrleave||%s$$",id_map[clientfd].data());
           // send2allclient(leave,clientfd);
        }
        void send2allclient(char *buffer,int &selffd){
            for(auto client:clientset){
                if(client !=selffd){
                std::cout<<"send to client:"<<client<<std::endl;
                int len;
                //char buffer_add[BUFFER_SIZE]={};
                //char buffer_send[BUFFER_SIZE]={};
                
                std::string msg=static_cast<std::string>(buffer);
                if(msg.find("#msg#msg") != msg.npos){
                    //正常通信消息直接转发
                    if(( len = send(client,buffer,strlen(buffer),0)) ==-1){
                    perror("send()");
                    mtx.lock();
                    clientset.erase(client);
                    id_map.erase(selffd);
                    delete_client(client);
                    mtx.unlock();
                }
                }
                else{
                    //hello消息或者leave消息或者hellreply消息，加上id转发
                    //回复消息格式#msg#type||id$$content；
                     msg = msg +"||"+id_map[selffd]+"$$";
        
                     std::cout<<msg<<std::endl;
                    if(( len = send(client,msg.data(),msg.size(),0)) ==-1){
                        perror("send()");
                        mtx.lock();
                        clientset.erase(client);
                        id_map.erase(selffd);
                        delete_client(client);
                        mtx.unlock();
                     }
                     sleep(0.1);
                }
                //开头附加加消息信息
               // sprintf(buffer_new,"#total");
                //sprintf(buffer_new,"#client%d#",selffd);
               
                }
            }
        }
        int connect2sql(const char *db){
            if(!mysql_real_connect(&mysql,"49.234.75.120","wzf","weizhifeng10",db,3306,"unix_socket",0)){
                std::cout<<"connect to "<<db<<" failed!"<<std::endl;
                std::cout<<mysql_error(&mysql);
                return 0;
            }
            std::cout<<"connect to "<<db<<" successful!"<<std::endl;
            return 1;
        }
        std::string usr_authorize(std::string usrname,std::string passwd){
            //判断账户密码是否正确
            std::string cmd="select * from usrname where usrname=";
            cmd =cmd + "'"+usrname+"'";
            int status = mysql_query(&mysql,cmd.data());
            if(status != 0){
                std::cout<<"cmd error:"<<cmd<<std::endl;
                return NULL;
            }
            MYSQL_RES *res_set;
            res_set = mysql_store_result(&mysql);//存储执行结果

            int count = mysql_num_rows(res_set);//统计多少行
            if(count == 0 ){
                std::cout<<"usrname not exit!"<<std::endl;
                return "failed";
            }
            MYSQL_ROW row = mysql_fetch_row(res_set);//row[0]是个void型指针
            std::string pwd = static_cast<std::string>(row[1]);//判断密码是否正确
            if(pwd != passwd){
                std::cout<<"cmd error:"<<cmd<<std::endl;
                return "failed";
            }
            //返回用户ID
            std::string id = static_cast<std::string>(row[2]);
            return id;
        }
        int usr_register(std::string msg){
            //#register#usrname||passwd$$id
            std::string usrname;
            std::string passwd;
            std::string id;
            int pos=msg.find("||");
            int pos2=msg.find("$$");
            usrname = msg.substr(10,pos-10);
            passwd = msg.substr(pos+2,pos2-pos-2);
            id=msg.substr(pos2+2);
            std::string cmd="insert into usrname values ";
            cmd =cmd + "('"+usrname+"','"+passwd+"','"+id+"')";
            int status = mysql_query(&mysql,cmd.data());
            if(status != 0){
                std::cout<<mysql_error(&mysql);
                std::cout<<"register failed:"<<cmd<<std::endl;
                return 0;
            }
            return 1;
        }
        std::string msg_judge(char *buffer,int clientfd){
            std::string msg=static_cast<std::string>(buffer);
            std::cout<<"buffer:"<<buffer<<std::endl;
            if(msg.find("#login#") != msg.npos){
                //login消息
                const char db[]="login";
                //初始化mysql
                if(!mysql_init(&mysql))//初始化mysql结构
                {
                    std::cout<<mysql_error(&mysql);
                }
                std::cout<<"mysql init!"<<std::endl;
                if(!connect2sql(db)){
                    return "#login#failed";
                    
                }
                std::string usrname;
                std::string passwd;
                int pos=msg.find("||");
                usrname = msg.substr(7,pos-7);
                passwd = msg.substr(pos+2);
                std::string id = usr_authorize(usrname,passwd);
                if(id == "failed"){
                    std::cout<<"usrname or passwd is incorret";
                    return "#login#failed";
                    mysql_close(&mysql);
                }
                std::cout<<id;
                mysql_close(&mysql);   
                id_map[clientfd] =id ; 
                return "#login#"+id;
            
            }
            if(msg.find("#register#") != msg.npos){
                //判断是否是注册消息
                const char db[]="login";
                //初始化mysql
                if(!mysql_init(&mysql))//初始化mysql结构
                {
                    std::cout<<mysql_error(&mysql);
                }
                std::cout<<"mysql init!"<<std::endl;
                if(!connect2sql(db)){
                    return "#register#failed";
                    
                }
                if(usr_register(msg)){
                    mysql_close(&mysql);
                    return "#register#success";
                }
                return "#register#failed";
            }
            if(msg.find("#msg#") != msg.npos){
                //非login消息
                return "#msg#";
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
