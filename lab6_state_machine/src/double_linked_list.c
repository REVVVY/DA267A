#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include "double_linked_list.h"

void initDoubleLinkedList(struct doubleLinkedList *list)
{
  list->first = NULL;
  list->last = NULL;
}

int addElementDoubleLinkedList(struct doubleLinkedList *list, int value)
{

  struct doubleLinkedListElement *nextNode = (struct doubleLinkedListElement *)malloc(sizeof(struct doubleLinkedListElement));

  nextNode->data = value;

  nextNode->next = NULL;

  if (list->first == NULL)
  {
    list->first = nextNode;
    list->last = nextNode;
    nextNode->previous = NULL;
    return value;
  }
  else
  {
    nextNode->previous = list->last;
    list->last->next = nextNode;
    list->last = nextNode;
    return value;
  }
  return INT_MIN;
}

int removeFirstElementDoubleLinkedList(struct doubleLinkedList *list)
{
  if (list->first != NULL && list->first->next != NULL)
  {
    struct doubleLinkedListElement *nodeToBeRemoved = list->first;
    list->first = list->first->next;
    list->first->previous = NULL;
    free(nodeToBeRemoved);
    return nodeToBeRemoved->data;
  }
  else if (list->first != NULL && list->first->next == NULL)
  {
    struct doubleLinkedListElement *nodeToBeRemoved = list->first;
    list->first = NULL;
    free(nodeToBeRemoved);
    return nodeToBeRemoved->data;
  }
  else
  {
    return INT_MIN;
  }
}
