# Data-structure
data   structure
//堆排序
//升序为例
//还是利用向下调整来实现
#include<iostream>
#include<cassert>

using namespace std;

void AdJustDown (int* arr, int parent, size_t size)
{
	int child = parent*2 +1;

	while(child < size)
	{
		if(child + 1 < size && arr[child] < arr[child + 1])
			++child;
		if(arr[child] > arr[parent])
		{
			swap(arr[child],arr[parent]);
			parent = child;
			child = parent*2 + 1;
		}

		else
			break;
	}
}

void HeapSort(int* arr, size_t size)
{
	assert(arr);

	//建堆
	for(int i = (size - 2)/2; i >= 0; i-- )
	{
		AdJustDown (arr, i, size);
	}

	//正式开始堆排序
	//一共排size-1趟；
	for(int i = size - 1; i > 0; i-- )
	{
		swap(arr[0],arr[i]);
		AdJustDown(arr,0, i );
	}
}
