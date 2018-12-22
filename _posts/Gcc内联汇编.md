# 				Gcc内联汇编

## 动机

- 用c语言无法表述cpu特权指令。例如:加载LGDT表

## 工作原理

- ###  Example1

  Assembly(*.S):

​		movl $0xffff %eax

​	inline assembly(*.c):

​		asm ("movl $0xffff , %%eax\n");

- ### Syntax

  ​	asm(assembler template 

  ​		: output operands     (optional) 		//输出操作数	

  ​		: input    operands     (optional)  	//输入操作数

  ​		: clobbers		      (optional)

  ​		)

- ### Example 2

     Inline assembly(*.c):
  ​	uint32_t **cr0**;

  ​	asm volatitle  ("movl %%cr0 , %0\n" :"=r"(**cr0**))   //将cr0寄存器的值读到%0 寄存器里 最终赋给cr0变量

  ​	cr0 |= 0x80000000;                                                 //将某一位置为1                                              

  ​	asm volatitle  ("movl %0   ,  %%cr0\n" ::"r"(**cr0**))	

     Generated assembly code (*.s)

  ​	movl %cr0 , %ebx

  ​	movl %ebx ,12(%esp)

  ​	orl     $-2147483648 , 12(%esp)

  ​	movl 12(%esp),  %eax

  ​	movl   %eax, %cr0



  volatile:
  ​	No reordering 

  %0

  ​	The first constraint following

  r

  ​	A constraint
