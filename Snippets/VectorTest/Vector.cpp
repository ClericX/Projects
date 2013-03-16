#include <Windows.h>
#include <stdio.h>
#include <iostream>


template <class T>
class Vector
{
public:
	Vector(void)
	{
		NumElems = 0;
		ElemSize = sizeof(T);
		MemPtr = 0;
	}
	~Vector(void)
	{
		free(MemPtr);
	}

	unsigned char *MemPtr;
	unsigned int ElemSize;
	unsigned int NumElems;

	void Add(T elem)
	{
		// Allocate more memory for the new element.
		NumElems++;
		MemPtr = (unsigned char *)realloc(MemPtr, (ElemSize * NumElems));
		(*this)[NumElems - 1] = elem;
	}

	T &operator [] (unsigned int index)
	{
		return ((T *)MemPtr)[index];
	}
};

int main()
{
	Vector<int> ints;
	ints.Add(2);
	ints.Add(5);
	for (int i = 0; i < ints.NumElems; i++)
	{
		printf("%d\n", ints[i]);
	}
	system("pause");
}