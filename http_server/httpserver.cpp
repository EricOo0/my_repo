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
    int offset =0;
    int nread=0;
    while(1){
        nread=recv(connfd,request+offset,100,0);
        //sleep(0.1);
        std::cout<<"reading offset:"<<offset<<std::endl;
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
    sprintf(response,"HTTP/1.0 200 OK\r\nContent-Type: %s\r\n\r\n",mime);//响应头
    int headlen = strlen(response);
    int filefd = open(filename,O_RDONLY);
    int filelen=read(filefd,response+headlen,sizeof(response)-headlen);//响应内容

    //返回
    write(connfd,response,headlen+filelen);
    std::cout<<"close!"<<std::endl;
    close(connfd);
    
}
int main(){
    int sockfd = create_listenfd();
    int connfd=0;
    struct sockaddr_in client;
    socklen_t client_len = 1;
    std::cout<<"waiting!"<<std::endl;
    while(true){
    std::cout<<"waiting!"<<std::endl;
    if(connfd = accept(sockfd,(struct sockaddr *)&client,&(client_len)) == -1){
        perror("accept()");
        exit(-1);
    }
    std::cout<<"waiting!"<<std::endl;
    handle_con(connfd);
    //close(connfd);
    
    }
    close(sockfd);
    

}