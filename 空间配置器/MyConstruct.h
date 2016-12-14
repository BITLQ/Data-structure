#pragma once
#include<new.h>

template<typename T>
void distory(T* poniter)
{
	//显示的调用析构函数
	poniter->~T();
}

template<typename T1,typename T2>
inline void construct(T1* p,const T2& value)
{
	new(p)T1(value);
}
