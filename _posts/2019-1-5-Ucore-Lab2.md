---
layout: post
title: "UCore-Lab2"
date: 2019-1-2
excerpt: "Lab2"
tags: [操作系统,Ucore]
comments: true
---

## X86特权级(privilege levels)

- Linux和Ucore只使用0和3

- 一些指令（比如特权指令）只能执行在ring 0(eg: lgdt)
- CPU在如下时刻会检查特权级
  - 访问数据段
  - 访问页
  - 进入中断服务例程(ISRS)
  - ...

- 段选择子

  - ![](../assets/img/段选择子1.png)

    ​		

- 段描述符中的DPL
  - ![](../assets/img/DPL.png)

- 中断门中的DPL
  - ![](../assets/img/中断门_DPL.png)

- 特权转移

  - 访问门时 CPL<=DPL[门] & CPL >= DPL[段]

  - 访问段时 MAX(CPL,RPL) <= DPL

