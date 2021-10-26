#include <stdio.h>
#include <stdlib.h>
#include <esp_system.h>
#include <esp_task_wdt.h>

#include "single_linked_list.h"
#include "double_linked_list.h"

void app_main()
{
    //Single linkedlist
    struct singleLinkedList singleList;
    initSingleLinkedList(&singleList);
    //Double linkedlist
    struct doubleLinkedList doubleList;
    initDoubleLinkedList(&doubleList);
    printf("\n");


    //Testing
    //Test 1
    printf("Test 1 - Add one element and check if added.\n");
    printf("Single LinkedList: \n");
    addElementSingleLinkedList(&singleList, 13);
    printSingleLinkedList(&singleList);

    printf("Double LinkedList: \n");
    addElementDoubleLinkedList(&doubleList, 13);
    printDoubleLinkedList(&doubleList);

    printf("Emptying linkedLists\n");
    removeFirstElementSingleLinkedList(&singleList);
    removeFirstElementDoubleLinkedList(&doubleList);


    //Test 2
    printf("\nTest 2 - Add three element and remove first and last element.\n");
    printf("Single LinkedList: \n\n");
    addElementSingleLinkedList(&singleList, 13);
    addElementSingleLinkedList(&singleList, 5);
    addElementSingleLinkedList(&singleList, 8);
    printSingleLinkedList(&singleList);
    removeFirstElementSingleLinkedList(&singleList);
    removeLastElementSingleLinkedList(&singleList);
    printSingleLinkedList(&singleList);

    printf("Double LinkedList: \n\n");
    addElementDoubleLinkedList(&doubleList, 13);
    addElementDoubleLinkedList(&doubleList, 5);
    addElementDoubleLinkedList(&doubleList, 8);
    printDoubleLinkedList(&doubleList);
    removeFirstElementDoubleLinkedList(&doubleList);
    removeLastElementDoubleLinkedList(&doubleList);
    printDoubleLinkedList(&doubleList);
    
    printf("Emptying linkedLists\n");
    removeFirstElementSingleLinkedList(&singleList);
    removeFirstElementDoubleLinkedList(&doubleList);

    //Test 3
    printf("\nTest 3 - Try to remove element when list is empty.\n");
    printf("\nSingle LinkedList: \n");
    removeFirstElementSingleLinkedList(&singleList);
    removeLastElementSingleLinkedList(&singleList);

    printf("\nDouble LinkedList: \n");
    removeFirstElementDoubleLinkedList(&doubleList);
    removeLastElementDoubleLinkedList(&doubleList);
    
}