/* 模拟实现STL的空间配置器
包括：一级配置器，二级配置器；

//每一级配置器里的两个基本函数：allocate和deallocate

*/
//首先，一级配置器：

#include<iostream>
#include<stdarg.h>

using namespace std;

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
    fprintf(stdout , "【%s:%d】%s" , GetFileName (filename). c_str(), line , function);  
      
    fprintf(fOut, "【%s:%d】%s" , GetFileName (filename). c_str(), line , function);  
    // 输出用户打的trace信息  
    va_list args ;  
    va_start (args , format);  
    //fprintf(stdout,"%c",'\n');  
    vfprintf (stdout , format, args);  
      
    vfprintf (fOut , format, args);  
    fprintf(fOut,"%c",'\n');  
    fprintf(stdout,"%c",'\n');  
    va_end (args );  
#endif  
}  
#define __TRACE_DEBUG(...) \
	__trace_debug(__FUNCTION__ , __FILE__, __LINE__, __VA_ARGS__);  



/******   一级配置器   ******/
//就是对malloc和free的封装

template<int inst>  //预留参数，以待将来
class MallocAllocTemplate
{
	static void* OomMalloc(size_t n);
	static void (* MallocAllocOomHandler)();

public:
	//allocate函数
	static void* Allocate(size_t n)
	{
		__TRACE_DEBUG("向一级配置器申请%u个字节",n);
		void* ret = malloc(n);
		if(ret == NULL)
			ret = OomMalloc(n);
		return ret;
	}

	//deallocate函数
	static void Deallocate(void* p,size_t n)
	{
		__TRACE_DEBUG("一级配置器回收%u个字节",n);
		free(p);
		p = NULL;
	}

	//仿真的set_new_handler,通过此函数制定自己的out_of_memory操作
	static void(*set_malloc_handler(void(*f)()))()
	{
		void (*old)() = MallocAllocOomHandler;
		MallocAllocOomHandler = f;
		return (old);
	}
};

template <int inst>
void* MallocAllocTemplate<inst>::OomMalloc(size_t n)
{
	void (*MyMallocHandler)();
	void* ret;

	for(;;)
	{
		MyMallocHandler = MallocAllocOomHandler;

		if(MyMallocHandler == 0)
		{throw bad_alloc();}              //如果句柄未定义抛出异常；

		(*MyMallocHandler)();             //如果句柄被定义，则调用；
		ret = malloc(n);                  //继续尝试malloc；
		if(ret) 
			return ret;
	}
}


//malloc失败时调用的失败处理函数中的句柄
//模仿C++的set_new_handler（参考STL源码剖析 && effective C++）
//namespace std
//{
//	typedef void (*new_handler)();
//	new_handler set_new_handler(new_handler p)throw();//不抛出异常
//}
//STL中没有对此句柄进行定义，留给客户定义，允许内存分配无法满足时调用；
template<int inst>
void (* MallocAllocTemplate<inst>::MallocAllocOomHandler)() = 0;

//注意，以下直接将ints指定为0
typedef MallocAllocTemplate<0> MallocAlloc;


/*****   二级配置器  *****/

//空间配置器的设置初衷是为了减少频繁的申请内存所带来的内存碎片问题和系统开销；
//较一级配置器复杂，利用内存池和自由链表管理内存；

/*(引自STL源码剖析)：二级配置器的做法：如果区块足够大，超过128 Byte时，就移交第一级配置器
处理，当区块小于128 Byte时，则以内存池管理：每次配置一大块内存，并维护对应的自由链表，下次
若有相同大小的内存需求，就直接从free_list中取出（头删），如果客户端释还小额区块，就由配置器
回收到free_list，为了方便管理，SGI配置器会主动将任何小额区块的内存需求上调至8的倍数；

而内存池向系统申请的所有内存都是在程序自行完成后才还回去的，这是一种空间换时间的做法；
而为了维护自由链表，每个节点也不必需要额外的指针，而是利用和最开始那个固定对象大小的内存池模拟
实现的方法类似，不过SGI是利用联合，char[1]是为了方便客户端查看使用情况；

union obj{
      union obj* free_list_link;
	  char client_data[1];
}
*/

