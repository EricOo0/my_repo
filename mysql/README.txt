centos 安装mysql
	客户端：
	yum install mysql
	服务器：
	 weget https://dev.mysql.com/get/mysql80-community-release-el7-3.noarch.rpm  下载服务其
	  rpm -ivh mysql80-community-release-el7-3.noarch.rpm 安装
	  sudo yum install mysql-community-server
	  yum install mysql-devel
初始化并开启服务	
mysqld --initialize;
chown mysql:mysql /var/lib/mysql -R;
systemctl start mysqld.service; 启动服务  systemctl stop mysqld.service;关闭服务
systemctl  enable mysqld;

连接到mysql
mysql -u root -p

修改初始密码
alter user user() identified by "weizhifeng10";
https://blog.csdn.net/wrh_csdn/article/details/79483590
user() 当前用户

sudo netstat -nlp 看运行状态

连接mysql的程序
要包含头文件目录-I /usr/include/mysql，动态库目录-L /usr/lib/mysql 动态库-lmysqlclient。(g++ test.cpp -I/usr/include/mysql -L/usr/lib64/mysql  -lmysqlclient   )

连接mysql服务器需要的信息:
host：0.0.0.0
port:3306
username:root
password:weizhifeng10
db：数据库名

可以用localhost连接但不能用ip连接
	原因：mysql权限没开
程序中利用mysql C api连接数据库，
	需要包含mysql.h;编译时带上头文件路径
	MYSQL mysql; mysql_init(&mysql)； 初始化mysql结构,使用指针记得分配空间或者使用MYSQL *mysql mysql=mysql_init(NULL)；
	mysql_real_connect();连接到数据库
	mysql_query(mysql, "SELECT * FROM host_summary");执行sql语句；返回值：Zero for success. Nonzero if an error occurred.
	MYSQL_RES *res_set;res_set = mysql_store_result(&mysql); 存储返回值
	int count = mysql_num_rows(res_set);//统计多少行
	num_fields = mysql_num_fields(res_set);//统计多少个字段 int 
	mysql_fetch_lengths()//长度
	row = mysql_fetch_row(res_set))//读取到第几行



创建一个登录用户名和密码的数据库
create database login;
create table usrname (usrname VARCHAR(10) not null ,passwd varchar(15));
insert into usrname values("wzf","******");
