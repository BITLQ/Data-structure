#pragma once
#include "alloc.h"
#include "MyConstruct.h"

template<class T>
struct ListNode
{
	T _data;
	ListNode<T>* _prev;
	ListNode<T>* _next;

	ListNode(const T& data)
		:_data(data)
		,_prev(NULL)
		,_next(Null)
	{}
};

template<typename T,typename Ref,typename Ptr>
struct ListIterator
{
	typedef  ListIterator<T,T&,T*>     Iterator;
	typedef  ListIterator<T,Ref,Ptr>   Self;
public:
	typedef ListNode<T> Node;

	ListIterator(){}
	ListIterator(Node* node):_node(node){}
	ListIterator(const Iterator& x):_node(x._node){}

	bool operator==(const Self& x)const { return _node == x._node; }
	bool operator!=(const Self& x)const { return _node != x._node; }
	Ref operator*() const { return _node->_data;}

	Self& operator++(){ _node = _node->_next; return *this;}
	Self  operator++(int){ Node* tmp = _node; ++*this; return tmp;}
	Self& operator--(){ _node = _node->_prev; return *this}
	Self  operator--(int){ Node* tmp = _node; --*this; return tmp;}


	Node* _node;
};

template<class T,class _Alloc = Alloc>
class List
{
public:
	typedef ListIterator<T,T&,T*> Iterator;
	typedef ListIterator<T,const T&,const T*> ConstIterator;
	typedef ListNode<T> Node;

	List()
	{
		_head = BuyNode();
		_head->_next = _head;
		_head->_prev = _head;
	}
	~List()
	{
		Clean();
		PutNode(_head);
	}

	void push_front(const T& data)
	{
		Insert(begin(),data);
		//Node* cur = GreatNode(data);
		////头插
		//Node* next = _head->_next;

		//cur->_next = _head->_next;
		//cur->_prev = _head;

		//if(_head->_next != _head)
		//	_head->_next->_prev = cur;
		//else
		//	_head->_prev = cur;
		//_head->_next = cur;
	}
	void push_back(const T& data)
	{
		Insert(end(),data);
	}

	void pop_back()
	{
		Iterator tmp = end();
		erase(--tmp);
	}
	void pop_front()
	{
		erase(begin());
	}

	Iterator erase(Iterator pos)
	{
		//删除
		//pos._node->_prev->_next = pos._node->_next;
		//pos._node->_next->_prev = pos._node->_prev;

		Node* nextNode = pos._node->_next;
		Node* prevNode = pos._node->_prev;

		prevNode->_next = nextNode;
		nextNode->_prev = prevNode;

		DistoryNode(pos._node);
		return nextNode;
	}

	Iterator begin() { return _head->_next; }
	ConstIterator begin() const { return _head->_next; }
	Iterator end() { return _head;}
	ConstIterator end() const { return _head; }

	//在某个位置之前插入一个元素
	Iterator Insert(Iterator pos, const T& data)
	{
		Node* tmp = GreatNode(data);
		tmp->_next = pos._node;
		tmp->_prev = pos._node->_prev;

		pos._node->_prev->_next = tmp;
		pos._node->_prev = tmp;

		return tmp;
	}

protected:
	//主要的就是利用Buynode和 PutNode 来测试模拟实现的alloc的可用性；
	Node* BuyNode() { return (Node*)(Alloc::Allocate(sizeof(Node))); }
	void PutNode(Node* p) { Alloc::DeAllocate(p,sizeof(Node)); }

	//还需要初始化内存的函数
	Node* GreatNode(const T& x)
	{
		Node* p = BuyNode();
		try{
			construct(&p->_data,x);
		}
		catch(...)
		{
			PutNode(p);
		}
		return p;
	}

	void DistoryNode(Node* p)
	{
		//调用数据类型的析构函数
		distory(&p->_data);
		PutNode(p);    //回收内存
	}
public:
	void Clean()
	{
		Node* cur = _head->_next;

		while(cur != _head)
		{
			Node* tmp = cur;
			cur = cur->_next;
			DistoryNode(tmp);
		}
		_head->_next = _head;
		_head->_prev = _head;
	}

private:
	Node* _head;
};