//可以从SGI使用枚举定义常量而没有用define回忆define的优点和缺陷
 enum {__ALIGN = 8};      //小区块的上调边界，即最小也是8，即指针大小的平台无关性；
 enum {__MAX_BYTES = 128};  //小区块的上限
 enum {__NFREELISTS = __MAX_BYTES/__ALIGN};  //free_list的长度

 //注意第一个参数是线程安全的，不讨论；
 template<bool threads,int inst>
 class DefaultAllocTemplate
 {
	 //维护自由链表需要的指针
	 union obj
	 {
		 union obj* freeListLink;
		 char clientData[1];
	 };
	 //长度为16自由链表
	 static obj* volatile freeList[__NFREELISTS];
	 //确定自由链表下标的函数，决定使用第几号自由链表下的内存；
	 static size_t FREELIST_INDEX(size_t bytes)
	 {
		 //这是一种略微提高效率的写法，即少了一次模除，但是降低了可读性
		 //return bytes%__ALIGN == 0 ? (bytes/__ALIGN) - 1:bytes/__ALIGN;
		 return (((bytes) + __ALIGN - 1)/__ALIGN - 1);
	 }

	 //提升至8的倍数
	 static size_t RoundUp(size_t bytes)
	 {
		 return (((bytes) + __ALIGN-1) & ~(__ALIGN - 1));
	 }

	 //内存池开始和结束:注意这里是char*,静态成员变量都得在类外初始化
	 static char* start_free;    //内存池起始位置，只在ChunkAlloc中变化
	 static char* end_free;      //内存池结束位置，只在ChunkAlloc中变化
	 static size_t heap_size;    //代表的是从程序开始到目前为止内存池向系统申请的总内存大小

	 //Refill  尝试从内存池申请空间填充free_list
	 static void* Refill(size_t n);

	 //向内存池申请
	 static char* ChunkAlloc(size_t n,size_t& nobjs);

 public:
	 //公共接口
	 static void* Allocate(size_t n);
	 static void DeAllocate(void* p, size_t /* n */);
 };

 //初始化
 template<bool threads,int inst>
 typename DefaultAllocTemplate<threads,inst>::obj* volatile
	 DefaultAllocTemplate<threads,inst>::freeList[__NFREELISTS] = {0};

 template<bool threads,int inst>
 size_t DefaultAllocTemplate<threads,inst>::heap_size = 0;
 
 template<bool threads,int inst>
 char* DefaultAllocTemplate<threads,inst>::start_free = 0;

 template<bool threads,int inst>
 char* DefaultAllocTemplate<threads,inst>::end_free = 0;


 //从allocate(空间配置函数开始)
template<bool threads,int inst>
void* DefaultAllocTemplate<threads,inst>::Allocate(size_t n)
{
	__TRACE_DEBUG("向二级配置器申请%u个字节",n);
	obj* volatile *myFreeList; //自由链表的地址，注意：二级指针
	obj* result;               //作为返回的结果      
	
	//首先判断申请的大小，大于128直接去一级配置器
	if(n > 128){return (MallocAlloc::Allocate(n));}

	//小于128，则在自由链表中找
	myFreeList = freeList + FREELIST_INDEX(n);  //分配8的倍数的大小，找到对应下标

	result = *myFreeList;   //采用头删的方法将小块分配
	if(result == 0)
	{
		//对应的自由链表的下面没有可用内存，则从内存池申请填充
		void* ret = Refill(RoundUp(n));
		return ret;
	}
	//头删
	*myFreeList = result->freeListLink;

	return (result);
}

//deallocate 函数
template<bool threads,int inst>
void DefaultAllocTemplate<threads,inst>::DeAllocate(void* p, size_t n )
{
	//还内存，同样分两级
	obj* q = (obj*)p;
	obj* volatile *myFreeList;

	//大于128直接调用一级配置器
	if(n > 128){ MallocAlloc::Deallocate(p,n); return;}

	__TRACE_DEBUG("二级配置器回收%u个字节",n);
	//否则，将内存直接链在自由链表上，这也是空间配置器的精妙之处；
	myFreeList = freeList + FREELIST_INDEX(n);

	//头插,回收区块
	q->freeListLink = *myFreeList;
	*myFreeList = q;
}

