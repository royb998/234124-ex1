#include "RLEList.h"

typedef struct node
{
	char data;
	int count;
	struct node *next;
} *Node;

struct RLEList_t
{
	//TODO: implement
	struct node *first;
	struct node *last;
	int count;
};

RLEList RLEListCreate ()
{
	RLEList list = malloc(sizeof(struct RLEList_t));
	if (!list)
	{
		return NULL;
	}

	list->count = 0;
	list->first = list->last = NULL;
	return list;
}

void RLEListDestroy (RLEList list)
{
	if (list == NULL)
	{
		return;
	}

	if (list->first != NULL)
	{
		Node node = list->first;
		while (node)
		{
			Node toDelete = node;
			node = node->next;
			free(toDelete);

		}
	}

	free(list);
}

RLEListResult RLEListAppend (RLEList list, char value)
{
	if (list == NULL || value == '\0')
	{
		return RLE_LIST_NULL_ARGUMENT;
	}

	if (list->first == NULL)
	{
		Node newFirst = malloc(sizeof(struct node));
		if (newFirst == NULL)
		{
			return RLE_LIST_OUT_OF_MEMORY;
		}
		list->first = list->last = newFirst;
		newFirst->data = value;
		newFirst->count = 0;
		newFirst->next = NULL;
	}

	if (list->last->data == value)
	{
		list->last->count++;
	}
	else
	{
		Node newLast = malloc(sizeof(struct node));
		if (newLast == NULL)
		{
			return RLE_LIST_OUT_OF_MEMORY;
		}
		newLast->data = value;
		newLast->count = 1;
		newLast->next = NULL;
		list->last->next = newLast;
		list->last = newLast;
	}

	list->count++;
	return RLE_LIST_SUCCESS;
}

int RLEListSize (RLEList list)
{
	if (list == NULL)
	{
		return -1;
	} else
	{
		return list->count;
	}
}

RLEListResult RLEListRemove (RLEList list, int index)
{
	if (list == NULL)
	{
		return RLE_LIST_NULL_ARGUMENT;
	}
	if (index >= list->count || index < 0)
	{
		return RLE_LIST_INDEX_OUT_OF_BOUNDS;
	}

	int temp_index = index;
	Node current = list->first;
	Node one_before = NULL;
	while ((temp_index -= (current->count)) >= 0)
	{
		one_before = current;
		current = current->next;
	}

	current->count--;
	if (current->count == 0)
	{
		if (list->first == current)
		{
			list->first = current->next;
		}
		else
		{
			if (current->next == NULL)
			{
				list->last = one_before;
			}

			one_before->next = current->next;
		}

		free(current);
	}

	list->count--;
	return RLE_LIST_SUCCESS;

}

char RLEListGet (RLEList list, int index, RLEListResult *result)
{
	if (list == NULL)
	{
		if (result != NULL)
		{
			*result =  RLE_LIST_NULL_ARGUMENT;
		}

		return 0;
	}
	if (index >= list->count || index < 0)
	{
		if (result != NULL)
		{
			*result =  RLE_LIST_INDEX_OUT_OF_BOUNDS;
		}

		return 0;
	}
	int temp_index = index;
	Node current = list->first;
	while (temp_index - (current->count) >= 0)
	{
		temp_index = temp_index - (current->count);
		current = current->next;
	}

	if (result != NULL)
	{
		*result =  RLE_LIST_SUCCESS;
	}

	return current->data;
}

RLEListResult RLEListMap(RLEList list, MapFunction map_function)
{
	if (list == NULL || map_function == NULL)
	{
		return RLE_LIST_NULL_ARGUMENT;
	}

	for (Node node = list->first; node != NULL; node = node->next)
	{
		node->data = map_function(node->data);
	}

	return RLE_LIST_SUCCESS;
}

char* RLEListExportToString(RLEList list, RLEListResult* result)
{
	RLEListResult dummyResult;
	if (result == NULL)
	{
		result = &dummyResult;
	}

	if (list == NULL)
	{
		*result = RLE_LIST_NULL_ARGUMENT;
		return NULL;
	}

	// Resolve final string size
	char countStr[10];
	int resultStrLength = 0;
	for (Node node = list->first; node != NULL; node = node->next)
	{
		itoa(node->count, countStr, 10);
		int numLength = strlen(countStr);
		resultStrLength += numLength + 2;
	}

	// Allocate memory for final string
	char *resultStr = malloc(resultStrLength + 1);
	if (resultStr == NULL)
	{
		*result = RLE_LIST_OUT_OF_MEMORY;
		return NULL;
	}

	// Fill final string
	int index = 0;
	for (Node node = list->first; node != NULL; node = node->next)
	{
		sprintf(&resultStr[index], "%c%d\n", node->data, node->count);
		itoa(node->count, countStr, 10);
		int length = strlen(countStr);
		index += length + 2;
	}

	*result = RLE_LIST_SUCCESS;
	return resultStr;
}
