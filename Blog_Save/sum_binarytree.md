**���ڶ������������������ܽ�**

 1. �����������Զ�������ľ��룻
 2. ��ǰ���������������ؽ���������
 3. �ж�һ�����Ƿ�����ȫ��������
 4.  ������������ڵ������������ȣ�
 5.  ������������ת����һ�������˫������Ҫ���ܴ����κ��µĽ�㣬ֻ�ܵ������н��ָ���ָ��


----------

> **����ϸ�Ķ������ע��**������


----------


**<һ> �����������Զ�������ľ���**

��������һƪ�������Ѿ���������ϸ��ʵ�֣����������������ӣ�

[ �����������Զ�������ľ���](http://blog.csdn.net/bitboss/article/details/53310234)


----------

**<��>��ǰ���������������ؽ�������**

**����**������һ�������������Ŀ�����Զ������ı�����ʽ�Ƿ�����̵��˽⣬ͬʱҲ����Զ������Ĺ������˽⣬ͨ������һ�����й����������ķ�ʽ������ǰ��������ҽ�ϷǷ�ֵ�ķ�ʽ���й���������ڵ�һ���ʵ�����Ѿ��������֣�

Ŀǰ�������˽�Ķ������ı�����ʽ�����֣�ǰ���У��󣬲��������
����ǰ�������������ȱ�������������ǹ�����ȱ�����������ڵ��������õ�������Ȼ���������ֻ����չ��

�����������ǰ������������������������������������������Ϊ����������

**��ǰ�����У�1 2 3 4 5 6 - ��������:3 2 4 1 6 5��**

![����дͼƬ����](http://img.blog.csdn.net/20161201164327025)

�����Ǹ���ǰ�����������ԣ����ֳ��˸��ڵ��������������������ô��ʱ����������֪����������ĸ��ڵ㣬Ҳ����ӵ���˸��ڵ㣬��֪�������������Ľڵ���������������͸�ȥ�����������������������������������ֿ��Ե����Ŀ�����һ���������ǿ���֪�����������ĸ��ڵ㣬��ô֪���ģ�

ǰ����������Ƿ��ţ������1 ���ұߵ�һ���������������ĸ��ڵ㣬�������������������֪���������Ľڵ������1 ���� 3 �����֮�󲻾���5����ô 5 ���� 1 ���������ĸ��ڵ㣬�Դ����ƣ�����������н�����Ƕ�ӵ���ˣ������ͽ������ˣ��������һ��ͼʾ��

![����дͼƬ����](http://img.blog.csdn.net/20161201165308950)


**��˼·����ò���ˣ�����ʵ�ִ����ֺ�˼·��Щ���죬��Ϊ�����ʵ����Ҫ���Ǻܶ����أ����ԣ�����ϸ�������ʵ�֣�**


**���룺**

```
	//ǰ�����У�1 2 3 4 5 6 - ��������:3 2 4 1 6 5
	//��û���ظ��ڵ��ǰ����
	Node* _GreatTree(int* prestart, int* preend,int* inarrstart,int* inarrend)
	{
		Node* root = new Node(*prestart);

		//���ֻ�е�ǰһ���ڵ㣬�򽫴����õ�����ڵ㷵�أ�
		if(prestart == preend && inarrstart == inarrend)
			return root;

		//�ҵ���������еĸ��ڵ�
		int* rootInorder = inarrstart;
		while(rootInorder <= inarrend && *prestart != *rootInorder)
				++rootInorder;
        
		//����������
		int lenth = rootInorder - inarrstart; //�������Ľڵ�����
		int* leftpreend = prestart+lenth;  // ������ǰ�������β
		
		//����ڵ�ǰ���ڵ��������������д���������
		if(lenth > 0)
			root->_left = _GreatTree(prestart+1,leftpreend,inarrstart,rootInorder);

		//����������
		int* rightprestart = leftpreend+1; //������ǰ������Ŀ�ʼ

		//�����ǰ���ڵ������������򴴽���������
		if(lenth < preend - prestart)
			root->_right = _GreatTree(rightprestart,preend,rootInorder+1,inarrend);

		return root;
	}

```

�����������ӣ�
[https://github.com/BITLQ/Data-structure/blob/master/BinaryTreeQuestions/%E6%A0%B9%E6%8D%AE%E4%B8%AD%E5%89%8D%E5%BA%8F%E9%81%8D%E5%8E%86%E5%BB%BA%E6%A0%91.txt](https://github.com/BITLQ/Data-structure/blob/master/BinaryTreeQuestions/%E6%A0%B9%E6%8D%AE%E4%B8%AD%E5%89%8D%E5%BA%8F%E9%81%8D%E5%8E%86%E5%BB%BA%E6%A0%91.txt)

----------

**<��>�ж�һ�����Ƿ�����ȫ������**

����������������ô˵��������뵽�˷������ͺܼ򵥲����ˣ�û��˼·�Ļ���Ȼ�Ͳ����ˣ����㲻���ʱ����һ�±��˵Ľ���������ֻ�ʹ�ļ��׵�ʹ���Լ���ô��ô�򵥵��ⶼ�벻������

��Ȼ�����ϴ����ϻ���

�ж�һ�����Ƿ�����ȫ��������������Ҫ֪��ʲô����ȫ���������������кܶ��˶����������Ǻ�������

> ��ȫ������(Complete Binary Tree)
> 
> ��������������Ϊh������ h ���⣬�������� (1��h-1) �Ľ�������ﵽ���������� h �����еĽ�㶼��������������ߣ��������ȫ��������
> ��ȫ���������������������������ġ��������ΪK�ģ���n�����Ķ����������ҽ�����ÿһ����㶼�����ΪK�����������б�Ŵ�1��n�Ľ��һһ��Ӧʱ��֮Ϊ��ȫ��������
> һ�ö���������ֻ���������һ���ϵĽ��Ķ�������С��2���������²��ϵĽ�㶼�����ڸò�����ߵ�����λ���ϣ���˶�������Ϊ��ȫ��������

���������������
![����дͼƬ����](http://img.blog.csdn.net/20161201171148335)


��ô���˽�����ȫ�������Ļ�������֮��������ƺ��ͱ�ü��˺ܶ࣬��Ȼǰk-1�������������������һ�����Ǵ�����û�м���ģ��˽���������ͬѧ���ź����׾��뵽���ò��������������⣬����ʲô�ǲ���������Ͳ�������׸���ˣ�

�����Ҹ����������ڲ�����������ֽⷨ��

1�����ñ�־��

�Ӹ��ڵ㿪ʼ������У�������в�Ϊ�գ�ѭ����
������һ��û������ӻ����Ҷ��ӵĽڵ�,���ñ�־λ,���֮������������/�Ҷ��ӵĽڵ㣬��ô�ⲻ��һ����ȫ��������

ͼʾ��
![����дͼƬ����](http://img.blog.csdn.net/20161203141236698)

�����㷨����ʵ�֣�

```
	bool IsCompleteTreeTwo(Node* root)
	{
		queue<Node*> q;
		q.push(root);
		Node* cur = q.front();
		int flag = 0;   //��־

		while(cur)
		{
            û������ȴ���Һ��ӣ�һ��������ȫ������
			if(cur->_left == NULL && cur->_right != NULL)
				return false;
          //���flaga==1�����ҵ�ǰ�ڵ��Һ��ӣ���false
			if(flag == 1 && (cur->_left != NULL || cur->_right != NULL))
				return false;

          //��ǰ�ڵ�û���Һ��ӻ�������
			if(cur->_left == NULL || cur->_right == NULL)
				flag = 1;

			if(cur->_left != NULL)
				q.push(cur->_left);
			if(cur->_right != NULL)
				q.push(cur->_right);

			q.pop();
			if(!q.empty())
				cur = q.front();
			else
				cur = NULL;
		}
		return true;
	}
```

2��ʣ������пշ�

�������ͬ����Ҫ����У���ͬ�������жϵķ�ʽ������һ����ȫ�����������ǰ��������нڵ㣬����Ҷ�ӽڵ�����ҿպ��Ӷ�����У�ͨ��������������ϵ�����кͳ����еķ�ʽ�����������һ����ͷԪ��Ϊ�գ���ô�������ȫ�������� ������ʱһ�������������нڵ㣬������ʣ���Ԫ�ؿ϶�ȫ��NULL�Ľڵ㣬Ҳ����Ҷ�ӽڵ�Ŀպ��ӣ�����������ˣ���ô����Ǹ�ˣ���Ͳ���һ����ȫ��������

ͼʾ��

ͼ��ʡ����ӳ��ӹ��̣�
![����дͼƬ����](http://img.blog.csdn.net/20161203143410193)


���Ĵ���ʵ�֣�

```
	bool IsCompleteTree(Node* root)
	{
		if(root == NULL)
			return false;
		//�õ������ڴ����
		queue<Node*> q;
		q.push(root);
		
		//�Ƚ����еĽڵ��Լ����ǵ����ҽڵ������
		Node* cur = q.front();
		while(cur) //���������һ���սڵ�ֹͣ
		{
			q.push(cur->_left);
			q.push(cur->_right);
			q.pop();
			
			cur = q.front();
		}
		//��ʱӦ������ȫ�������Ľ�β��
		//��ʱ��ȷ�����Ӧ���Ƕ����е�����Ԫ�ض���NULL����������ȫ������
		while(!q.empty())
		{
			if(q.front() != NULL)
				return false;
			q.pop();
		}
		return true;
	}
```

������������ʵ�����ӣ�

[https://github.com/BITLQ/Data-structure/blob/master/BinaryTreeQuestions/%E5%88%A4%E6%96%AD%E5%AE%8C%E5%85%A8%E4%BA%8C%E5%8F%89%E6%A0%91.txt](https://github.com/BITLQ/Data-structure/blob/master/BinaryTreeQuestions/%E5%88%A4%E6%96%AD%E5%AE%8C%E5%85%A8%E4%BA%8C%E5%8F%89%E6%A0%91.txt)


----------


**<��>������������ڵ�������������**

������  ������ǽ�ָoffer�ϵ�һ���⣬�ѵ�����ȫ��Ŀ��Ǹ�����������ú��ʵķ���������������

ʲô������������ȣ�
���Ӹ��ڵ�ֱ������ڵ��·�������һ�������ڵ㣬������������ȣ�

����ͼ��ʾ��
![����дͼƬ����](http://img.blog.csdn.net/20161208142228012?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYml0Ym9zcw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
��Ȼ��������Ҫ������������Щ�����

1������������
2�����и�ָ��Ķ�������������������
3��һ����ͨ����

���1��

��򵥵��龰�����������һ�Ŷ����������������й����ģ���ô����ֻҪ�ҵ�һ��������һ���ڵ����ߵ��ڣ����ұ���һ���ڵ�С���ߵ��ڵĽڵ�Ϳ����ˣ�����ڵ������͹������ȣ�����ܺ���⣬�Ͳ�����׸����ֱ���ϴ����ˣ�

```
//������С��������
    int MinCommonAncestor(int x,int y)
	{
		Node* cur = _root;
		Node* tmp = NULL;

		while(cur)
		{
			//������Ŀ��ڵ��ֵ���󣬼������������ң�
			if(cur->_data > x && cur->_data > y)
			{
				tmp = cur;
				cur = cur->_left;
			}
			//������Ŀ��ڵ��ֵ��С���������������ң�
			else if(cur->_data < x && cur->_data < y)
			{
				tmp = cur;
				cur = cur->_right;
			}

			//���������ҵ�Ŀ��ڵ�
			else if(cur->_data >= x && cur->_data <= y || cur->_data <= x && cur->_data >= y)
			{
					return cur->_data;
			}
		}
	}
```


�龰2��
���Ƕ����������������и�ָ��Ķ�������

![����дͼƬ����](http://img.blog.csdn.net/20161208144359464?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYml0Ym9zcw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


��Ȼ�и�ָ�룬��������������ô������ֱ�ļ��ˣ���Ϥ��ͬѧӦ�úܿ�ͷ��֣������Ļ��Ͱ�����ת��Ϊ�������ཻ����ĵ�һ�������ڵ�������ˣ���Ȼ��Ϊʲô�ǵ�һ�������ڵ㣿ǰ��˵��͹������ȵĸ����ʱ����˵�ǴӸ��ڵ�ֱ������ڵ�����һ�������ڵ���

��Ҫ���������룬�����������˸��ڵ�Ļ�����ȫ���Դ�����Ŀ��ڵ����ϱ������ⲻ���Ƿ��ŵ��ˣ��������ཻ�ڵ�� ��һ�������ڵ㣬�����㲻һ����

��������Ŀ��ڵ����ϱ�����ʱ�����кü��ְ취������˵����ջ�����ó��Ȳ�ķ�����ջ�ķ��������Ȳ��ã�������һ���龰�ã����������õ� �ǳ��Ȳ�ķ�������Ϊ���Ȳ�ķ�����

���Ȳ������Ѱ������Ŀ��ڵ��ʱ��˳��ѴӸ��ڵ㵽������������·�����ȱ����������ֱ���count1��count2;   Ȼ��������ǵĲ�ֵ������������·���ȶ̵�����·����Ľڵ�����ΪʲôҪ�����أ�

��Ϊ������Ҫ������·���ϵĵ�һ�������ڵ㣬�����������Ŀ��ڵ�ͬʱ�����Ļ����϶��Ͳ�����ô��Ҫ�Ľ���ˣ����������Ǹ�ͼ���ٵ����ӣ����7��6ͬʱ��ʼ�������Ǿ��������ˣ������������·�������Ǹ��ڵ����������ǵĳ��Ȳ�Ľڵ�������ڵ���ͬʱ���ϱ�������һ����ͬ�Ľڵ㲻���ǵ�һ�������ڵ��ˣ���Ȼ�������ų�������ͬ�ڵ�Ŀ����ԣ���

���룺

```
	//������С��������
    int MinCommonAncestor(int x,int y)
	{
		//������֪�ڵ����ϲ��ҵ����ڵ㣬����������ĵ�һ������
		//ǰ�ᶼ�ǽ�����û���ظ�Ԫ�صĶ�������

		if(x == y)
			return x;

		//�����Ƿ��ؿյ����������ʱ�����ǷǷ�����
		
		//���������������һ�������ڵ�ķ�����⣬���ַ�������٣�����ջ�����󳤶Ȳ
		//���õ����ַ���,�󳤶Ȳ�

		int count1 = 1;
		int count2 = 1;
		Node* nodeone = NULL;
		Node* nodetwo = NULL;
		Node* cur = _root;

		while(cur)
		{
			if(cur->_data == x)
			{
				nodeone = cur;
				break;
			}
			++count1;
		}

		cur = _root;
		while(cur)
		{
			if(cur->_data == y)
			{
				nodetwo = cur;
				break;
			}
			++count2;
		}
		
		//������Ȳ�
		int len = abs(count1 - count2);
		if(count1 >= count2)
			return _FindFirstNode(nodeone,nodetwo,len);
		else
			return _FindFirstNode(nodeone,nodetwo,len);
	}

	int _FindFirstNode(Node* one,Node* two, int len)
	{
		while(len--)
			one = one->_parent;

		while(one != two)
		{
			one = one->_parent;
			two = two->_parent;
		}

		return one->_data;
	}
```

�龰3��

��ͨ�Ķ��������Ǿͱ��������ˣ�ֻ��������ԭʼ�ĸ���Ӹ��ڵ㿪ʼ�ֱ�����Ŀ��ڵ��·�������һ�������ڵ㣻

�����ľ�û��ʲô���ɿ����ˣ�����Ϊ�˷�����������Զ�����Ϊ������Ȼ�����棬�Ĳ�ľ��鷳���ˣ���

�����������ַ�����ʵ�֣����ݹ������ջ��

�ݹ��˼����Щ�����ڵ�һ���˼�룺���õݹ������ֵ�ķ��������жϣ�

��ͣ�����µݹ飬����ҵ�����Ŀ��ڵ������һ���ڵ㣬�Ͱѵ�ǰ����ڵ㷵�أ�����ݹ鵽Ҷ�ӽڵ㻹û���ҵ�һ��Ŀ��ڵ�ͷ���NULL��

�������������ķ�����������left��right����ΪNULL����ô�ܺã�˵������Ŀ��ڵ�ֱ����ҵ����������ϣ����ص�ǰ�ڵ㣬��ǰ�ڵ������͹������ȣ����ֻ��һ���������صķǿ�ֵ����ô��˵������Ŀ��ڵ㶼���ҵ��Ǹ����طǿսڵ�������ϣ�������ֵ������͹������ȣ����������Ѷ��ĵط���Ϊʲô�ǿյķ���ֵ������͹������ȣ�

��ϵ����ͼ������
![����дͼƬ����](http://img.blog.csdn.net/20161208151714354?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYml0Ym9zcw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

��������Ҫ���2��5����͹������ȣ���ô���ǴӸ��ڵ�ݹ�����±������������϶��Ƿ���NULL�ˣ����������ݹ鵽2��ʱ��ͷ����ˣ���Ȼ���Ѿ��ҵ���һ��Ŀ��ڵ㣬���ҾͲ������ݹ��ˣ�Ҫ��Ȼ��һ��Ŀ��ڵ�����һ�������ϣ�������2���£�Ҫ��Ȼ5������2�������ϣ��Ǿ͸����ˣ�2������͹��������ˣ�����Ҳ�Ͳ����ںܶ�ͬѧ��������Ŀ��ڵ���һ�����ϵ������������е�����ˣ�

������θо��Ҷ���2�ˣ�

```

int MinCommonAncestor(int x,int y)
	{
		Node* left = NULL;
		Node* right = NULL;

		Node* cur = _MinCommonAncestor(_root,x,y);
		return cur->_data;
	}
	
	Node* _MinCommonAncestor(Node* root,const int x, const int y)
	{
		Node* left = NULL;
		Node* right = NULL;

		if(root == NULL)
			return NULL;

		if(root->_data == x || root->_data == y)
			return root;

		left = _MinCommonAncestor(root->_left,x,y);
		right = _MinCommonAncestor(root->_right,x,y);

		if(left && right)
			return root;

		return left ? left : right;
	}
```

����ջ��ʵ��ԭ����������Ǳ���·������ջ�ĺ���ȳ������ԣ����ֱ���˵�һ����ͬ�ڵ�������ˣ�

��ͼ��
![����дͼƬ����](http://img.blog.csdn.net/20161208152625686?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYml0Ym9zcw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

7��6��·���ֱ���ջ��Ȼ������Ҫһ��·���������ʱ���·����ͺܼ򵥵Ŀ�������ջ��size()�����ͬ����size���ջ��pop��·�����Ԫ�أ�Ȼ������ջͬʱpop��֪������һ����ͬ��Ԫ�أ�����ʵ�Ѿ����龰2һ���ˣ�

���룺

```
//���õݹ��ջ����·����Ȼ����ȥ����·���Ĳ�ֵ������ͬʱpop();�ҵ���һ�Բ�ͬ��

	int MinCommonAncestor(int x,int y)
	{
		stack<Node*> s1;
		_MinCommonAncestor(_root,s1,x);
		stack<Node*> s2;
		_MinCommonAncestor(_root,s2,y);

		if(s1.size() > s2.size())
		{
			int count = s1.size() - s2.size();
			return FindCommonParent(s1,s2,count)->_data;
		}

		int count = s2.size() - s1.size();
		return FindCommonParent(s2,s1,count)->_data;
	}

	Node* FindCommonParent(stack<Node*> first,stack<Node*> second,int count)
	{
		while(count--)
			first.pop();

		Node* cur = NULL;

		while(!first.empty() && !second.empty() && first.top() != second.top())
		{
			first.pop();
			second.pop();
		}

		return first.top();
	}

	bool _MinCommonAncestor(Node* root,stack<Node*>& s, const int x)
	{
		if(root == NULL)
			return false;

		if(root->_data == x)
		{
			s.push(root);
			return true;
		}

		s.push(root);

		bool left = _MinCommonAncestor(root->_left,s,x);
		bool right = _MinCommonAncestor(root->_right,s,x);

		if(left == false && right == false)
		{
			s.pop();
			return false;
		}

		return true;
	}
```


�����������ӣ�
[https://github.com/BITLQ/Data-structure/blob/master/BinaryTreeQuestions/%E6%9C%80%E4%BD%8E%E5%85%AC%E5%85%B1%E7%A5%96%E5%85%88.txt](https://github.com/BITLQ/Data-structure/blob/master/BinaryTreeQuestions/%E6%9C%80%E4%BD%8E%E5%85%AC%E5%85%B1%E7%A5%96%E5%85%88.txt)


----------
**<��>������������ת����һ�������˫������Ҫ���ܴ����κ��µĽ�㣬ֻ�ܵ������н��ָ���ָ��**


��������ô˵�أ�����Ȿ��Ҳ�ܼ򵥣���������һ���������������Ȼ����ˣ���Ȼ�и��������϶�������ô�򵥣���ô����ʵ��ĸ����ˣ��������ʵ���Ǽ򵥵Ŀ�����Զ������������˽⣻

������������Ҳ���зֱ�ָ������������������ָ�룬��������������� 
���Ѷ�����������Ϊ�����˫��������һ�㲻���뵽�Ŀ϶������������ ������Ȼ����Ǹı�ָ���ָ����Ϊ���������µĽڵ㣬���ı�ָ���ָ��Ļ����ͼ򵥺ܶ��ˣ�ֻҪ��left��Ϊָ��ǰһ���ڵ��ָ�룬right��Ϊָ���һ���ڵ�� ָ��Ϳ����ˣ���Ȼ�������ʵ�ֹ����п϶�����Щ���ϸ�ڲ��죡

���룺

```
	Node* TurnToList()
	{
		Node* prev = NULL;
		_TurnToList(_root,prev);

		//����˫�������ͷ���
		Node* cur = _root;
		while(cur && cur->_left)
			cur = cur->_left;

		return cur;
	}

void _TurnToList(Node* root, Node*& prev)
	{
		if(root == NULL)
			return;

		_TurnToList(root->_left,prev);
		if(prev != NULL)
			prev->_right = root;
		prev = root;
		_TurnToList(root->_right,prev);
	}
```


���������������ӣ�
[https://github.com/BITLQ/Data-structure/blob/master/BinaryTreeQuestions/%E4%BA%8C%E5%8F%89%E6%A0%91%E8%BD%AC%E9%93%BE%E8%A1%A8.txt](https://github.com/BITLQ/Data-structure/blob/master/BinaryTreeQuestions/%E4%BA%8C%E5%8F%89%E6%A0%91%E8%BD%AC%E9%93%BE%E8%A1%A8.txt)

