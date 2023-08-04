# Group129_Homework_汇总报告

## 基本信息
组号：第129组  

姓名：谢著海  

学号：202100460142  

处理器型号：AMD Ryzen 7 5800H with Radeon Graphics 3.20 GHz

操作系统：Windows 10 家庭中文版

编译器：VS2022、PyCharm

虚拟机：Ubuntu2020.04.3

模拟器:Gem5

运行指导：  
- project8中的代码为ARM架构，需要在gem5模拟器下运行。  
- 其它项目的C代码均可直接运行。
- 其它项目的python代码在安装对应的库后可直接运行。

项目完成情况：完成所有项目  

(由于创建文件夹时对命名的疏忽，文件夹显示顺序稍有错乱)

## 项目运行结果简介
### project01_implement the naïve birthday attack of reduced SM3
完成情况：
- 完成SM3编写
- 利用生日悖论对SM3进行碰撞攻击（结果如下表所示）
   
| 比特数 | 8bit | 16bit | 24bit | 32bit|
|-------|-------|-------|-------|-------|
|攻击时间 | 1.600ms | 317.195ms |788010.291ms|586404.935ms|

### project02_implement the Rho method of reduced SM3
完成情况：
- 完成SM3编写
- 利用Pollard-Rho算法对SM3进行碰撞攻击（结果如下表所示）
 
| 比特数 | 8bit | 16bit | 24bit | 32bit|
|-------|-------|-------|-------|-------|
|攻击时间 | 0.933ms | 118.207ms |96883.906ms|7489641.282ms|

### project03_implement length extension attack for SM3
完成情况：
- 完成SM3编写
- 完成长度拓展攻击函数编写，实现SM3的长度拓展攻击
### project04_do your best to optimize SM3 implementation
完成情况：
- 完成SM3编写
- 利用o2编译进行加速
- 优化部分代码，减少使用循环
- 使用移位代替乘号，减少开销
- 最终实现所编写的SM3加密10000次512bit数据的时间与库函数加密时间相差无几
### project05_Impl Merkle Tree following RFC6962
完成情况：
- 实现Merkle_tree
### project06_impl this protocol with actual network communication
完成情况：
- 完成哈希链的范围证明
- 模拟了协议运行的情况
- 实现在零知识的情况下对Alice的年龄证明
### project07_ Try to Implement this scheme
完成情况：
- 完成泛化哈希链的编写
- 实现最终的HashWire协议
### project08_AES impl with ARM instruction
完成情况：
- 使用ARM指令实现AES加密函数的编写
- 在gem5模拟器下成功运行用ARM指令编写的AES
### project09_AES / SM4 software implementation
完成情况：
- 实现AES的C语言编写
- 实现SM4的C语言编写
### project10_report on the application of this deduce technique in Ethereum with ECDSA
完成情况：
- 介绍ECDSA基本情况、原理
- 介绍ECDSA数字签名过程
- 介绍以太坊
- 介绍以太坊中对ECDSA的使用
- 介绍ECDSA与区块链的关系
### project11_impl sm2 with RFC6979
完成情况：
- 实现SM2的签名算法
- 实现SM2的签名验证算法
- 实现SM2的密钥交换算法
- 实现SM2的加密算法
- 实现SM2的解密算法

### project12_verify the above pitfalls with proof-of-concept code
完成情况：
- 验证了泄露k导致d的泄露
- 验证了重复使用k导致d的泄露
- 验证了两个用户,若使用相同的K,可推断出相互的d
- 验证了(r,s)是有效签名，则(r,-s)也是有效签名
- 验证了在ECDSA和Schnorr签名中使用相同的d和k,会导致d泄露
### project13_Implement the above ECMH scheme
完成情况：
- 介绍多重哈希求和结构
- 基于SM2实现ECMH算法
### project14_Implement a PGP scheme with SM2
完成情况：
- 实现PGP的加密算法
- 实现PGP解密算法
- 模拟PGP的使用
### project15_implement sm2 2P sign with real network communication
完成情况：
- 实现客户端代码编写
- 实现服务器端代码编写
- 完成SM2的双方签名
### project16_implement sm2 2P decrypt with real network communication
完成情况：
- 实现客户端代码编写
- 实现服务器端代码编写
- 完成SM2的双方解密
### project17_比较Firefox和谷歌的记住密码插件的实现区别
完成情况：
- 介绍Firefox记住密码的实现方式
- 介绍Google记住密码的实现方式
- 比较两者实现方式的不同
### project18_send a tx on Bitcoin testnet, and parse the tx data down to every bit, better write script yourself
完成情况：
- 编写脚本发送指定信息到比特币测试网站
- 在实际网站访问一笔交易并编写脚本读取交易信息
### project19_forge a signature to pretend that you are Satoshi
完成情况：
- 实现ECDSA算法
- 实现在不检查信息的情况下对签名的伪造
### project20_ECMH PoC
完成情况：
与project13重复，在此不多赘述
### project21_Schnorr Bacth
完成情况：
- 实现schnorr签名算法
- 实现schnorr验证算法
### project22_research report on MPT
完成情况：
- 介绍Trie Tree
- 介绍Patricia Tree
- 介绍Merke Tree
- 介绍以太坊中的MPT

简介报告结束，具体内容在请查看对应文件夹。
