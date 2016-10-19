# Data-structure
data   structure

//HuffmanTree.h

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

// FileCompress.cpp

#pragma once
#define  _CRT_SECURE_NO_WARNINGS 1

#include <direct.h>
#include<string>
#include<iostream>
#include "Huffman.h"
typedef long long TypeData;

using namespace std;

struct CharInfo
{
	//并不一定256个字符都有
	unsigned char _ch; //字符
	TypeData _count;   //字符出现的次数
	string _code;      //编码

	CharInfo(const TypeData& count = 0):_count(count){}
	CharInfo(const char ch):_ch(ch){}

	CharInfo operator+(const CharInfo& x)
	{
		//return _count + x._count ;
	    return CharInfo(_count + x._count);
	}

	bool operator<(const CharInfo& x)
	{
		return _count < x._count ;
	}

	bool operator!=(const CharInfo& x)
	{
		return _count != x._count ;
	}

};

class FileCompress
{
private:
	CharInfo _Info[256]; //ASCII表中的字符表示从 0~255；

public:
	typedef HuffmanTreeNode<CharInfo> Node;


	//构建哈夫曼树
	//HuffmanTree<CharInfo> huffmantree(char* _Info,int ,CharInfo CharInfo());
	//文件压缩模块
	void Compress(const char* filename)
	{
		FILE* fread = fopen(filename,"rb");
		if(fread == NULL)
		{
			cout<<"the file open fail"<<endl;
			exit(0);
		}

		//采用哈希表的方式统计字符出现次数
		for(int i = 0; i < 256; i++)
			_Info[i]._ch = i;

		//统计文件中字符出现的次数
	    int ch = fgetc(fread);
		while(ch != EOF)
		{
			_Info[ch]._count ++;
			ch = fgetc(fread);
		}

		//构建哈夫曼树
		HuffmanTree<CharInfo> huffmantree(_Info,256,CharInfo());

		Node* root = huffmantree.GetRoot ();
		string code;
		
		//获取哈夫曼编码
		_GetHuffmanCode(root,code);
        fseek(fread,0,SEEK_SET);
		
		string write(filename);

		write = write + ".Compress";
		//利用string的成员函数将write转为char*类型的字符串
		FILE* fwrite = fopen( write.c_str() , "wb");
		
	    ch = fgetc(fread);
		unsigned char data = 0;//压缩的数据以二进制的形式存储在文件中；
		int pos = 7; //控制bit位的移动次数

		while(!feof(fread))
		{
			const char* ptr = _Info[ch]._code.c_str();
			//遍历保存的编码结点；
			while(*ptr)
			{
				if(pos >= 0)
				{
					data = data | ((*ptr - '0')<<pos);
					pos --;
				}
				if(pos < 0)
				{
					fputc(data,fwrite);
					pos = 7;
					data = 0;
				}
				ptr++;
			}
			ch = fgetc(fread);
		}
		//最后一个字节的数据不管写没写满都放进去
		fputc(data,fwrite);

		//写配置文件
		WriteConfig(filename);

		fclose(fread);
		fclose(fwrite);
		cout<<"压缩成功"<<endl;
	}

