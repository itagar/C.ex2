#ifndef _MYLINKEDLIST_H
#define _MYLINKEDLIST_H


/********************************************************************************
 * @file MyString.h
 * @author  slabc <slbac@cs.huji.ac.il>
 * @version 1.0
 * @date 9 Jun 2016
 * 
 * @brief The SLabC Standard Strings library.
 *
 * @section LICENSE
 * This program is not a free software; 
 *
 * @section DESCRIPTION
 * 
 *
 * Error handling
 * ~~~~~~~~~~~~~~
 * Most functions may fail due to failure to allocate dynamic memory. When
 * this happens the functions will return an appropriate failure value. If this
 * happens, then the state of the other outputs of the function is undefined.
 ********************************************************************************/

// ------------------------------ includes ------------------------------

#include <stdbool.h>

///DO NOT include other libraries/code
// -------------------------- const definitions -------------------------

/*
* mystr error code
*/
#define MYLIST_ERROR_CODE -999

/*
	MYLinkedListP is a pointer for a LinkedList 
*/
//struct _MyLinkedList;
typedef struct _MyLinkedList *MyLinkedListP;


// ------------------------------ functions -----------------------------


/**
 * @brief Allocates a new empty LinkedList
 * 			It is the caller's responsibility to free the returned LinkedList.
 *
 * RETURN VALUE:
 * @return a pointer to the new LinkedList, or NULL if the allocation failed.
 */
MyLinkedListP createList();


/**
 * @brief Allocates a new MyLinkedList with the same values as l. It is the caller's
 * 			responsibility to free the returned LinkedList.
 * @param l the MyLinkedListP to clone.
 * RETURN VALUE:
 *   @return a pointer to the new LinkedList, or NULL if the allocation failed.
 */
MyLinkedListP cloneList(MyLinkedListP l);

/**
 * @brief Frees the memory and resources allocated to LinkedList l.
 * @param l the LinkedList to free.
 * If l is NULL, no operation is performed.
 */
void freeList(MyLinkedListP l);

/**
 * @brief print LinkedList l and it's contents- see school solution for the exact format.
 * @param l the LinkedList to print.
 */
void printList(MyLinkedListP l);

/**
 * @brief remove all the ocuurences of val in l 
 * @param l the LinkedList
 * @param val the value - char *
 * RETURN VALUE:
 *   @return number of elements that were removed. or MYLIST_ERROR_CODE if error occured 
 */
int removeData(MyLinkedListP l, char const *val);

/**
 * @brief add val to the beginning of the list(the List may contain duplicates) 
 * UPDATE -- val may be changed /deleted and your list should bot be effected by that. 
 * @param l the LinkedList
 * @param val the value - char *
 * RETURN VALUE:
 *   @return true iff succeed 
 */
bool insertFirst(MyLinkedListP l, char const *val);

/**
 * @brief search val in the list 
 * @param l the LinkedList 
 * @param val the value to add 
 * RETURN VALUE:
 *   @return the number of val ocuurences in the list.  or MYLIST_ERROR_CODE if error occured 
 */
int isInList(MyLinkedListP const l, char const *val);

/**
 * @brief get list size 
 * @param l the LinkedList 
 * RETURN VALUE:
 *   @return number of elements in the list.  or MYLIST_ERROR_CODE if error occured 
 */

int getSize(MyLinkedListP const l);

/**
 * @brief Returns size in bytes of l and all it's contents
 *			eqvuilant to sum of sizeof for all the list contents
 * @param l the LinkedList 
 * RETURN VALUE:
 *   @return the allocated size for l 
 */
int getSizeOf(MyLinkedListP const l);
#endif // MYLINKEDLIST