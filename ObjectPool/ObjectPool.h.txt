#include<iostream>

using namespace std;

/* 内存池项目：是利用申请一大块内存，对自定义类型数据的内存分配，
达到内存的复用，提高利用率的目的；

对于反复的申请和释放内存，可以提高一定的效率；
*/

template<class T>
class ObjectPool
{
public:
	struct BlockNode
	{
		void* _memory;    // 内存块；
		BlockNode* _next; // 指向下一块内存；
		size_t _objNum;   // 内存对象的个数；

		BlockNode(size_t objNum)
			:_objNum(objNum)
			,_next(NULL)
		{
			_memory = malloc(_itemSize*objNum);
			if(_memory == NULL)
				throw -1;
		}

		~BlockNode()
		{
			free(_memory);
			_memory = NULL;
		}
	};

protected:
	size_t _countIn;  // 当前结点在用的计数
	BlockNode* _first;
	BlockNode* _last;
	size_t _maxNum;   //结点申请内存块对象个数
	static size_t _itemSize; //单个对象大小
	T* _lastDelete; // 最近释放的对象内存

public:
	ObjectPool(size_t initNum = 32,size_t maxNum = 100000)
		:_countIn(0)
		,_maxNum(maxNum)
		,_lastDelete(NULL)
	{
		_first = _last = new BlockNode(initNum);
	}

	~ObjectPool()
	{
		BlockNode* cur = _first;
		while(cur)
		{
			BlockNode* del = cur;
			cur = cur->_next;
			delete del;
		}
		_countIn = 0;
		_first = _last = NULL;

	}

	T* New()
	{
		// 1.优先使用以前释放的空间
		// 2.从内存块中申请空间
		// 3.申请新的结点

		if(_lastDelete)
		{
			//需要将上一次的_lastDelete保存起来，保证下一次的寻找；
			T* obj = _lastDelete;
			//强制类型转换，从上一次释放的空间中读取一个指针的大小；
			_lastDelete = *((T**)_lastDelete);
			//返回定位new调用的构造函数好的空间；
			return new(obj) T;
		}
		
		if(_countIn >= _last->_objNum)
		{
			size_t objNum = _last->_objNum * 2;
			if(objNum > _maxNum)
				objNum = _maxNum;
			_last->_next = new BlockNode(objNum);
			_last = _last->_next;
			_countIn = 0;
		}
	
			T* obj = (T*)((char*)(_last->_memory)+_countIn*_itemSize);
			_countIn += 1;
			return new(obj) T;
	}

	//回收空间
	void Delete(T* obj)
	{
		//回收的重点在于塞进去_lastDelete;
		T* tmp = _lastDelete;
		_lastDelete = obj;
		*(T**)obj = tmp;
	}

	static size_t GetSize()
	{
		//每块内存的大小至少不小于一个指针
		if(sizeof(T) > sizeof(void*))
			return sizeof(T);
		else
			return sizeof(void*);
	}
};

template<class T>
size_t ObjectPool<T>::_itemSize = ObjectPool<T>::GetSize();