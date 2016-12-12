/* ģ��ʵ��STL�Ŀռ�������
������һ����������������������

//ÿһ�������������������������allocate��deallocate

*/
//���ȣ�һ����������

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
// ���ڵ���׷�ݵ�trace log  
inline static void __trace_debug (const char* function,  
                                const char * filename, int line , char* format , ...)  
{  
    // ��ȡ�����ļ�  
#ifdef __DEBUG__  
    // ������ú�������Ϣ  
    fprintf(stdout , "��%s:%d��%s" , GetFileName (filename). c_str(), line , function);  
      
    fprintf(fOut, "��%s:%d��%s" , GetFileName (filename). c_str(), line , function);  
    // ����û����trace��Ϣ  
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



/******   һ��������   ******/
//���Ƕ�malloc��free�ķ�װ

template<int inst>  //Ԥ���������Դ�����
class MallocAllocTemplate
{
	static void* OomMalloc(size_t n);
	static void (* MallocAllocOomHandler)();

public:
	//allocate����
	static void* Allocate(size_t n)
	{
		__TRACE_DEBUG("��һ������������%u���ֽ�",n);
		void* ret = malloc(n);
		if(ret == NULL)
			ret = OomMalloc(n);
		return ret;
	}

	//deallocate����
	static void Deallocate(void* p,size_t n)
	{
		__TRACE_DEBUG("һ������������%u���ֽ�",n);
		free(p);
		p = NULL;
	}

	//�����set_new_handler,ͨ���˺����ƶ��Լ���out_of_memory����
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
		{throw bad_alloc();}              //������δ�����׳��쳣��

		(*MyMallocHandler)();             //�����������壬����ã�
		ret = malloc(n);                  //��������malloc��
		if(ret) 
			return ret;
	}
}


//mallocʧ��ʱ���õ�ʧ�ܴ������еľ��
//ģ��C++��set_new_handler���ο�STLԴ������ && effective C++��
//namespace std
//{
//	typedef void (*new_handler)();
//	new_handler set_new_handler(new_handler p)throw();//���׳��쳣
//}
//STL��û�жԴ˾�����ж��壬�����ͻ����壬�����ڴ�����޷�����ʱ���ã�
template<int inst>
void (* MallocAllocTemplate<inst>::MallocAllocOomHandler)() = 0;

//ע�⣬����ֱ�ӽ�intsָ��Ϊ0
typedef MallocAllocTemplate<0> MallocAlloc;


/*****   ����������  *****/

//�ռ������������ó�����Ϊ�˼���Ƶ���������ڴ����������ڴ���Ƭ�����ϵͳ������
//��һ�����������ӣ������ڴ�غ�������������ڴ棻

/*(����STLԴ������)����������������������������㹻�󣬳���128 Byteʱ�����ƽ���һ��������
����������С��128 Byteʱ�������ڴ�ع���ÿ������һ����ڴ棬��ά����Ӧ�����������´�
������ͬ��С���ڴ����󣬾�ֱ�Ӵ�free_list��ȡ����ͷɾ��������ͻ����ͻ�С�����飬����������
���յ�free_list��Ϊ�˷������SGI���������������κ�С��������ڴ������ϵ���8�ı�����

���ڴ����ϵͳ����������ڴ涼���ڳ���������ɺ�Ż���ȥ�ģ�����һ�ֿռ任ʱ���������
��Ϊ��ά����������ÿ���ڵ�Ҳ������Ҫ�����ָ�룬�������ú��ʼ�Ǹ��̶������С���ڴ��ģ��
ʵ�ֵķ������ƣ�����SGI���������ϣ�char[1]��Ϊ�˷���ͻ��˲鿴ʹ�������

union obj{
      union obj* free_list_link;
	  char client_data[1];
}
*/

