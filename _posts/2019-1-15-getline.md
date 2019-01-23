---
layout: post
title: "getline()"
date: 2019-1-16
excerpt: "c++输入"
tags: [c++]
comments: true

---

## 输入一行以逗号分割的坐标，提取各个坐标

```c++
#include <iostream>
#include <vector>
using namespace std;
int main()
{
    int len = 0;
    int ch;
    string str;
    int i = 0 ;
    getline(cin , str , '\n');
    vector<int> vec;
    const int STATE_BEGIN = 0;
    int STATE = STATE_BEGIN;
    const int STATE_NUMBER = 1;
    const int STATE_COMMA = 2;
    int number = 0;
    for(int i = 0 ; i < str.size() ; i++)
    {
    	if(str[i] == ' ')
        {
            continue;
        }
        switch(STATE)
        {
            case STATE_BEGIN:
            {
            	if(str[i] >= '0' && str[i] <= '9')
                {
                    STATE = STATE_NUMBER;
                }
                i--;
                break;
            }
            case STATE_NUMBER:
            {
            	if(str[i] >= '0' && str[i] <= '9')
                {
                    number = number * 10 + str[i] - '0';
                }
                else
                {
                    i--;
                    STATE = STATE_COMMA;
                }
            }
            case STATE_COMMA:
            {
            	vec.push_back(number);
                STATE = STATE_NUMBER;
                number = 0;
                break;
            }
        }
    }
    vec.push_back(number);
    cout<<str<<endl;
   	for(int i = 0 ; i < vec.size() ; i++)
    {
        cout << vec[i] << endl;
    }
    cout << endl;
    return 0;
}
```




























