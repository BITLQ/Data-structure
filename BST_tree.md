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
		Node* parent = NULL;
		return _RemoveR(_root, key);
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
				Node* del = cur;
				if(cur->_left && cur->_right )
				{
					//删除左右孩子都不为NULL的情况
					//可以选择左树的最右结点或者右树的最左结点交换删除
					Node* minright = cur->_right ;

					while(minright->_left )
						minright = minright ->_left ;

					cur->_key = minright->_key ;
					del = minright ;
		
				}
				else if(cur->_left == NULL)
				{
					//有一种情况是，删除根节点时，根节点只有一个子树，即按照左子树为NULL
					//或者右子树为NULL的情况处理，此时根节点的parent是NULL，所以需要单独处理！

					if(parent == NULL)
					{
						_root = _root->_right ;
					}
					else
					{
						if(parent->_key < key)
						parent->_right = cur->_right ;
					    else
						parent->_left = cur->_right;
					}
				}
				else if(cur->_right ==NULL)
				{
					if(parent == NULL)
					{
						_root = cur->_left ;
					}
					else
					{
						if(parent->_key < key)
							parent->_right = cur->_left ;
						else
							parent->_left = cur->_left ;
					}
					
				}

				delete del;
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
		if(root->_key < key)
			return _RemoveR(root->_right, key);
		else if(root->_key > key)
			return _RemoveR(root->_left, key);
		else
		{
			Node* del = root;
			if(root->_left == NULL)
				root = root->_right ;
			else if(root->_right == NULL)
				root = root->_left ;
			else
			{
				Node* minright = root; //寻找右树的最左结点进行key值的交换
				minright =  root->_right ;

				while(minright->_left)
					minright = minright->_left ;
				
				root->_key = minright->_key ;
				del = minright;
			}

			delete del;
			return true;
		}
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

	t.Remove (0);
	t.Remove (1);
	t.Remove (2);
	t.Remove (3);
	t.Remove (4);
	t.Remove (5);
	t.Remove (6);
	t.Remove (7);
	t.Remove (8);
	t.Remove (9);
	t.Inorder ();
	system("pause");
	return 0;
}
