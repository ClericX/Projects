#define FROST_EXPORT
#include "List.h"

namespace Frost
{
	ListNode::ListNode(void)
	{
		Prev = 0;
		Next = 0;
		Content = 0;
	}

	ListNode::~ListNode(void)
	{
		DeleteContent();
	}

	void ListNode::SetContent(unsigned int Size, void *TheContent)
	{
		DeleteContent();

		Content = malloc(Size);
		memcpy(Content, TheContent, Size);
	}

	void ListNode::DeleteContent()
	{
		if (Content != 0)
		{
			free(Content);
		}
	}
}