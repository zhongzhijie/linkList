#include "linkList.h"

#define STU_NAME_LEN 16

typedef struct student
{
    uint32 index;
    uint32 score;
    uint8 name[STU_NAME_LEN];
}stStudent;

#define assertSuccess(uiRet) {if (SUCCESS != uiRet) return uiRet;}