//实现refill,对自由链表的填充
template<bool threads,int inst>
void* DefaultAllocTemplate<threads,inst>::Refill(size_t n)
{
	size_t nobjs = 20;  //默认的一次向内存池申请20个对象的大小，总不能一次一个一个的申请吧；
	
	//调用chunkallock函数，向内存池申请；
	char* chunk = ChunkAlloc(n,nobjs);  //参数nobjs是引用传递；
	
	obj* volatile *myFreeList;  //哪里都有它；
	obj* result;
	obj* curObj,*nextObj;        //代表当前和下一个，后面会看到具体作用；

	//如果只申请了一个对象的大小
	if(nobjs == 1) return (chunk);

	//否则，我们就把剩余的都链在自由链表上；
	//（为什么会有剩余，就是nobjs = 20 并且传引用的意义）
	myFreeList = freeList + FREELIST_INDEX(n);
	//第一块返回
	result = (obj*)chunk;

	//先把剩余的全部链在自由链表
	*myFreeList = nextObj = (obj*)(chunk + n);

	//对内存进行obj指针的插入
	int i;
	for(i = 1;;++i)
	{
		//就是一个循环植入的过程；
		curObj = nextObj;
		nextObj = (obj*)(((char*)nextObj) + n);
		if(nobjs - 1 == i)   //循环次数可以推演，注意，前面已经有一块给了result;
		{
			//最后一个置为NULL
			curObj->freeListLink = 0;
			break;
		}
		else
		{
			//当前空间存放下一块空间的地址
			curObj->freeListLink = nextObj;
		}
	}
	return (result);
}


/********   下面就是类似内存池的操作 ******/
//实现chunk_alloc
template<bool threads,int inst>
char* DefaultAllocTemplate<threads,inst>::ChunkAlloc(size_t size,size_t& nobjs)
{
	
	//注意，nobjs传的是引用；size在调用的时候已经提升为8的倍数   
	char* result;                                                
	size_t totalBytes = size * nobjs;                            
	size_t bytesLeft = end_free - start_free;  //内存池的剩余空间 
	
	__TRACE_DEBUG("向内存池申请%u个字节",totalBytes);
	__TRACE_DEBUG("内存池剩余%u个字节",bytesLeft);
	if(bytesLeft >= totalBytes)
	{
		//代表内存池空间够用
		result = start_free;
		start_free += totalBytes;
		return (result);
	}
	else if(bytesLeft >= size)
	{
		//表示可满足对象数在1到nobjs之间；
		nobjs = bytesLeft/size;
		result = start_free;
		totalBytes = size * nobjs; 
		start_free += totalBytes;
		return (result);
	}
	else
	{
		//表示连一个对象大小的内存都不能满足；需要从系统申请新内存；
		size_t ByteToGet = 2*totalBytes + RoundUp(heap_size >> 16);   //heap_size>>16是额外附带的

		//首先，如果有剩余，将剩余的小块内存挂在合适的自由链表下面；
		if(bytesLeft > 0)
		{
			//头插在合适的自由链表下面
			obj* volatile *myFreeList = freeList + FREELIST_INDEX(bytesLeft);
			((obj*)start_free)->freeListLink = *myFreeList;
			*myFreeList =  (obj*)start_free;
		}

		//从系统申请内存来填充内存池
		start_free = (char*)malloc(ByteToGet);
		if(start_free == 0)
		{
			//表示申请失败了，则再尝试从自由链表的后面寻找合适的内存；
			//所谓合适，即未被使用，并且足够大的内存，即至少比当前size大；
			int i = 0;
			obj* volatile *myFreeList;
			obj* p;

			for(i = size; i <= __MAX_BYTES; i += __ALIGN)
			{
				myFreeList = freeList + FREELIST_INDEX(i);
				p = *myFreeList;
				
				if(p != 0)
				{
					//说明有满足条件的内存
					*myFreeList = p->freeListLink;
					start_free = (char*)p;
					end_free = start_free + i;

					//递归的调用自己，修正nobjs和返回内存并且将剩余小块内存编入适当的FreeList；
					return (ChunkAlloc(size,nobjs));
				}
			}
			end_free = 0;
		/*  end_free一定得置为0；因为如果山穷水尽也没有找到内存，那么就会抛出异常，
		而下一次进入的时候，去算byteLeft的时候会用end_free - start_free，如果不把
		end_free置为0，会引发严重的内存错误  */

		//再次调用一级配置器，看是否还有奇迹，哈哈！！！
		start_free = (char*)MallocAlloc::Allocate(ByteToGet);
		//除非奇迹发生，要不然还是抛出异常；
		}
		
		//收尾工作
		heap_size += ByteToGet;
		end_free = start_free + ByteToGet;

		//递归的调用自己，和前面的那个作用一样
		return (ChunkAlloc(size,nobjs));
	}
}

//STL中将二级空间配置器设为默认配置器；
#ifdef __USE_MALLOC
typedef MallocAlloc Alloc;
#else
typedef DefaultAllocTemplate<0,0> Alloc;

#endif  //__USE_MALLOC