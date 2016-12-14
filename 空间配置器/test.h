// Trace 跟踪
// 
#include<string>
#include "alloc.h"
#include<stdarg.h>
#include<windows.h>
#include<vector>

// Trace 跟踪
// 

#define __DEBUG__

FILE* fOut = fopen("trace.log", "w");

static string GetFileName(const string& path)
{
	char ch = '/';

#ifdef _WIN32
	ch = '\\' ;
#endif

	size_t pos = path. rfind(ch );
	if (pos == string:: npos)
		return path ;
	else
		return path .substr( pos + 1);
}
// 用于调试追溯的trace log
inline static void __trace_debug (const char* function,
								  const char * filename, int line , char* format , ...)
{
	// 读取配置文件
#ifdef __DEBUG__
	// 输出调用函数的信息
	fprintf(stdout , "【 %s:%d】%s" , GetFileName (filename). c_str(), line , function);
	fprintf(fOut, "【 %s:%d】%s" , GetFileName (filename). c_str(), line , function);

	// 输出用户打的trace信息
	va_list args ;
	va_start (args , format);
	vfprintf (stdout , format, args);
	vfprintf (fOut , format, args);
	va_end (args );
#endif
}

#define __TRACE_DEBUG(...)  \
	__trace_debug(__FUNCTION__ , __FILE__, __LINE__, __VA_ARGS__);

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
