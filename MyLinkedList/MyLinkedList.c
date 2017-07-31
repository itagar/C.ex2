/**
 * @file MyLinkedList.c
 * @author Itai Tagar <itagar>
 * @version 1.3
 * @date 17 Aug 2016
 *
 * @brief A program for a Linked List data structure and support several operations on it.
 *
 * @section LICENSE
 * This program is free to use in every operation system.
 *
 * @section DESCRIPTION
 * A program for a Linked List data structure and support several operations on it.
 * Input:       No particular input.
 * Process:     The program handles the entire process of several operations on a Linked List.
 *              The user is revealed only to a Pointer for a Linked List, and all the Data Structure
 *              and the functions implementation are hidden.
 * Output:      Depends on the specific driver that used these operations.
 */


/*-----=  Includes  =-----*/


#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "MyLinkedList.h"


/*-----=  Definitions  =-----*/


/**
 * @def EMPTY_LIST_SIZE 0
 * @brief A Macro that sets the size number for an empty Linked List.
 */
#define EMPTY_LIST_SIZE 0

/**
 * @def NO_OCCURRENCES 0
 * @brief A Macro that sets the number for no occurrences of a value in a Linked List.
 */
#define NO_OCCURRENCES 0

/**
 * @def EMPTY_LIST_MESSAGE "Empty!\n"
 * @brief A Macro that sets the output message for an empty Linked List when attempting to print it.
 */
#define EMPTY_LIST_MESSAGE "Empty!\n"

/**
 * @def PRINT_NODE_OUTPUT "'%s'->"
 * @brief A Macro that sets the output of a single Node print.
 */
#define PRINT_NODE_OUTPUT "'%s'->"

/**
 * @def END_OF_PRINT_OUTPUT "|| size:%d \n"
 * @brief A Macro that sets the output of the end of a Linked List printing process.
 */
#define END_OF_PRINT_OUTPUT "|| size:%d \n"

/**
 * @def STRING_TERMINATOR_COUNT 1
 * @brief A Macro that sets the number for the string terminator addition.
 */
#define STRING_TERMINATOR_COUNT 1

/**
 * @def STRING_EQUALITY 0
 * @brief A Flag for the result of strings equality.
 */
#define STRING_EQUALITY 0

/**
 * @def NO_ALLOCATE_SIZE 0
 * @brief A Macro that sets the number for the size of no memory allocation.
 */
#define NO_ALLOCATE_SIZE 0


/*-----=  Structs  =-----*/


/**
 * A structure for a single Node in the Linked List.
 * It holds the data it contains and a pointer to the next Node.
 */
typedef struct Node
{
    char *data;
    struct Node *next;
} Node;

/**
 * A structure for a Linked List data structure.
 * It holds the pointer for the first Node, the Head, and a counter for the amount of elements
 * currently in the Linked List.
 */
typedef struct _MyLinkedList
{
    Node *head;
    int size;
} MyLinkedList;


/*-----=  Node Functions  =-----*/


/**
 * @brief Initialize a given pointer to a Node struct with the default values.
 * @param pNode The pointer to the Node to initialize.
 */
static void initializeNode(Node *pNode)
{
    assert(pNode != NULL);
    pNode -> data = NULL;
    pNode -> next = NULL;
    return;
}

/**
 * @brief Frees the memory and resources allocated to the given Node.
 *        If the given Node is NULL, no operation is performed.
 *        The function returns the next Node of the Node we are about to free.
 * @param pNode A pointer to the pointer of the Node to free.
 * @return A pointer to the next Node of the Node we are about to free.
 */
static Node * freeNode(Node *pNode)
{
    Node *pNext = NULL;
    if (pNode != NULL)
    {
        free(pNode -> data);
        pNode -> data = NULL;

        pNext = pNode -> next;
        pNode -> next = NULL;

        free(pNode);
    }
    return pNext;
}

/**
 * @brief Creates a new Node and initialize it with the given data.
 * @param data The data to assign for the new Node.
 * @return A pointer to the new Node that was created and now holding the given data,
 *         or NULL if the allocation failed.
 */
static Node * createNode(char const *data)
{
    assert(data != NULL);

    Node *pNode = NULL;
    pNode = (Node *)malloc(sizeof(Node));

    // If the memory allocation was successful, we set this Node's values.
    if (pNode != NULL)
    {
        initializeNode(pNode);

        // Setting the data value.
        pNode -> data = (char *)malloc(sizeof(char) * (strlen(data) + STRING_TERMINATOR_COUNT));
        if ((pNode -> data) != NULL)
        {
            memcpy(pNode -> data, data, (sizeof(char) * (strlen(data) + STRING_TERMINATOR_COUNT)));
        }
        else
        {
            // Unsuccessful Node creation.
            freeNode(pNode);
            pNode = NULL;
        }

    }
    return pNode;
}

