# Data-structure
data   structure

#include<iostream>
#include<cassert>

using namespace std;

template<class K>
struct SearchBinaryTreeNode
{
	SearchBinaryTreeNode<K>* _left;
	SearchBinaryTreeNode<K>* _right;
	K _key;

	SearchBinaryTreeNode(const K& key)
		:_left(NULL)
		,_right(NULL)
		,_key(key)
	{}
};

template<class K>
class SearchBinaryTree
{
public:
	typedef SearchBinaryTreeNode<K>  Node;

	SearchBinaryTree():_root(NULL){}
	SearchBinaryTree( K* arr, size_t size)
	{
		assert(arr);
		_root = NULL;
		for(size_t i = 0; i < size; i++)
		{
			//InsertR(_root,arr[i]);
			Insert(arr[i]);
		}
	}

	//增  递归
	bool InsertR(Node*& root, K key)
	{
		if(!root)
		{
			root = new Node(key);
			return true;
		}
		else if(root->_key == key)
			return false;
		else if(root->_key > key)
		{
			return Insert(root->_left ,key);
		}
		
		return Insert(root->_right ,key);
	}

	//增  非递归

	bool Insert(const K& key)
	{
		if(_root == NULL)
		{
			_root = new Node(key);
			return true;
		}
		Node* cur = _root;
		Node* parent =  NULL;
		int flag = -1;

		while(cur)
		{
			parent = cur;

			if(cur->_key == key)
				return false;
			else if(cur->_key < key)
			{
				cur = cur->_right ;
				flag = 1;
			}
			else if(cur->_key > key)
			{
				cur = cur->_left ;
				flag = 0;
			}
		}
		cur = new Node(key);
		if(flag == 1)
			parent->_right = cur;
		else if(flag == 0)
			parent->_left = cur;

		return true;
	}

	//删   递归
	bool RemoveR(const K& key)
	{
		Node* root = _root;
		Node* parent = NULL;
		return _RemoveR(root, key);
	}

	//删   非递归
	bool  Remove(const K& key)
	{
		//左为NULL
		//右为NULL
		//左右都不为NULL
		Node* cur = _root;
		Node* parent = NULL;
		while(cur)
		{
			if(cur->_key == key)
			{
				if(cur->_left && cur->_right )
				{
					//删除左右孩子都不为NULL的情况
					//可以选择左树的最右结点或者右树的最左结点交换删除
					Node* tmp = cur;
					parent = cur;
					cur = cur->_right ;

					if(cur->_left == NULL)
					{
						tmp->_key = cur->_key ;
						parent->_right = cur->_right ;
					}
					else
					{
						while(cur->_left)
						{
							parent = cur;
							cur = cur->_left ;
						}
					//交换key值
						tmp->_key = cur->_key ;
						parent->_left = cur->_right ;
					}
		
				}
				else if(cur->_left == NULL)
				{
					if(parent->_key < key)
						parent->_right = cur->_right ;
					else
						parent->_left = cur->_right;
				}
				else if(cur->_right ==NULL)
				{
					if(parent->_key < key)
						parent->_right = cur->_left ;
					else
						parent->_left = cur->_left ;
				}

				delete cur;
				return true;
			}

			else if(cur->_key < key)
			{
				parent = cur;
				cur = cur->_right ;
			}
			else if(cur->_key > key)
			{
				parent = cur;
				cur = cur->_left ;
			}
		}

		return false;
	}


	//查   非递归
	bool Find(const K& key)
	{
		Node* cur = _root;
		while(cur)
		{
			if(cur->_key == key)
				return true;
			else if(cur->_key < key)
				cur = cur->_right ;
			else
				cur = cur->_left ;
		}
		return false;
	}

	//查找  递归
	bool FindR(const K& key)
	{
		Node* root = _root;

		return _FindR(root, key);
	}

	//中序遍历
	void Inorder()
	{
		_Inorder(_root);
		cout<<endl;
	}



protected:

	//删除   递归
	bool _RemoveR(Node*& root, const K& key)
	{
		if(root == NULL)
			return false;

		if(root->_key == key)
		{
			Node* del = root;
			if(root->_left == NULL)
			{
				//左子树为NULL；
				root = root->_right ;
				delete del;
			}
			else if(root->_right == NULL)
			{
				//右子树为空
				root = root->_left ;
				delete del;
			}
			else
			{
				//左右子树都不为空，交换右子树的最左结点，删除右子树最左结点；
				Node* tmp = root;
				while(tmp->_left)
				{
					tmp = tmp->_left ;
				}

				root->_key = tmp->_key ;

				_RemoveR(tmp->_right , tmp->_key );
			}
			return true;
		}

		else if(root->_key > key)
		{
			return _RemoveR(root->_left , key);
		}
		else
			return _RemoveR(root->_right ,key);
	}

	//查找  递归
	bool _FindR(Node* root, const K& key)
	{
		if(root == NULL)
			return false;
		if(root->_key > key)
			_FindR(root->_right ,key);
		else if(root->_key < key)
			_FindR(root->_left ,key);
		else
			return true;
	}

	void _Inorder(Node* root)
	{
		if(root == NULL)
			return;
		
		_Inorder(root->_left );
		cout<<root->_key <<" ";
		_Inorder(root->_right );
	}

private:
	Node* _root;
};

int main()
{
	int arr[10] = {5,2,4,6,9,0,3,7,8,1};
	SearchBinaryTree<int> t(arr,10);

	t.Inorder ();
	cout<<(t.RemoveR (4))<<endl;
	t.Inorder ();
	cout<<t.FindR (5)<<endl;
	t.Inorder ();
	system("pause");
	return 0;
}
