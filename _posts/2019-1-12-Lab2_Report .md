---
layout: post
title: "智能指针"
date: 2019-1-11
excerpt: "智能指针"
tags: [c++，智能指针]
comments: true
---

### shared_pointer

- **不要将原生指针赋予不同shared_pointer**

  ```c++
  class Node
  {
  public:
      Node(int val):
          x(val)
      {
          cout<<"Node Constructor----------------->"<<endl;
      }
      ~Node()
      {
          cout<<"Node destructor------------------>"<<endl;
      }
      int x;
  };
  class ListNode
  {
  public:
      ListNode(int v):
          val(v),node(new Node(0))
      {
          cout<<"ListNode Constructor--------------->"<<endl;
      }
      ~ListNode()
      {
          cout<<node->x<<endl;
          delete node;
          node = nullptr;//避免野指针
          cout<<"ListNode destructor--------------->"<<endl;
      }
      int val;
      Node* node;
  };
  void test()
  {
      ListNode* s = new ListNode(0);
      shared_ptr<ListNode> sp1(s);
      shared_ptr<ListNode> sp2(sp1);
      shared_ptr<ListNode> sp3(s);
      cout<<"sp1 use count =  "<<sp1.use_count()<<endl;
      cout<<"sp3 use count =  "<<sp3.use_count()<<endl;
  }
  ```

  node将会被释放两次而导致段错误。

- **循环引用问题**

  ```c++
  struct ListNode
  {
      ListNode(int v):
          val(v)
      {
          cout<<"ListNode Constructor--------------->"<<endl;
      }
      ~ListNode()
      {
          cout<<"ListNode destructor--------------->"<<endl;
      }
      shared_ptr<ListNode> pre;
      shared_ptr<ListNode> next;
      int val;
  };
  void test()
  {
      shared_ptr<ListNode> sp1(new ListNode(1));
      shared_ptr<ListNode> sp2(new ListNode(2));
      cout<<"sp1 use count =  "<<sp1.use_count()<<endl;
      cout<<"sp2 use count =  "<<sp2.use_count()<<endl;
      sp1->next = sp2;
      sp2->pre  = sp1;
      cout<<sp1.use_count()<<endl;
      cout<<sp2.use_count()<<endl;
  
  ```




































