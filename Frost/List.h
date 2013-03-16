#pragma once

#include "DLL.h"

#include <Windows.h>

#include "Log.h"

namespace Frost
{
	class FROSTAPI ListNode
	{
	public:
		ListNode(void);
		~ListNode(void);

		ListNode *Prev; // A reference to the previous node in the node chain.
		ListNode *Next; // A reference to the next node in the node chain.
		void *Content; // A pointer to a block of allocated memory which contains the content stored in this node.

		void SetContent(unsigned int Size, void *TheContent); // A function to allocate a memory block and set the content within it for this node.
		void DeleteContent(); // A function to free the memory allocated by this node, mainly used for the destructor.
	};

	template <class T>
	class List
	{
	public:
		List(void);
		~List(void);

		unsigned int SizeOfAnElement;
		ListNode *First;
		ListNode *Last;

		void Insert(unsigned int atPos, T object);
		void Add(T object);
		void Remove(unsigned int index);
		ListNode *FindNode(unsigned int index);
		T Find(unsigned int index);
		T operator [] (unsigned int index);
		unsigned int count();
		void clear();
		unsigned int find(T object);
		bool contains(T object);
		bool isEmpty();

	private:
		unsigned int NumNodes;
	};

	template <class T>
	List<T>::List(void)
	{
		NumNodes = 0;
		SizeOfAnElement = sizeof(T);
		First = 0;
		Last = 0;
	
		if (SizeOfAnElement == 0)
		{
			Log::Msg("An instance of a List class was initialized with an element size of 0.", Log::FatalError);
		}
	}

	template <class T>
	List<T>::~List(void)
	{
		// Clear the list and free all node memory.
		clear();
	}

	template <class T>
	void List<T>::Insert(unsigned int atPos, T object)
	{
		if (atPos < 0)
		{
			Log::Msg("Tried to insert a List object at an index below 0.", Log::FatalError);
			return;
		}

		ListNode *curNode = new ListNode();
		curNode->SetContent(SizeOfAnElement, (void *)&object);

		if ( count() == 0 ) // If this is true that means this is the first node ever to be in the list.
		{
			First = curNode;
			Last = curNode;
		}
		// Otherwise this is not the initial node and should be handled to be added to the right index of the node tree.
		else if (atPos == 0) // Inserting at the beginning. Need to set First.
		{
			First->Prev = curNode;
			curNode->Next = First;
			First = curNode;
		}
		else if (atPos > count() - 1) // Inserting at the end. Need to set Last.
		{
			Last->Next = curNode;
			curNode->Prev = Last;
			Last = curNode;
		}
		else // This is a node being inserted somewhere in between the first and last nodes. This means a little more work.
		{
			Log::Msg("Inserting nodes between existing nodes is unsupported.", Log::FatalError);

			ListNode *modLeft = FindNode(atPos - 1);
			modLeft->Next = curNode;
			curNode->Prev = modLeft;

			ListNode *modRight = FindNode(atPos + 1);
			modRight->Prev = curNode;
			curNode->Next = modRight;
		}

		NumNodes++;
	}

	template <class T>
	void List<T>::Add(T object)
	{
		Insert(count(), object);
	}

	template <class T>
	void List<T>::Remove(unsigned int index)
	{
		// Get the node.
		ListNode *theNode = FindNode(index);

		// Remove it from the node chain.
		// This means updating all references that are relative to the node in question.
		// Since relative indexes, those offsets have to be adjusted when removing a node as well.
		ListNode *oldNext = theNode->Next;
		ListNode *oldPrev = theNode->Prev;

		if (oldPrev) // If this node has a previous node (can be either the last node or any middle node).
		{
			oldPrev->Next = oldNext;
		}
		if (oldNext) // If this node has a next node (can be either the first node or any middle node).
		{
			oldNext->Prev = oldPrev;
		}
		if (theNode == First) // If we are affecting the first node, adjust it accordingly.
		{
			First = oldNext;
		}
		if (theNode == Last) // If we are affecting the last node, adjust it accordingly.
		{
			Last = oldPrev;
		}

		// Free node memory usage and subtract from saved list length.
		delete theNode;
		NumNodes--;

		// If this is the last node and it's being removed, these pointers should be nulled.
		if (count() == 0)
		{
			First = 0;
			Last = 0;
		}
	}

	template <class T>
	ListNode *List<T>::FindNode(unsigned int index)
	{
		// Just some  small sanity checks.
		if (count() == 0)
		{
			Log::Msg(String("Tried to find() an item at index ") + (int)index + " when no items exist within the list.", Log::FatalError);
			return First; // This is the same thing as return (ListNode *)0 because if there are no nodes or the only one is removed, First is 0.
		}
		else if (index > count() - 1)
		{
			Log::Msg(String("Tried to find a List index that is out of bounds. Try: ") + (int)index + " | Max: " + (int)count(), Log::FatalError);
			return First;
		}

		// Find what we're looking for.
		ListNode *retVal = First;

		for (unsigned int i = 0; i < index; i++)
		{
			retVal = retVal->Next;
		}

		return retVal;
	}

	template <class T>
	T List<T>::Find(unsigned int index)
	{
		return (*((T *)FindNode(index)->Content));
	}

	template <class T>
	T List<T>::operator [] (unsigned int index)
	{
		return Find(index);
	}

	template <class T>
	unsigned int List<T>::count()
	{
		return NumNodes;
	}

	template <class T>
	void List<T>::clear()
	{
		// Delete all list content; every node.
		unsigned int TheCount = count();

		for (unsigned int i = 0; i < TheCount; i++)
		{
			Remove(0);
		}
	}

	template <class T>
	unsigned int List<T>::find(T object)
	{
		unsigned int i;
		for (i = 0; i < count(); i++)
		{
			if ((*this)[i] == object)
			{
				return i;
			}
		}
		return (unsigned int)-1;
	}

	template <class T>
	bool List<T>::contains(T object)
	{
		return find(object) != -1;
	}

	template <class T>
	bool List<T>::isEmpty()
	{
		return count() == 0;
	}
};