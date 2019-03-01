# 								Socket总结

### ipv4套接字结构:

```c
struct sockaddr_in
{
    uint8_t 		sin_len;
    sa_family_t 	sin_family;
    in_port_t 		sin_port;
    struct in_addr 	sin_addr;
    char         	sin_zero[8];
}
struct in_addr
{
    in_addr_t s_addr;
}
```

### 网络字节序与主机字节序

```c
uint16_t htons(uint16_t host16bitvalue); //port
uint16_t htonl(uint32_t host32bitvalue); //ip

uint16_t ntohs(uint16_t net16bitvalue);  //port
uint16_t ntohl(uint16_t net16bitvalue); //ip
```

### Socket

```c
#include <sys/socket.h>
int socket(int family , int type , int protocol);
//Ipv4: family AF_INET
//protocol: SOCK_TREAM
```

### Connect

```c
#include <sys/socket.h>
int connect(int sockfd , const struct sockaddr* servaddr , socklen_t addrken)
```

- 如果sockfd是TCP套接字，会激发TCP三路握手过程，仅在连接建立成功或失败时才返回
- 若TCP客户没有收到SYN分节的响应，则返回ETIMEOUT错误。
- 若对客户的SYN的响应时RST，则表明该服务器主机在我们指定的端口上没有进程在等待与之连接  (***hard error***)

- 若客户发出的SYN在中间的某个路由器引发了一个“**destination unreachable**”(soft error)

### Bind

```c
#include <sys/socket.h>
int bind(int sockfd , const struct sockaddr* myaddr , socklen_t addrlen)
```

- myaddr中的port和ip可以都指定也可以都不指定

### Listen

```c
#include <sys/socket.h>
int listen(int sockfd , int backlog);
```

- backlog为已完成队列和未完成队列之和
- 来自客户的SYN到达时，TCP在未完成连接队列中创建一个新项，然后响应以三路握手的第二个分节：服务器**的SYN响应**，**其中捎带对客户SYN的ACK**，**这一项一直保留在未完成队列中**，直到三路握手的第三个分节到达或超时。当调用accept时，已完成连接队列头项将返回给进程，如果该队列为空，那么进程将被投入睡眠。
- 如果队列满了，TCP就忽略该分节，不发送RST。

### Accept

```c
#include <sys/socket.h>
int accept (int sockfd , struct sockaddr* cliaddr ,socklen* addrlen);
```

### Select

```c
#include <sys/select.h>
#include <sys/time.h>
int select(int maxfdpl , fd_set *readset , fd_set *writeset , fd_set *exceptset , 
           const struct timeval *timeout)
    		//返回：若有就绪描述符则为其数目，若超时则为0.若出错则为-1
```

- timeout中timeval结构如下:

  ```c
  struct timeval
  {
      long tv_sec;
      long tv_usec;         /*microseconds微秒*/
  }
  //(1) 永远等下去：参数为NULL
  //(2) 等待一定时间，设置秒与微秒
  //(3) 立即返回，设置为0
  ```

- 必须要的一些宏

  ```c
  fd_set;
  FD_ZERO(&rset);                       //初始化集合
  FD_SET(1 ，&rset);                    //将fd = 1 设置为关心 
  FD_ISSET(1 , &rset);                  //fd = 1 ,是否在已就绪的名单内
  ```

- 例子

  ```c++
  	int maxFD = _fd;
      struct timeval maxSelectTime;
      fd_set fds;
      maxSelectTime.tv_sec = timeout / 1000000;
      maxSelectTime.tv_usec = timeout % 1000000;
      while(true)
      {
          FD_ZERO(&fds);
          FD_SET(_fd,&fds);
          rc = select(maxFD + 1,&fds,NULL,NULL,timeout >= 0 ? &maxSelectTime:NULL);
          // 0 means timeout
          if(rc == 0)
          {
              rc = EDB_TIMEOUT;
              goto done;
          }
          // if < 0 something wrong
          if(rc < 0)
          {
              rc = SOCKET_GETLASTERROR;
              if(EINTR == rc)
              {
                  continue;
              }
              printf("Failed to select from socket,rc = %d",rc);
              rc = EDB_NETWORK;
              goto error;
          }
      
          if(FD_ISSET(_fd,&fds))
          {
              break;
          }
      }
  ```

##  一些与网络相关的内核参数

- time_wait

  - 调低端口释放后的等待时间，默认为60s，修改为15~30s
    sysctl -w net.ipv4.tcp_fin_timeout=30

  - 修改tcp/ip协议配置， 通过配置/proc/sys/net/ipv4/tcp_tw_resue, 默认为0，修改为1，释放TIME_WAIT端口给新连接使用
    sysctl -w net.ipv4.tcp_timestamps=1

  - 修改tcp/ip协议配置，快速回收socket资源，默认为0，修改为1
    sysctl -w net.ipv4.tcp_tw_recycle=1

- 端口

  - vi /etc/sysctl.conf
    net.ipv4.ip_local_port_range = 10000     65000 

  - sysctl -p 生效

- 文件数目
  - ulimit -n 100000  //重启失效

# accept + select 的一些问题

- unp 中写道如果accept设置为阻塞，select返回监听套接字可读、accept之前对端发送RST分节会导致accept会阻塞到下一个连接(按照书中的方法我并没有测出来)

  ```c
          if(FD_ISSET(_fd,&fds))
          {
              rc = EDB_OK;
              sleep(5);
              *sock = ::accept(_fd , addr , addrlen);
              printf("Accept successfull \n");
              break;
          }
  ```

  ```c
      void Accept_Block()
      {
          struct linger ling;
          int sock;
          sock = socket(AF_INET,SOCK_STREAM,0);
          connect(sock,(struct sockaddr*)&addr , sizeof(addr));
          ling.l_onoff = 1; //cause RST to be sent on close()
          ling.l_linger = 0;
          setsockopt(sock , SOL_SOCKET , SO_LINGER , &ling , sizeof(ling));
          close(sock);
          return;
      }
  ```