/**
 * @brief Returns the size in bytes of the given Node and all it's contents.
 *		  Equivalent to sum of sizeof for all the Node contents.
 * @param l A pointer to the Node.
 * @return The allocated size for the Node.
 */
int getSizeOfNode(Node * const pNode)
{
    int sizeOf = NO_ALLOCATE_SIZE;
    if (pNode != NULL)
    {
        sizeOf += sizeof(Node);
        sizeOf += sizeof(char) * (strlen(pNode -> data) + STRING_TERMINATOR_COUNT);
    }
    return sizeOf;
}


/*-----=  Linked List Functions  =-----*/


/**
 * @brief Initialize a given pointer to a Linked List struct with the default values.
 * @param pMyLinkedList The pointer to the Linked List to initialize.
 */
static void initializeList(MyLinkedListP pMyLinkedList)
{
    assert(pMyLinkedList != NULL);
    pMyLinkedList -> head = NULL;
    pMyLinkedList -> size = EMPTY_LIST_SIZE;
    return;
}

/**
 * @brief Allocates a new empty Linked List.
 * 		  It is the caller's responsibility to free the returned Linked List.
 * @return A pointer to the new Linked List, or NULL if the allocation failed.
 */
MyLinkedListP createList()
{
    MyLinkedListP pMyLinkedList = NULL;
    pMyLinkedList = (MyLinkedListP)malloc(sizeof(MyLinkedList));
    if (pMyLinkedList != NULL)
    {
        initializeList(pMyLinkedList);
    }
    return pMyLinkedList;
}

/**
 * @brief Helper function for cloneList, which clones each Node recursively to the given
 *        pointer for the clone List.
 * @param pMyLinkedList A pointer for the clone Linked List.
 * @param pNode A pointer to the current Node to clone.
 * @return true iff succeed.
 */
static bool cloneListHelper(MyLinkedListP const pMyLinkedList, Node * const pNode)
{
    if (pNode == NULL)
    {
        return true;
    }
    cloneListHelper(pMyLinkedList, pNode -> next);
    return insertFirst(pMyLinkedList, pNode -> data);
}

/**
 * @brief Allocates a new MyLinkedList with the same values as given Linked List.
 *        It is the caller's responsibility to free the returned Linked List.
 * @param l A pointer to the Linked List to clone.
 * @return A pointer to the new cloned Linked List, or NULL if the allocation failed.
 */
MyLinkedListP cloneList(MyLinkedListP const l)
{
    MyLinkedListP clone = NULL;
    if (l != NULL)
    {
        clone = createList();
        if (clone != NULL)
        {
            if (!(cloneListHelper(clone, l -> head)))
            {
                // Unsuccessful Linked List cloning.
                freeList(clone);
                clone = NULL;
            }
        }
    }
    return clone;
}

/**
 * @brief Frees the memory and resources allocated to the given Linked List.
 *        If the given Linked List is NULL, no operation is performed.
 * @param l A pointer to the Linked List to free.
 */
void freeList(MyLinkedListP l)
{
    if (l != NULL)
    {

        if ((l -> head) != NULL)
        {
            int const numberOfElements = (l -> size);
            assert(numberOfElements >= EMPTY_LIST_SIZE);
            Node *currentNode = l -> head;

            // Free each Node in the Linked List.
            int i = 0;
            while (i < numberOfElements)
            {
                assert(currentNode != NULL);
                currentNode = freeNode(currentNode);  // Current Node is now the next Node.
                i++;
            }

        }

        // Free the Linked List itself.
        free(l);
    }
    return;
}

/**
 * @brief Print the given Linked List and it's contents.
 * @param l A pointer to the Linked List to print.
 */
void printList(MyLinkedListP const l)
{
    if (l != NULL)
    {
        int const numberOfElements = l -> size;
        assert(numberOfElements >= EMPTY_LIST_SIZE);

        // Printing process.
        if (numberOfElements == EMPTY_LIST_SIZE)
        {
            printf(EMPTY_LIST_MESSAGE);
        }
        else
        {
            assert((l -> head) != NULL);
            Node *currentNode = l -> head;
            int i = 0;
            while (i < numberOfElements)
            {
                assert(currentNode != NULL);
                assert((currentNode -> data) != NULL);
                printf(PRINT_NODE_OUTPUT, currentNode -> data);
                currentNode = currentNode -> next;
                i++;
            }
            printf(END_OF_PRINT_OUTPUT, numberOfElements);
        }
    }
    return;
}

