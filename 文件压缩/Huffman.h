#include<iostream>
#include<cassert>
#include<vector>
using namespace std;

//哈夫曼编码的过程
//给出数据的权值，根据权值构造哈夫曼树
//权值即统计文件中字符出现的次数

//构造哈夫曼树的过程
//利用堆的特性；

//写一个堆类
template<class T>
struct Max
{
	bool operator()(const T& l,const T& r)
	{
		return l > r;
	}
};

template<class T>
struct Less
{
	bool operator()(const T& l, const T& r)
	{
		return l < r;
	}
};

template<class T>
struct Less<T*>
{
	bool operator()(T* node1,const T* node2)
	{
		return (node1->_weight) < (node2->_weight);
	}
};

template<class T, class Campare = Less<T>>
class Heap
{
public:
	Heap(){}
	Heap(T* arr, const int size)
	{
		assert(arr);
		for(size_t i = 0; i < size; i++)
			_arr[i] = arr[i];
		
		//建堆
		for(int i = (size-2)/2; i >= 0; i-- )
		{
			_AdJustDown(i, size);
		}
	}
	
	//push

	void Push(const T& data)
	{
		_arr.push_back(data);

		_AdJustUp(_arr.size ());
	}

	//pop
	void Pop()
	{
		if(!_arr.empty ())
		{
			swap(_arr[0],_arr[_arr.size () - 1]);
			_arr.pop_back ();

		    _AdJustDown(0, _arr.size ());

		}
	}

	//size
	int Size()
	{
		return _arr.size ();
	}

	//top
	T& Top()
	{
		if(!_arr.empty ())
		    return _arr[0];
	}

protected :
	void _AdJustDown(int parent, int size)
	{
		//assert(_arr);
		int child = parent*2 + 1;

		while(child < size)
		{
			if(child + 1 < size && Campare()(_arr[child+1],_arr[child]))
				++child;
			if(Campare()(_arr[child],_arr[parent]))
			{
				swap(_arr[child],_arr[parent]);
				parent = child;
				child = parent*2 + 1;
			}
			else
				break;
		}
	}

	void _AdJustUp(int size)
	{
		int child = size - 1;
		

		while(child)
		{
			int parent = (child - 1)/2;
			if(Campare()(_arr[child],_arr[parent]))
			{
				swap(_arr[child],_arr[parent]);
			    child = parent;
			}
			else
				break;
		}
	}

private:
	vector<T> _arr;
};


//先构造出哈夫曼树的结点结构体
//哈夫曼树的根节点代表所有字符出现的次数；

template<class T>
struct HuffmanTreeNode
{
	T _weight;
	HuffmanTreeNode<T>* _left;
	HuffmanTreeNode<T>* _right;

	HuffmanTreeNode(const T& data)
		:_weight(data)
		,_left(NULL)
		,_right(NULL)
	{}
};


template<class T>
class HuffmanTree
{
	typedef HuffmanTreeNode<T> Node;
public:
	HuffmanTree():_root(0){}
	HuffmanTree(T* arr, int size, const T& Invalid)
	{
		_root = GreatHuffman(arr, size,Invalid);
	}

	Node* GetRoot()
	{
		return _root;
	}

protected:
	Node* GreatHuffman(T* arr,int size, const T& invalid)
	{
		assert(arr);
		Heap<Node*, Less<Node*>> minheap;
		//建小堆
		for(int i = 0; i < size; i++)
		{
			if(arr[i] != invalid)
			{
				Node* tmp = new Node(arr[i]);
				minheap.Push (tmp);
			}
		}

		//构造哈夫曼树
		if(minheap.Size())
		{
			while(minheap.Size() > 1)
			{
				Node* left = minheap.Top();
				minheap.Pop ();
				Node* right = minheap .Top ();
				minheap .Pop ();
				
				Node *newNode = new Node(left->_weight + right->_weight );
				
				newNode ->_left = left;
				newNode ->_right = right;
				
				minheap .Push (newNode);
			}
			return minheap.Top ();
		}
		else
			return NULL;
	}

private:
	Node* _root;
};