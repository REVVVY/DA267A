#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include "double_linked_list.h"

int addElementDoubleLinkedList(struct doubleLinkedList *list, int value)
{

  struct doubleLinkedListElement *nextNode = (struct doubleLinkedListElement *)malloc(sizeof(struct doubleLinkedListElement));

  nextNode->data = value;

  nextNode->next = NULL;

  if (list->first == NULL)
  {
    list->first = nextNode;
    list->last = nextNode;
    printf("Value added: %d\n\n", nextNode->data);
    nextNode->previous = NULL;
    return value;
  }
  else
  {
    nextNode->previous = list->last;
    list->last->next = nextNode;
    list->last = nextNode;
    printf("Value added: %d\n\n", nextNode->data);
    doubleBubbleSort(list->first); //SORTERA
    return value;
  }
  return INT_MIN;
}

void doubleBubbleSort(struct doubleLinkedListElement *list)
{
  int swapped;
  struct doubleLinkedListElement *ptr1;
  struct doubleLinkedListElement *lptr = NULL;

  if (list == NULL)
  {
    return;
  }

  do
  {
    swapped = 0;
    ptr1 = list;

    while (ptr1->next != lptr)
    {
      if (ptr1->data > ptr1->next->data)
      {
        doubleDataSwap(ptr1, ptr1->next);
        swapped = 1;
      }
      ptr1 = ptr1->next;
    }
    lptr = ptr1;
  } while (swapped);
}

void doubleDataSwap(struct doubleLinkedListElement *firstNode, struct doubleLinkedListElement *secondNode)
{
  int temp = firstNode->data;
  firstNode->data = secondNode->data;
  secondNode->data = temp;
}

void initDoubleLinkedList(struct doubleLinkedList *list)
{
  list->first = NULL;
  list->last = NULL;
}

int removeFirstElementDoubleLinkedList(struct doubleLinkedList *list)
{
  if (list->first != NULL && list->first->next != NULL)
  {
    struct doubleLinkedListElement *nodeToBeRemoved = list->first;
    list->first = list->first->next;
    list->first->previous = NULL;
    printf("First element was removed, Value: %d \n\n", nodeToBeRemoved->data);
    free(nodeToBeRemoved);
    return nodeToBeRemoved->data;
  }
  else if (list->first != NULL && list->first->next == NULL)
  {
    struct doubleLinkedListElement *nodeToBeRemoved = list->first;
    list->first = NULL;
    printf("First element was removed, Value: %d \n\n", nodeToBeRemoved->data);
    free(nodeToBeRemoved);
    return nodeToBeRemoved->data;
  }
  else
  {
    printf("Trying to remove first element that does not exist\n");
    return INT_MIN;
  }
}

int removeLastElementDoubleLinkedList(struct doubleLinkedList *list)
{
  if (list->first != NULL)
  {
    int data = list->last->data;
    printf("Last element removed, Value: %d \n\n", data);
    list->last->previous->next = NULL;
    free(list->last);
  }
  else
  {
    printf("Trying to remove last element that does not exist\n");
  }
  return INT_MIN;
}

void printDoubleLinkedList(struct doubleLinkedList *list)
{
  struct doubleLinkedListElement *old = list->first;

  if (list->first == NULL)
  {
    printf("Trying to print empty list\n");
  }
  else
  {
    printf("| Prev: NULL, Data: %d Next | ", old->data);

    while (old->next != NULL)
    {
      old = old->next;
      printf("Prev: %d, Data: %d, Next | ", old->previous->data, old->data);
    }
  }
  printf("\n\n");
}
