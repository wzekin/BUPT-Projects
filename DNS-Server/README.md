# DNS 服务器实验报告

## 系统功能的设计

### 基本功能的设计
设计一个DNS服务器程序，读入“域名-IP地址”对照表，当客户端查
询域名对应的IP地址时，用域名检索该对照表，三种检索结果：
* 检索结果为ip地址0.0.0.0，则向客户端返回“域名不存在”的报错消息，而
不是返回IP地址为0.0.0.0（不良网站拦截功能）
* 检索结果为普通IP地址，则向客户返回这个地址（服务器功能）
* 表中未检到该域名，则向因特网DNS服务器发出查询，并将结果返给客户端
（中继功能）
    * 考虑多个计算机上的客户端会同时查询，需要进行消息ID的转换

### 附加功能的设计
* 跨平台设计，程序可以在Windows/Linux上运行
* 使用Hashmap存储host数据，大大优化查询时间
* 使用LinkedHashmap提供LRU缓冲池

## 模块划分
程序主要分为`日志模块`，`数据存储模块`, `数据输入模块`, `DNS解析模块`, `主程序模块`

### 日志模块
* 用于打印日志，便于后期调试

### 数据存储模块
* Hashmap
  * 支持不同类型的key value
  * 用于存储host数据
  * 用于存储发送给远程DNS的client
* LinkedHashmap
  * 用于提供LRU
  

### 数据输入模块
* 读取hosts文件
* 分为hosts和ban_hosts, 便于调用

### DNS解析模块
* 解析DNS报文
* 生成DNS报文
* 替换ID

### 主程序模块
* 初始化Hashmap和LRU
* 初始化句柄
* 处理业务逻辑(见流程图)

## 软件流程图
``` mermaid
graph TD
A(开始) --> B[接收报文]
B --> C{查找host}
C --找到--> Z[返回报文]
C --未找到--> D{查找缓存}
D --找到--> Z
D --未找到--> E[请求服务器]
E --服务器响应--> F[存入缓存]
F --> Z
E --服务器未响应--> X[丢弃]
```

## 测试用例以及运行结果

* 测试工具：`nslookup`
* 测试平台：`Arch`

### 拦截域名

* `test0`	
  > server can't find test0: NXDOMAIN

* `008.cn`  
  >  server can't find test0: NXDOMAIN

* `financial.washingtonpost.com`
  >  server can't find test0: NXDOMAIN

* `lars-s.privat.t-online.de`
  >  server can't find test0: NXDOMAIN

### 已知域名

- `test1`
  > Name:    test1  
    Address: 11.111.11.111
- `test2`
  > Name:    test2  
    Address: 22.22.222.222
- `h255`
  > Name:    h255  
    Address: 192.168.0.255
- `bupt`
  > Name:    bupt  
    Address: 123.127.134.10

### 未知域名

- `baidu.com`
  > Non-authoritative answer:  
    Name:   baidu.com  
    Address: 220.181.38.148  
    Name:   baidu.com  
    Address: 39.156.69.79  
- `github.com`
  > Non-authoritative answer:  
    Name:   github.com  
    Address: 13.250.177.223  
- `zekin.wang`
  > Non-authoritative answer:  
    Name:   zekin.wang  
    Address: 123.56.70.65

## 调试中遇到并解决的问题

在编码和调试的过程中，经常会因为Hashmap强制转换类型导致程序挂掉，通过`valgrind`一步一步的寻找内存泄漏点，修复bug，还有libuv的监听不能通过SIGNINT信号停止，在尝试了各种方案后，最后通过一个idle的句柄监听关闭信号，实现了使用SIGNINT信号关闭程序。

## 课程设计工作总结
### LinkedHashmap与list_head的对比

在验收的过程中，老师提醒我LRU使用list_head作为数据结构。经过翻阅资料，我发现list_head属于嵌入式链表，在没有任何额外开销的情况下，在c语言实现了面向对象的特征，它比我使用的数据结构有如下的优点：
* 没有额外开销，我的数据结构需要引入函数指针，在调用过程中有很多的资源开销
* 方便管理内存，list_head是嵌入式链表，不需要精力去管理内存，不容易有bug
* 面向对象，可以一套代码执行所有操作，不需要适配

本次实验中，我学会了 UDP 通信的基本方法、DNS 协议的基本内容、libuv的使用，以及有关计算机网络编程的调试和分析等，还学习了内核数据结构list_head，收获很大。