	//解压缩模块
	void UnCompress(const char* filename)
	{
		assert(filename);

		//解压缩需要遍历哈夫曼树，重建哈夫曼树
		//怎样重建哈夫曼树？
		//第一种，在已有原文件的情况下进行解压缩
       
		
		//开始解压缩
		FILE* fread = fopen(filename,"rb");
		unsigned char ch = fgetc(fread);

		string write(filename);
		TypeData index = write.rfind ('.',write.size ());
		write = write.substr (0,index);
		FILE* fwrite = fopen(write.c_str(),"wb");
		//根据压缩文件对哈夫曼树进行遍历，写入新的解压缩文件；

		//读配置文件
		CharInfo RInfo[256];
		ReadConfig((write + ".config").c_str(), RInfo);

		//需要获取哈夫曼树的根节点，因为根节点的权值就是总共字数的个数
		//HuffmanTree<CharInfo> h(_Info,256,CharInfo());
		HuffmanTree<CharInfo> h(RInfo, 256, CharInfo());
		HuffmanTreeNode<CharInfo>* root = h.GetRoot ();
		if(root == NULL)
			return;
		HuffmanTreeNode<CharInfo>* cur = root;
		TypeData charcount = root->_weight ._count ;

		//用字符的总数来控制循环条件
		
		int pos = 8;
		while(charcount)
		{	
			--pos;
			unsigned char value = 1;
			//需要对压缩文件一个字节一个字节的访问；
			if(ch & (value << pos))
			{
				cur = cur->_right ;
			}
			else
			{
				cur = cur->_left ;
			}
			if(cur->_left == NULL && cur->_right == NULL )
			{
				//读到叶子结点则把对应的字符写入解压缩文件
				fputc(cur->_weight._ch, fwrite);
				//每次将cur置为根节点，因为每次是从根节点开始遍历；
				cur = root;
				if(--charcount == 0)
					break;
			}
			if(pos == 0)
			{
				pos = 8;
				ch = fgetc(fread);
			}
			
		}
		fclose(fread);
		fclose(fwrite);
	}
protected:
	//后序遍历哈夫曼树
	//因为我们只需要访问叶子结点；
	void _GetHuffmanCode(Node* root,string code)
	{
		if(root == NULL)
			return;
		_GetHuffmanCode(root->_left ,code + '0');
		_GetHuffmanCode(root->_right,code + '1');

		if(root->_left == NULL && root->_right == NULL)
		{
			_Info[root->_weight ._ch ]._code = code;
		}
	}

	//写配置文件
	void WriteConfig(const char* filename)
	{
		//压缩的文件信息保存在.config后缀的文件中
		string write(filename);
		write = write + ".config";
		FILE* fwrite = fopen(write.c_str (), "wb");

		
		//需要保存的是字符的出现次数,哈夫曼编码
		string line;
		char buff[128];
		for(int i = 0; i < 256; i++)
		{
			if(_Info[i]._count)
			{
			fputc(_Info[i]._ch , fwrite);
			fputc(',',fwrite);
			//将_count以十进制的形式存入字符数组arr;
			char arr[126];
			_itoa(_Info[i]._count, arr, 10);
			fputs(arr, fwrite);
			fputc(',', fwrite);
			fputs(_Info[i]._code.c_str (), fwrite);
			fputc('\n',fwrite);

				/*line += _Info[i]._ch;
				line += ',';
				line += itoa(_Info[i]._count, buff, 10);
				line += ',';
				line += _Info[i]._code;
				line += '\n';
				fputs(line.c_str(), fwrite);
				line.clear();*/
			}
		}
		//关闭文件其实就是保存！
		fclose(fwrite);
	}

	//读配置文件
	void ReadConfig(const char* configfilename, CharInfo* RInfo)
	{
		FILE* fread = fopen(configfilename, "rb");
		if(fread == NULL)
		{
			cout<<"read file fault"<<endl;
			exit(0);
		}

		int ch = fgetc(fread);
		while(ch != EOF)
		{
			//字符，数量，编码
			RInfo[ch]._ch = ch;
			unsigned char tmp = ch;

			//','的ASCII值为44
			ch = fgetc(fread);
			ch = fgetc(fread);

			string arr;
			while(ch != ',')
			{	
				arr.push_back(ch);
				
				ch = fgetc(fread);
			}

			RInfo[tmp]._count = atoi(arr.c_str());


			ch = fgetc(fread);
			while(ch != '\n')
			{
				RInfo[tmp]._code.push_back (ch);

				ch = fgetc(fread);
			}

			ch = fgetc(fread);
		}
	}

};


int main()
{
	/*if(_mkdir("\\testtmp")==0)
		cout<<"创建目录成功"<<endl;*/
	FileCompress f;
	f.Compress("test.txt");
	f.UnCompress ("test.txt.Compress");

	//f.Compress ("test.jpg");
	//f.UnCompress ("test.jpg.Compress");

	/*f.Compress ("test.exe");
	f.UnCompress ("test.exe.Compress");*/
	cout<<"Ok"<<endl;
	system("pause");
	return 0;
}
