#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include "single_linked_list.h"

int addElementSingleLinkedList(struct singleLinkedList *list, int value)
{
  struct singleLinkedListElement *nextNode = (struct singleLinkedListElement *)malloc(sizeof(struct singleLinkedListElement));

  struct singleLinkedListElement *old = list->first;

  nextNode->data = value;

  nextNode->next = NULL;

  if (list->first == NULL)
  {
    list->first = nextNode;
    printf("Value added: %d\n\n", nextNode->data);
    return value;
  }
  else if (list->first != NULL)
  {
    while (old->next != NULL)
    {
      old = old->next;
    }
    printf("Value added: %d\n\n", nextNode->data);
    old->next = nextNode;
    bubbleSort(list->first);
    return value;
  }
  return INT_MIN;
}

void bubbleSort(struct singleLinkedListElement *list)
{
  int swapped;
  struct singleLinkedListElement *ptr1;
  struct singleLinkedListElement *lptr = NULL;

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
        dataSwap(ptr1, ptr1->next);
        swapped = 1;
      }
      ptr1 = ptr1->next;
    }
    lptr = ptr1;
  } while (swapped);
}

void dataSwap(struct singleLinkedListElement *firstNode, struct singleLinkedListElement *secondNode)
{
  int temp = firstNode->data;
  firstNode->data = secondNode->data;
  secondNode->data = temp;
}

void initSingleLinkedList(struct singleLinkedList *list)
{
  list->first = NULL;
}

int removeFirstElementSingleLinkedList(struct singleLinkedList *list)
{
  if (list->first != NULL && list->first->next != NULL)
  {
    struct singleLinkedListElement *nodeToBeRemoved = list->first;
    list->first = list->first->next;
    printf("First element was removed, Value: %d \n\n", nodeToBeRemoved->data);
    free(nodeToBeRemoved);
    return nodeToBeRemoved->data;
  }
  else if (list->first != NULL && list->first->next == NULL)
  {
    struct singleLinkedListElement *nodeToBeRemoved = list->first;
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

int removeLastElementSingleLinkedList(struct singleLinkedList *list)
{

  struct singleLinkedListElement *old = list->first;
  int value = 0;

  if (old != NULL && old->next != NULL)
  {
    while (old->next != NULL)
    {
      if (old->next->next == NULL)
      {
        value = old->next->data;
        free(old->next);
        old->next = NULL;
        printf("Last element removed, Value: %d \n\n", value);
        break;
      }
      old = old->next;
    }
    return value;
  }
  else if (old != NULL && old->next == NULL)
  {
    value = old->data;
    free(old);
    list->first = NULL;
    printf("Last element removed, Value: %d \n\n", value);
    return value;
  }
  else
  {
    printf("Trying to remove last element that does not exist\n");
    return INT_MIN;
  }
}

void printSingleLinkedList(struct singleLinkedList *list)
{
  struct singleLinkedListElement *old = list->first;

  if (list->first == NULL)
  {
    printf("Trying to print empty list\n\n");
  }
  else
  {
    if (list->first != NULL)
    {
      printf("| %d Next | ", old->data);
      while (old->next != NULL)
      {
        old = old->next;
        printf("%d Next | ", old->data);
      }
      printf("\n\n");
    }
  }
}