//���Դ�SGIʹ��ö�ٶ��峣����û����define����define���ŵ��ȱ��
 enum {__ALIGN = 8};      //С������ϵ��߽磬����СҲ��8����ָ���С��ƽ̨�޹��ԣ�
 enum {__MAX_BYTES = 128};  //С���������
 enum {__NFREELISTS = __MAX_BYTES/__ALIGN};  //free_list�ĳ���

 //ע���һ���������̰߳�ȫ�ģ������ۣ�
 template<bool threads,int inst>
 class DefaultAllocTemplate
 {
	 //ά������������Ҫ��ָ��
	 union obj
	 {
		 union obj* freeListLink;
		 char clientData[1];
	 };
	 //����Ϊ16��������
	 static obj* volatile freeList[__NFREELISTS];
	 //ȷ�����������±�ĺ���������ʹ�õڼ������������µ��ڴ棻
	 static size_t FREELIST_INDEX(size_t bytes)
	 {
		 //����һ����΢���Ч�ʵ�д����������һ��ģ�������ǽ����˿ɶ���
		 //return bytes%__ALIGN == 0 ? (bytes/__ALIGN) - 1:bytes/__ALIGN;
		 return (((bytes) + __ALIGN - 1)/__ALIGN - 1);
	 }

	 //������8�ı���
	 static size_t RoundUp(size_t bytes)
	 {
		 return (((bytes) + __ALIGN-1) & ~(__ALIGN - 1));
	 }

	 //�ڴ�ؿ�ʼ�ͽ���:ע��������char*,��̬��Ա���������������ʼ��
	 static char* start_free;    //�ڴ����ʼλ�ã�ֻ��ChunkAlloc�б仯
	 static char* end_free;      //�ڴ�ؽ���λ�ã�ֻ��ChunkAlloc�б仯
	 static size_t heap_size;    //������Ǵӳ���ʼ��ĿǰΪֹ�ڴ����ϵͳ��������ڴ��С

	 //Refill  ���Դ��ڴ������ռ����free_list
	 static void* Refill(size_t n);

	 //���ڴ������
	 static char* ChunkAlloc(size_t n,size_t& nobjs);

 public:
	 //�����ӿ�
	 static void* Allocate(size_t n);
	 static void DeAllocate(void* p, size_t /* n */);
 };

 //��ʼ��
 template<bool threads,int inst>
 typename DefaultAllocTemplate<threads,inst>::obj* volatile
	 DefaultAllocTemplate<threads,inst>::freeList[__NFREELISTS] = {0};

 template<bool threads,int inst>
 size_t DefaultAllocTemplate<threads,inst>::heap_size = 0;
 
 template<bool threads,int inst>
 char* DefaultAllocTemplate<threads,inst>::start_free = 0;

 template<bool threads,int inst>
 char* DefaultAllocTemplate<threads,inst>::end_free = 0;


 //��allocate(�ռ����ú�����ʼ)
template<bool threads,int inst>
void* DefaultAllocTemplate<threads,inst>::Allocate(size_t n)
{
	__TRACE_DEBUG("���������������%u���ֽ�",n);
	obj* volatile *myFreeList; //��������ĵ�ַ��ע�⣺����ָ��
	obj* result;               //��Ϊ���صĽ��      
	
	//�����ж�����Ĵ�С������128ֱ��ȥһ��������
	if(n > 128){return (MallocAlloc::Allocate(n));}

	//С��128������������������
	myFreeList = freeList + FREELIST_INDEX(n);  //����8�ı����Ĵ�С���ҵ���Ӧ�±�

	result = *myFreeList;   //����ͷɾ�ķ�����С�����
	if(result == 0)
	{
		//��Ӧ���������������û�п����ڴ棬����ڴ���������
		void* ret = Refill(RoundUp(n));
		return ret;
	}
	//ͷɾ
	*myFreeList = result->freeListLink;

	return (result);
}

//deallocate ����
template<bool threads,int inst>
void DefaultAllocTemplate<threads,inst>::DeAllocate(void* p, size_t n )
{
	//���ڴ棬ͬ��������
	obj* q = (obj*)p;
	obj* volatile *myFreeList;

	//����128ֱ�ӵ���һ��������
	if(n > 128){ MallocAlloc::Deallocate(p,n); return;}

	__TRACE_DEBUG("��������������%u���ֽ�",n);
	//���򣬽��ڴ�ֱ���������������ϣ���Ҳ�ǿռ��������ľ���֮����
	myFreeList = freeList + FREELIST_INDEX(n);

	//ͷ��,��������
	q->freeListLink = *myFreeList;
	*myFreeList = q;
}

//ʵ��refill,��������������
template<bool threads,int inst>
void* DefaultAllocTemplate<threads,inst>::Refill(size_t n)
{
	size_t nobjs = 20;  //Ĭ�ϵ�һ�����ڴ������20������Ĵ�С���ܲ���һ��һ��һ��������ɣ�
	
	//����chunkallock���������ڴ�����룻
	char* chunk = ChunkAlloc(n,nobjs);  //����nobjs�����ô��ݣ�
	
	obj* volatile *myFreeList;  //���ﶼ������
	obj* result;
	obj* curObj,*nextObj;        //����ǰ����һ��������ῴ���������ã�

	//���ֻ������һ������Ĵ�С
	if(nobjs == 1) return (chunk);

	//�������ǾͰ�ʣ��Ķ��������������ϣ�
	//��Ϊʲô����ʣ�࣬����nobjs = 20 ���Ҵ����õ����壩
	myFreeList = freeList + FREELIST_INDEX(n);
	//��һ�鷵��
	result = (obj*)chunk;

	//�Ȱ�ʣ���ȫ��������������
	*myFreeList = nextObj = (obj*)(chunk + n);

	//���ڴ����objָ��Ĳ���
	int i;
	for(i = 1;;++i)
	{
		//����һ��ѭ��ֲ��Ĺ��̣�
		curObj = nextObj;
		nextObj = (obj*)(((char*)nextObj) + n);
		if(nobjs - 1 == i)   //ѭ�������������ݣ�ע�⣬ǰ���Ѿ���һ�����result;
		{
			//���һ����ΪNULL
			curObj->freeListLink = 0;
			break;
		}
		else
		{
			//��ǰ�ռ�����һ��ռ�ĵ�ַ
			curObj->freeListLink = nextObj;
		}
	}
	return (result);
}


