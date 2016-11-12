#include "linkListDemo.h"

struct linkList* gStudentList = NULL;

uint32 studentListInit(void)
{
    uint32 uiRet = FAIL;

    uiRet = listNew(&gStudentList);
    assertSuccess(uiRet);

    uiRet = listInit(gStudentList);
    assertSuccess(uiRet);

    return uiRet;
}

int main(int argc,char **argv)

{
    uint32 uiRet = FAIL;
    uint32 loop = 0;
    
    stStudent studentData[] = {
        {1,98,"liming"},
        {2,94,"zhangwei"},
        {3,93,"zhongwen"},
        {4,97,"heyang"},
        {5,91,"liubang"},
    };

    uiRet = studentListInit();
    assertSuccess(uiRet);

    for (loop = 0; loop < (sizeof(studentData))/(sizeof(stStudent)); loop++)
    {
        gStudentList->objAdd(gStudentList, (void*)&studentData[loop], sizeof(stStudent));
    }

    listDataPrint(gStudentList);

    /*
    gStudentList->clearList(gStudentList);
    listDataPrint(gStudentList);
    */
    
    return 0;
}