/**
 * @brief Remove all the occurrences of val in the given Linked List.
 * @param l A pointer to the Linked List to remove from.
 * @param val The value to remove, a (char *) value type.
 * @return The Number of elements that were removed, or MYLIST_ERROR_CODE if an error occurred.
 */
int removeData(MyLinkedListP l, char const *val)
{
    if ((l == NULL) || (val == NULL))
    {
        return MYLIST_ERROR_CODE;
    }

    int const numberOfElements = l -> size;
    assert(numberOfElements >= EMPTY_LIST_SIZE);

    int numberOfOccurrences = NO_OCCURRENCES;

    if ((l -> head))
    {
        Node *currentNode = l -> head;
        Node *previousNode = NULL;

        int i = 0;
        while (i < numberOfElements)
        {
            assert(currentNode != NULL);
            assert((currentNode -> data) != NULL);

            // If we found a Node to remove.
            if (strcmp((currentNode -> data), val) == STRING_EQUALITY)
            {
                if (previousNode == NULL)  // In case the Node to delete is the Head Node.
                {
                    l -> head = currentNode -> next;
                }
                else
                {
                    previousNode -> next = currentNode -> next;
                }
                (l -> size)--;
                currentNode = freeNode(currentNode);  // Current Node is now next Node.
                numberOfOccurrences++;
            }
            else
            {
                previousNode = currentNode;
                currentNode = currentNode -> next;
            }
            i++;
        }

    }
    return numberOfOccurrences;

}

/**
 * @brief Adds the given value to the beginning of the Linked List
 *        (the Linked List may contain duplicates).
          'val' may be changed/deleted and your Linked List should not be effected by that.
 * @param l A pointer to the LinkedList to add the value to.
 * @param val The value to add, a (char *) value type.
 * @return true iff succeed.
 */
bool insertFirst(MyLinkedListP l, char const *val)
{
    if ((l != NULL) && (val != NULL))
    {
        Node *currentHead = l -> head;
        Node *newHead = createNode(val);
        if (newHead != NULL)
        {
            newHead -> next = currentHead;
            l -> head = newHead;
            (l -> size)++;
            return true;
        }
        freeNode(newHead); // If the action did not succeed.
    }
    return false;
}

/**
 * @brief Search the given value in the Linked List.
 * @param l A pointer to the LinkedList to search in.
 * @param val The value to search for, a (char *) value type.
 * @return The number of occurrences of the given value in the Linked List,
 *         or MYLIST_ERROR_CODE if an error occurred.
 */
int isInList(MyLinkedListP const l, char const *val)
{
    if ((l != NULL) && (val != NULL))
    {
        int numberOfOccurrences = NO_OCCURRENCES;

        Node *currentNode = l -> head;
        while (currentNode != NULL)
        {
            assert((currentNode -> data) != NULL);
            if (strcmp((currentNode -> data), val) == STRING_EQUALITY)
            {
                numberOfOccurrences++;
            }

            currentNode = currentNode -> next;
        }
        return numberOfOccurrences;

    }
    else
    {
        return MYLIST_ERROR_CODE;
    }
}

/**
 * @brief Gets the size of the given Linked List.
 * @param l A pointer to the Linked List.
 * @return The number of elements in the Linked List, or MYLIST_ERROR_CODE if an error occurred.
 */
int getSize(MyLinkedListP const l)
{
    if (l != NULL)
    {
        int const numberOfElements = l -> size;
        assert(numberOfElements >= EMPTY_LIST_SIZE);
        return numberOfElements;
    }
    else
    {
        return MYLIST_ERROR_CODE;
    }
}

/**
 * @brief Returns the size in bytes of the given Linked List and all it's contents.
 *		  Equivalent to sum of sizeof for all the Linked List contents.
 * @param l A pointer to the Linked List.
 * @return The allocated size for to the Linked List.
 */
int getSizeOf(MyLinkedListP const l)
{
    int sizeOf = NO_ALLOCATE_SIZE;
    if (l != NULL)
    {
        sizeOf += sizeof(MyLinkedList);

        if ((l -> head) != NULL)
        {
            int const numberOfElements = (l -> size);
            assert(numberOfElements >= EMPTY_LIST_SIZE);
            Node *currentNode = l -> head;

            // Add the sizeof for each Node in the Linked List.
            int i = 0;
            while (i < numberOfElements)
            {
                assert(currentNode != NULL);
                sizeOf += getSizeOfNode(currentNode);
                currentNode = currentNode -> next;
                i++;
            }
        }
    }
    return sizeOf;
}
