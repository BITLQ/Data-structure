#pragma once
#define  _CRT_SECURE_NO_WARNINGS 1

#include<cstdio>
#include<windows.h>
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
		
		//获取哈夫曼编码
		string code;
		_GetHuffmanCode(root,code);

        fseek(fread,0,SEEK_SET);
		string write(filename);
		write = write + ".Compress";

		//利用string的成员函数将write转为char*类型的字符串
		FILE* fwrite = fopen( write.c_str() , "wb");
		
	    ch = fgetc(fread);
		unsigned char data = 0;//压缩的数据以二进制的形式存储在文件中；
		int pos = 7; //控制bit位的移动次数

		while(ch != EOF)
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
		unsigned int index = write.rfind ('.',write.size ());
		write = write.substr (0,index);

		//根据压缩文件对哈夫曼树进行遍历，写入新的解压缩文件；

		//读配置文件
		CharInfo RInfo[256];
		ReadConfig((write + ".config").c_str(), RInfo);

		write = write + ".jpg"; 

		FILE* fwrite = fopen(write.c_str(),"wb");

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
		//string line;
		//char buff[128];

		for(int i = 0; i < 256; i++)
		{
			if(_Info[i]._count)
			{
			fputc(_Info[i]._ch , fwrite);
			fputc(',',fwrite);
			//将_count以十进制字符的形式存入字符数组arr;
			char arr[126];
			_itoa(_Info[i]._count, arr, 10);
			fputs(arr, fwrite);
			fputc(',', fwrite);
			fputs(_Info[i]._code.c_str (), fwrite);
			fputc('\n',fwrite);
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


//测试图片，mp3，和大文件的部分
int main()
{
	FileCompress f;

	int start =  GetTickCount();
	//f.Compress("test.exe");
	int end = GetTickCount();
	//cout<<"压缩文件耗时: "<<end-start<<"毫秒"<<endl;

	//start = end;
	//f.UnCompress ("test.exe.Compress");
	//end = GetTickCount();
	//cout<<"解压缩文件耗时: "<<end-start<<"毫秒"<<endl;

	/*f.Compress("music.mp3");
	cout<<"压缩文件耗时: "<<end-start<<"毫秒"<<endl;
	f.UnCompress ("music.mp3.Compress");
	end = GetTickCount();
	cout<<"解压缩文件耗时: "<<end-start<<"毫秒"<<endl;*/

   // f.Compress("picture.jpg");
	//cout<<"压缩文件耗时: "<<end-start<<"毫秒"<<endl;
	f.UnCompress ("picture.jpg.Compress");
	end = GetTickCount();
	cout<<"解压缩文件耗时: "<<end-start<<"毫秒"<<endl;

	system("pause");
	return 0;
}