/********   ������������ڴ�صĲ��� ******/
//ʵ��chunk_alloc
template<bool threads,int inst>
char* DefaultAllocTemplate<threads,inst>::ChunkAlloc(size_t size,size_t& nobjs)
{
	
	//ע�⣬nobjs���������ã�size�ڵ��õ�ʱ���Ѿ�����Ϊ8�ı���   
	char* result;                                                
	size_t totalBytes = size * nobjs;                            
	size_t bytesLeft = end_free - start_free;  //�ڴ�ص�ʣ��ռ� 
	
	__TRACE_DEBUG("���ڴ������%u���ֽ�",totalBytes);
	__TRACE_DEBUG("�ڴ��ʣ��%u���ֽ�",bytesLeft);
	if(bytesLeft >= totalBytes)
	{
		//�����ڴ�ؿռ乻��
		result = start_free;
		start_free += totalBytes;
		return (result);
	}
	else if(bytesLeft >= size)
	{
		//��ʾ�������������1��nobjs֮�䣻
		nobjs = bytesLeft/size;
		result = start_free;
		totalBytes = size * nobjs; 
		start_free += totalBytes;
		return (result);
	}
	else
	{
		//��ʾ��һ�������С���ڴ涼�������㣻��Ҫ��ϵͳ�������ڴ棻
		size_t ByteToGet = 2*totalBytes + RoundUp(heap_size >> 16);   //heap_size>>16�Ƕ��⸽����

		//���ȣ������ʣ�࣬��ʣ���С���ڴ���ں��ʵ������������棻
		if(bytesLeft > 0)
		{
			//ͷ���ں��ʵ�������������
			obj* volatile *myFreeList = freeList + FREELIST_INDEX(bytesLeft);
			((obj*)start_free)->freeListLink = *myFreeList;
			*myFreeList =  (obj*)start_free;
		}

		//��ϵͳ�����ڴ�������ڴ��
		start_free = (char*)malloc(ByteToGet);
		if(start_free == 0)
		{
			//��ʾ����ʧ���ˣ����ٳ��Դ���������ĺ���Ѱ�Һ��ʵ��ڴ棻
			//��ν���ʣ���δ��ʹ�ã������㹻����ڴ棬�����ٱȵ�ǰsize��
			int i = 0;
			obj* volatile *myFreeList;
			obj* p;

			for(i = size; i <= __MAX_BYTES; i += __ALIGN)
			{
				myFreeList = freeList + FREELIST_INDEX(i);
				p = *myFreeList;
				
				if(p != 0)
				{
					//˵���������������ڴ�
					*myFreeList = p->freeListLink;
					start_free = (char*)p;
					end_free = start_free + i;

					//�ݹ�ĵ����Լ�������nobjs�ͷ����ڴ沢�ҽ�ʣ��С���ڴ�����ʵ���FreeList��
					return (ChunkAlloc(size,nobjs));
				}
			}
			end_free = 0;
		/*  end_freeһ������Ϊ0����Ϊ���ɽ��ˮ��Ҳû���ҵ��ڴ棬��ô�ͻ��׳��쳣��
		����һ�ν����ʱ��ȥ��byteLeft��ʱ�����end_free - start_free���������
		end_free��Ϊ0�����������ص��ڴ����  */

		//�ٴε���һ�������������Ƿ����漣������������
		start_free = (char*)MallocAlloc::Allocate(ByteToGet);
		//�����漣������Ҫ��Ȼ�����׳��쳣��
		}
		
		//��β����
		heap_size += ByteToGet;
		end_free = start_free + ByteToGet;

		//�ݹ�ĵ����Լ�����ǰ����Ǹ�����һ��
		return (ChunkAlloc(size,nobjs));
	}
}

//STL�н������ռ���������ΪĬ����������
#ifdef __USE_MALLOC
typedef MallocAlloc Alloc;
#else
typedef DefaultAllocTemplate<0,0> Alloc;

#endif  //__USE_MALLOC