// Trace 跟踪
// 
#pragma once
#include<string>
#include<windows.h>
#include<vector>
#include "MyList.h"

//测试分配释放的场景  
void TestDefaultAllocTemplate2()  
{  
    int begin = GetTickCount();  
	DefaultAllocTemplate<false,0> d;  
    vector<pair<void*,int> >v;  
    v.push_back(make_pair(d.Allocate(130),130));  
    for(int i = 0; i < 100;++i)  
    {  
        v.push_back(make_pair(d.Allocate(28),28));  
    }  
    while(!v.empty())  
    {  
		d.DeAllocate(v.back().first,v.back().second);  
        v.pop_back();  
    }  
  
    for(int i = 0; i < 100;++i)  
    {  
        v.push_back(make_pair(d.Allocate(28),28));  
    }  
    while(!v.empty())  
    {  
		d.DeAllocate(v.back().first,v.back().second);  
        v.pop_back();  
    }  
    int end = GetTickCount();  
    cout<<end - begin<<endl;  
}  


void Display(const List<int>& l)
{
		/*Node* cur = _head->_next;
		while(cur != _head)
		{
			cout<<cur->_data<<" ";
			cur = cur->_next;
		}*/

		List<int>::ConstIterator it = l.begin();
		while(it != l.end())
		{
			cout<<*it<<" ";
			++it;
		}
		cout<<endl;
}

void TestConstList(List<int>& l)
{
	l.push_back(1);
	l.push_back(2);
	l.push_back(3);
	l.push_front(0);

	Display(l);
}

//测试自定义List
void TestMyList()
{
	List<int> l;
	TestConstList(l);
}
