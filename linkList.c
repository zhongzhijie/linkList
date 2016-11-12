#ifndef __SELFDEF_LINKLIST_C__
#define __SELFDEF_LINKLIST_C__

#include "linklist.h"

/* create list and malloc memory from heap */
uint32 listNew(struct linkList** list)
{
    *list = (struct linkList *)malloc(sizeof(struct linkList));

    if (NULL == *list)
    {
        return FAIL;
    }

    memset(*list, 0, sizeof(struct linkList));

    return SUCCESS;
}

/* create list node and malloc memory from heap */
uint32 nodeNew(struct listNode** node)
{
    *node = (struct listNode*)malloc(sizeof(struct listNode));

    if (NULL == *node)
    {
       return FAIL;
    }

    memset(*node, 0, sizeof(struct listNode));

    return SUCCESS;
}

/* free list node */
uint32 nodeFree(struct listNode* node)
{
    if (NULL != node->objPtr)
    {
        free(node->objPtr);
    }

    free(node);

    return SUCCESS;
}

/* add node to the tail of list */
static uint32 listAddNodeTail(struct linkList* list, struct listNode* node)
{
    if (NULL == list || NULL == node)
    {
        return FAIL;
    }

    if (NULL == list->tail)
    {
        list->head = node;
        list->tail = node;
    }
    else
    {
        node->prev = list->tail;
        list->tail->next = node;
        list->tail = node;
    }

    list->count++;
    
    return SUCCESS;
}

/* add node to the head of list */
static uint32 listAddNodeHead(struct linkList* list, struct listNode* node)
{
    if (NULL == list || NULL == node)
    {
        return FAIL;
    }
    
    if (NULL == list->head)
    {
        list->head = node;
        list->tail = node;
    }
    else
    {
        node->next = list->head;
        list->head->prev = node;
        list->head = node;
    }

    list->count++;
    
    return SUCCESS;
}

/* add obj to a list, need malloc memory for obj and create node  */
uint32 listObjAdd(struct linkList* list, void* obj, uint32 objLen)
{
    uint32 uiRet = FAIL;
    struct listNode* newNode = NULL;
    void* data = NULL;
    
    if (NULL == list)
    {
        return FAIL;
    }

    uiRet = nodeNew(&newNode);
    if (FAIL == uiRet)
    {
        return FAIL;
    }

    data = (void*)malloc(objLen);
    if (NULL == data)
    {
        nodeFree(newNode);
        return FAIL;
    }

    memcpy(data, obj, objLen);
    
    newNode->next = NULL;
    newNode->prev = NULL;
    newNode->objLen = objLen;
    newNode->objPtr = data;

    listAddNodeTail(list, newNode);

    return SUCCESS;
}

/* del node from list */
uint32 listDelNode(struct linkList* list, struct listNode* node)
{
    struct listNode* spNode = NULL;
    struct listNode* nextNode = NULL;

    if (NULL == list || NULL == node)
    {
        return FAIL;
    }
    
    spNode = list->head;

    while(NULL != spNode)
    {
        nextNode = spNode->next;
        
        if (spNode == node)
        {
            if (NULL != spNode->prev)
            {
                spNode->prev->next = spNode->next;
            }
            else
            {
                list->head = spNode->next;
            }

            if (NULL != spNode->next)
            {
                spNode->next->prev = spNode->prev;
            }
            else
            {
                list->tail = spNode->prev;
            }

            nodeFree(spNode);
            list->count--;
            return SUCCESS;
        }

        spNode = nextNode;
    }

    return FAIL;
}

/* del obj from list and the free the node related to it */
uint32 listObjDel(struct linkList* list, void* obj)
{
    struct listNode* node = NULL;
    struct listNode* nextNode = NULL;
    uint32 uiRet = FAIL;
    
    if (NULL == list || NULL == obj)
    {
        return FAIL;
    }

    node = list->head;

    while(NULL != node)
    {
        nextNode = node->next;
        
        if (node->objPtr == obj)
        {
            uiRet = listDelNode(list, node);
            return uiRet;
        }
        node = nextNode;
    }

    return FAIL;
}

/* del all node from list */
uint32 listClear(struct linkList* list)
{
    struct listNode* node = NULL;
    struct listNode* nextNode = NULL;
    
    if (NULL == list)
    {
        return FAIL;
    }

    node = list->head;

    while(NULL != node)
    {
        nextNode = node->next;
        
        listDelNode(list, node);

        node = nextNode;
    }

    list->count = 0;

    return SUCCESS;
}

/* free list and all the node attached to it */
uint32 listFree(struct linkList* list)
{
    if (NULL == list)
    {
        return FAIL;
    }
    
    listClear(list);

    free(list);
}

/* get the num of node attached to the list */
uint32 listGetCount(struct linkList* list)
{
    if (NULL == list)
    {
        return FAIL;
    }

    return list->count;
}

/* add list to another list */
uint32 listAddList(struct linkList* dstList, struct linkList* srcList)
{
    struct listNode* node = NULL;
    struct listNode* nextNode = NULL;
    
    if (NULL == dstList || NULL == srcList)
    {
        return FAIL;
    }

    node = srcList->head;

    while(NULL != node)
    {
        nextNode = node->next;
        listObjAdd(dstList, node->objPtr, node->objLen);
        node = nextNode;
    }

    return SUCCESS;
}

/* initial list and register the hook function */
uint32 listInit(struct linkList* list)
{
    uint32 uiRet = SUCCESS;

    if (NULL == list)
    {
        return FAIL;
    }
    
    list->head = NULL;
    list->tail = NULL;
    list->count = 0;

    list->objAdd = listObjAdd;
    list->objDel = listObjDel;
    list->freeList = listFree;
    list->clearList = listClear;
    list->getLength = listGetCount;

    return SUCCESS;
}

static uint32 dataPrint(void* data, uint32 dataLen)
{
    uint32 loop = 0;
    char* dataIndex = (char*)data;

    printf("data length=%d\r\n", dataLen);

    for (loop = 0; loop < dataLen; loop++)
    {
        printf("%02x ", *(dataIndex+loop));
        if ((loop+1) % 16 == 0)
        {
            printf("\r\n");
        }
    }
    printf("\r\n");

    return SUCCESS;
}

/* print the obj of the list, just for test */
uint32 listDataPrint(struct linkList* list)
{
    struct listNode* node = NULL;
    struct listNode* nextNode = NULL;
    
    if (NULL == list)
    {
        return FAIL;
    }

    printf("number of objects is %d.\r\n", list->count);

    node = list->head;

    while(NULL != node)
    {
        nextNode = node->next;
        
        dataPrint(node->objPtr, node->objLen);

        node = nextNode;
    }

    return SUCCESS;
}

#endif
