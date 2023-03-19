#include "AdptArray.h"
#include "assert.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


typedef struct AdptArray_
{
	int ArrSize;
	PElement* pElemArr;
	DEL_FUNC delFunc;
	COPY_FUNC copyFunc;
    PRINT_FUNC printFunc;
}AdptArray;

// create a new array with the given size and initialize all elements to NULL
PAdptArray CreateAdptArray(COPY_FUNC copyFunc_, DEL_FUNC delFunc_, PRINT_FUNC printFunc)
{
    // input validation: return NULL if any function pointer is NULL
    if (copyFunc_ == NULL || delFunc_ == NULL || printFunc == NULL) 
    {
        return NULL;  // input validation: return NULL if any function pointer is NULL
    }
    // allocate memory for the array struct
    PAdptArray p_Arr_Mall = calloc(1, sizeof(AdptArray));  // use calloc() instead of malloc()
    if (p_Arr_Mall == NULL) // check if the allocation failed
    {
        return NULL;
    }
    // initialize the array struct:
    p_Arr_Mall->ArrSize = 0;
    p_Arr_Mall->pElemArr = NULL;
    p_Arr_Mall->delFunc = delFunc_;
    p_Arr_Mall->copyFunc = copyFunc_;
    p_Arr_Mall->printFunc = printFunc;

    // check if initialization was successful
    if (p_Arr_Mall->delFunc == NULL || p_Arr_Mall->copyFunc == NULL || p_Arr_Mall->printFunc == NULL)
    {
        free(p_Arr_Mall);
        return NULL;
    }

    return p_Arr_Mall; // return the new array
}


// delete the array and all its elements
// if the array is NULL do nothing
void DeleteAdptArray(PAdptArray pArr)
{
    if (pArr == NULL)
        return;

// free all elements in the array
    size_t i = 0;
while (i < pArr->ArrSize)
{
    if (pArr->pElemArr[i] != NULL)
        pArr->delFunc(pArr->pElemArr[i]);
    i++;
}
    // Free the array itself
    if (pArr->pElemArr != NULL)
    {
        PElement* temp = pArr->pElemArr;
        free(temp);
        pArr->pElemArr = NULL;
    }

    // Free the struct
    free(pArr);
}



// set the element at the index
// if the index is out of range resize the array    
// if the element is NULL do nothing

Result SetAdptArrayAt(PAdptArray pArr, int index, PElement pNewElem)
{
    // input validation: return FAIL if the array is NULL or the index is negative
    if (pArr == NULL || index < 0)
        return FAIL;
    // if the index is out of range resize the array
    if (index >= pArr->ArrSize)
    {
        // resize the array
        int newSize = index + 1;
        PElement* newpElemArr = (PElement*)realloc(pArr->pElemArr, newSize * sizeof(PElement));
        if (newpElemArr == NULL)
            return FAIL;

        // initialize new elements to NULL
        for (int i = pArr->ArrSize; i < newSize; i++)
            newpElemArr[i] = NULL;
        // update the array
        pArr->pElemArr = newpElemArr;
        pArr->ArrSize = newSize;
    }
    // if the element is NULL do nothing
    if (pNewElem == NULL)
        return FAIL;
    // if the element is not NULL delete the old element
    if (pArr->pElemArr[index] != NULL)
    {
        // delete the old element
        pArr->delFunc(pArr->pElemArr[index]);
    }

    // copy the new element to the array
    pArr->pElemArr[index] = pArr->copyFunc(pNewElem);
    if (pArr->pElemArr[index] == NULL)
        return FAIL;

    return SUCCESS;
}


// return the element at the index 
// if the index is out of range or the element is NULL return NULL
// else return a copy of the element
PElement GetAdptArrayAt(PAdptArray pArr, int index)
{
    // input validation: return NULL if the array is NULL or the index is out of range
    if (pArr == NULL)
    {
        return NULL;
    }

    if (index < 0 || index >= pArr->ArrSize)
    {
        printf("Error: index %d is out of range.\n", index);
        return NULL;
    }

    // if the element is NULL return NULL
    PElement pElem = pArr->pElemArr[index];

    if (pElem == NULL)
    {
        printf("Error: element at index %d is NULL.\n", index);
        return NULL;
    }

    // else return a copy of the element
    PElement pCopy = pArr->copyFunc(pElem);

    if (pCopy == NULL)
    {
        printf("Error: failed to copy element at index %d.\n", index);
        return NULL;
    }

    // return the copy
    return pCopy;
}


// return the array size, or -1 if the array is NULL
// get the size of the array
int GetAdptArraySize(PAdptArray pArr)
{
    // input validation: return -1 if the array is NULL
    if (pArr == NULL)
    {
        return -1;
    }
    // return the size of the array
    return pArr->ArrSize;
}

// print the array elements 
void PrintDB(PAdptArray pArr)
{
    // input validation: return if the array is NULL
    if (pArr == NULL)
    {
        return;
    }
size_t i = 0;
while (i < pArr->ArrSize) 
{
    // check if the element is NULL
    if (pArr->pElemArr[i] == NULL)
    {
        i++; // move to the next element
        continue;  // skip this element and move to the next
    }
    // print the element
    if (pArr->printFunc != NULL)
    {
        pArr->printFunc(pArr->pElemArr[i]); 
    }
    printf("\n");
    i++; // move to the next element
}

}


// My name: Maya Rom
// My id: 207485251
// I used a part from the moodle's answer to this assignment

