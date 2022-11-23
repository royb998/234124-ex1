#include <assert.h>
#include "RLEList.h"

typedef struct node
{
    char data;
    int count;
    struct node* next;
} * Node;

struct RLEList_t
{
    Node first;
    Node last;
    int count;
};

/**
 * Remove the given `node` from the given list by linking the one before it and
 * the one after it.
 *
 * @note When removing the first node, `previous` should be set to null.
 * @note `node` must represent no characters, that is, have a count of 0.
 *
 * @param list List to remove node from.
 * @param node Node to remove.
 * @param previous Node immediately preceding `node`. NULL if `node` is the
 *      first node in `list`.
 *
 * @return
 *  RLE_LIST_NULL_ARGUMENT if any argument is NULL (when it shouldn't be).
 *  RLE_LIST_SUCCESS upon successful operation.
 * */
RLEListResult removeNode(RLEList list, Node node, Node previous)
{
    assert(0 == node->count);
    assert(node == previous->next);
    if (NULL == node || (NULL == previous && list->first != node))
    {
        return RLE_LIST_NULL_ARGUMENT;
    }

    /* Only enters if removing the first node. */
    if (previous == NULL)
    {
        list->first = node->next;
    }
    else
    {
        previous->next = node->next;
        if (node == list->last)
        {
            list->last = previous;
        }
    }

    free(node);
    return RLE_LIST_SUCCESS;
}

RLEList RLEListCreate()
{
    RLEList list = malloc(sizeof(struct RLEList_t));
    if (NULL == list)
    {
        return NULL;
    }

    list->count = 0;
    list->first = list->last = NULL;
    return list;
}

void RLEListDestroy(RLEList list)
{
    if (list == NULL)
    {
        return;
    }

    if (list->first != NULL)
    {
        Node node = list->first;
        while (NULL != node)
        {
            Node toDelete = node;
            node = node->next;
            free(toDelete);
        }
    }

    free(list);
}

RLEListResult RLEListAppend(RLEList list, char value)
{
    if (list == NULL)
    {
        return RLE_LIST_NULL_ARGUMENT;
    }

    if (NULL != list->last && list->last->data == value)
    {
        list->last->count++;
    }
    else
    {
        Node new = malloc(sizeof(struct node));
        if (NULL == new)
        {
            return RLE_LIST_OUT_OF_MEMORY;
        }
        new->data = value;
        new->count = 1;
        new->next = NULL;

        if (list->first == NULL)
        {
            list->first = list->last = new;
        }
        else
        {
            list->last->next = new;
            list->last = new;
        }
    }

    list->count++;
    return RLE_LIST_SUCCESS;
}

int RLEListSize(RLEList list)
{
    if (list == NULL)
    {
        return -1;
    }

    return list->count;
}

RLEListResult RLEListRemove(RLEList list, int index)
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
    Node previous = NULL;

    temp_index -= (current->count);
    while (temp_index >= 0)
    {
        previous = current;
        current = current->next;
        temp_index -= (current->count);
    }

    current->count--;
    /* Remove node if count for current node goes down to 0. */
    if (current->count == 0)
    {
        removeNode(list, current, previous);
    }

    /* Merge duplicate consecutive characters. */
    Node next = previous->next;
    if (previous->data == next->data)
    {
        previous->count += next->count;
        next->count = 0;
        removeNode(list, next, previous);
    }

    list->count--;
    return RLE_LIST_SUCCESS;
}

char RLEListGet(RLEList list, int index, RLEListResult* result)
{
    if (list == NULL)
    {
        if (result != NULL)
        {
            *result = RLE_LIST_NULL_ARGUMENT;
        }

        return 0;
    }
    if (index >= list->count || index < 0)
    {
        if (result != NULL)
        {
            *result = RLE_LIST_INDEX_OUT_OF_BOUNDS;
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
        *result = RLE_LIST_SUCCESS;
    }

    return current->data;
}

RLEListResult RLEListMap(RLEList list, MapFunction map_function)
{
    if (list == NULL || map_function == NULL)
    {
        return RLE_LIST_NULL_ARGUMENT;
    }

    Node previous = NULL;
    for (Node node = list->first; node != NULL; node = node->next)
    {
        node->data = map_function(node->data);
        if (node != list->first && (node->data == previous->data))
        {
            previous->count += node->count;
            node->count = 0;
            removeNode(list, node, previous);
            node = previous;
        }
        previous = node;
    }

    return RLE_LIST_SUCCESS;
}

void itoa(int value, char* p_string, int n)
{
    snprintf(p_string, n, "%d", value);
}

int integerSize(int value)
{
    char countStr[10];

    itoa(value, countStr, 10);
    int length = strlen(countStr);

    return length;
}

char* RLEListExportToString(RLEList list, RLEListResult* result)
{
    if (list == NULL)
    {
        if (NULL != result)
        {
            *result = RLE_LIST_NULL_ARGUMENT;
        }
        return NULL;
    }

    // Resolve final string size
    int resultStrLength = 0;

    for (Node node = list->first; node != NULL; node = node->next)
    {
        int numLength = integerSize(node->count);
        resultStrLength += numLength + 2;
    }

    // Allocate memory for final string
    char* resultStr = malloc(resultStrLength + 1);
    if (resultStr == NULL)
    {
        if (NULL != result)
        {
            *result = RLE_LIST_OUT_OF_MEMORY;
        }
        return NULL;
    }

    // Fill final string
    int index = 0;
    for (Node node = list->first; node != NULL; node = node->next)
    {
        sprintf(&resultStr[index], "%c%d\n", node->data, node->count);
        int length = integerSize(node->count);
        index += length + 2;
    }

    if (NULL != result)
    {
        *result = RLE_LIST_SUCCESS;
    }
    return resultStr;
}
