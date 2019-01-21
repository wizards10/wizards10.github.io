---
layout: post
title: "leetcode栈、队列"
date: 2019-1-12
excerpt: "栈、队列"
tags: [数据结构、队列、栈]
comments: true
---

## 225.用队列实现栈

```c++
class MyStack {
public:
    /** Initialize your data structure here. */
    MyStack() {
        
    }
    
    /** Push element x onto stack. */
    void push(int x)
    {
         queue<int> temp;
         temp.push(x);
         while(!data.empty())
         {
             temp.push(data.front());
             data.pop();
         }
         while(!temp.empty())
         {
             data.push(temp.front());
             temp.pop();
         }
         return;
    }
    
    /** Removes the element on top of the stack and returns that element. */
    int pop() 
    {
        if(data.size() > 0)
        {
            int num = data.front();
            data.pop();
            return num;
        }
        else
        {
            return 0;
        }
    }
    
    /** Get the top element. */
    int top() 
    {
        return data.front();    
    }
    
    /** Returns whether the stack is empty. */
    bool empty()
    {
        return data.empty();
    }
private:
    queue<int> data;
};
```

## 232.用栈实现队列

```c++
class MyQueue {
public:
    /** Initialize your data structure here. */
    MyQueue()
    {
        
    }
    
    /** Push element x to the back of queue. */
    void push(int x) 
    {
        stack<int> temp;
        while(!data.empty())
        {
            temp.push(data.top());
            data.pop();
        }
        temp.push(x);
        while(!temp.empty())
        {
            data.push(temp.top());
            temp.pop();
        }
        return;
    }
    
    /** Removes the element from in front of queue and returns that element. */
    int pop()
    {
        if(data.size() > 0)
        {
            int num = data.top();
            data.pop();
            return num;
        }
        else
        {
            return 0;
        }
    }
    
    /** Get the front element. */
    int peek() 
    {
        return data.top();
    }
    
    /** Returns whether the queue is empty. */
    bool empty()
    {
        return data.empty();
    }
private:
    stack<int> data;
};
```
























