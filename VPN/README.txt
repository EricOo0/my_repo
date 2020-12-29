腾讯云搭建vpn（回国网络）：仅供学习用

原理：vpn虚拟专用网-用专有的通道连接两个网络（隧道）
	VPN客户机连接到VPN服务器上（加密），通过VPN服务器访问另一个网络
使用协议: pptp点对点隧道协议或l2tp等
	使用pptp协议或者其他协议，对ip包再封一层，送到服务器，服务器解析出来源目的，发送到目的网络，
1、安装shadowssocks软件
https://www.cnblogs.com/youyaoqi/p/11979760.html
	使用python开发的开源代理软件	
	pip python的包管理工具

    先下载相关
	sudo yum install python-setuptools
	sudo yum install python-pip
	pip install shadowsocks
     配置文件
	touch /etc/shadowsocks.json
	{ 
		"server":"0.0.0.0", 
		"server_port":443, 
		"local_address": "127.0.0.1", 
		"local_port":1080, 
		"password":"123456", 
		"timeout":600, 
		"method":"rc4-md5"
		}
		
	启动服务器
	ssserver -c /etc/shadowsocks.json -d start
	关闭
	ssserver -c /etc/shadowsocks.json -d stop
	重启
	ssserver -c /etc/shadowsocks.json -d restart