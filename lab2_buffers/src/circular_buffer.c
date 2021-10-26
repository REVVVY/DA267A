#include <stdio.h>
#include <limits.h>
#include "circular_buffer.h"

void initCircularBuffer(struct circularBuffer *bufferPtr, int *data, int maxLength)
{
  bufferPtr->data = data;
  bufferPtr->head = 0;
  bufferPtr->tail = 0;
  bufferPtr->maxLength = maxLength;

  int lastPos = maxLength - 1;
  int len = maxLength;
  while (len > 0)
  {
    bufferPtr->data[lastPos] = INT_MIN;
    lastPos--;
    len--;
  }
}

/* 
 * This function should check if the buffer pointed to by bufferPtr
 * contains one or more elements with the value specified by the 
 * 'value' argument.
 * 
 * The function should return:
 *  - 'value' if the an element with the argument value was found in the queue.
 *  - INT_MIN (defined in limits.h) if value was not found.
 */
int contains(struct circularBuffer *bufferPtr, int value)
{
  int length = bufferPtr->maxLength;
  int pos = bufferPtr->maxLength - 1;
  int count = 0;

  while (length > 0)
  {
    if (bufferPtr->data[pos] == value)
    {
      count = count + 1;
    }
    pos--;
    length--;
  }

  printf("%d är med %d gånger i buffern\n", value, count);
  if (count > 0)
  {
    return value;
  }
  else
  {
    return INT_MIN;
  }
}

/*
 * This function should add the value specified by the 'value' 
 * argument at the tail of the buffer.
 *
 * The function should return:
 *  - 'value' if the value was successfully added to the queue.
 *  - INT_MIN (defined in limits.h) if the value was not added.
 */
int addElement(struct circularBuffer *bufferPtr, int value)
{
  if (bufferPtr->tail == bufferPtr->head && bufferPtr->data[bufferPtr->tail] == INT_MIN)
  {
    bufferPtr->data[bufferPtr->tail] = value;
    printf("%d läggs till\n", value);
    return value;
  }
  else if (bufferPtr->tail == bufferPtr->head && bufferPtr->data[bufferPtr->tail] != INT_MIN)
  {
    if (bufferPtr->tail == 0)
    {
      bufferPtr->tail = bufferPtr->maxLength - 1;
      bufferPtr->data[bufferPtr->tail] = value;
      printf("%d läggs till\n", value);
      return value;
    }
    else
    {
      bufferPtr->tail = bufferPtr->tail - 1;
      bufferPtr->data[bufferPtr->tail] = value;
      printf("%d läggs till\n", value);
      return value;
    }
  }

  else if (bufferPtr->tail == 0 && bufferPtr->maxLength - 1 != bufferPtr->head)
  {
    bufferPtr->tail = bufferPtr->maxLength - 1;
    bufferPtr->data[bufferPtr->tail] = value;
    printf("%d läggs till\n", value);
    return value;
  }
  else if (bufferPtr->tail - 1 != bufferPtr->head)
  {
    bufferPtr->tail = bufferPtr->tail - 1;
    bufferPtr->data[bufferPtr->tail] = value;
    printf("%d läggs till\n", value);
    return value;
  }
  else
  {
    printf("%d läggs ej till lista full\n", value);
    return INT_MIN;
  }
}

/* 
 * This function should remove all elements in the buffer whose
 * value matches the 'value' argument.
 * 
 * The function should return:
 *  The argument value: if at least one element was removed from queue.
 *  INT_MIN (defined in limits.h) if no element was removed. 
 * 
 */
int removeValue(struct circularBuffer *bufferPtr, int value)
{
  int length = bufferPtr->maxLength;
  int pos = bufferPtr->maxLength - 1;
  int elements = 0;

  //Find length for temp array
  while (length > 0)
  {
    if (bufferPtr->data[pos] != INT_MIN && bufferPtr->data[pos] != value)
    {
      elements++;
    }
    pos--;
    length--;
  }
  int temp[elements];

  //Temp array
  length = bufferPtr->maxLength;
  pos = bufferPtr->head;
  int arrayIndex = 0;
  while (length > 0)
  {
    if (pos == 0 && bufferPtr->data[pos] != value && bufferPtr->data[pos] != INT_MIN)
    {
      temp[arrayIndex] = bufferPtr->data[pos];
      pos = bufferPtr->maxLength - 1;
      arrayIndex++;
    }
    else if (bufferPtr->data[pos] != value && bufferPtr->data[pos] != INT_MIN)
    {
      temp[arrayIndex] = bufferPtr->data[pos];
      pos--;
      arrayIndex++;
    }
    else
    {
      pos--;
    }
    length--;
  }

  length = bufferPtr->maxLength;
  arrayIndex = bufferPtr->maxLength - 1;
  while (length > 0)
  {
    bufferPtr->data[arrayIndex] = INT_MIN;
    arrayIndex--;
    length--;
  }

  length = sizeof(temp) / sizeof(temp[0]);
  pos = bufferPtr->head;
  arrayIndex = 0;
  while (length > 0)
  {
    if (pos == 0)
    {
      bufferPtr->data[pos] = temp[arrayIndex];
      pos = bufferPtr->maxLength - 1;
      arrayIndex++;
    }
    else
    {
      bufferPtr->data[pos] = temp[arrayIndex];
      pos--;
      arrayIndex++;
    }
    length--;
  }
  bufferPtr->tail = pos + 1;

  return value;
}

/* 
 * Remove the oldest element, which is at the head of the queue. 
 * 
 * The function should return:
 *   - 'value' if the head element was successfully removed
 *   - INT_MIN (defined in limits.h) if no element was removed (i.e., the
 *     queue was empty when the function was called.       
 */
int removeHead(struct circularBuffer *bufferPtr)
{
  if (bufferPtr->head == bufferPtr->tail && bufferPtr->data[bufferPtr->head] != INT_MIN)
  {
    printf("%d rensades\n", bufferPtr->data[bufferPtr->head]);
    int value = bufferPtr->data[bufferPtr->head];
    bufferPtr->data[bufferPtr->head] = INT_MIN;
    return value;
  }
  else if (bufferPtr->head == 0 && bufferPtr->tail != bufferPtr->maxLength - 1)
  {
    printf("%d rensades\n", bufferPtr->data[bufferPtr->head]);
    int value = bufferPtr->data[bufferPtr->head];
    bufferPtr->data[bufferPtr->head] = INT_MIN;
    bufferPtr->head = bufferPtr->maxLength - 1;
    return value;
  }
   else if (bufferPtr->head == 0 && bufferPtr->tail == bufferPtr->maxLength - 1)
  {
    printf("%d rensades\n", bufferPtr->data[bufferPtr->head]);
    int value = bufferPtr->data[bufferPtr->head];
    bufferPtr->data[bufferPtr->head] = INT_MIN;
    bufferPtr->head = bufferPtr->maxLength - 1;
    return value;
  }
  else if (bufferPtr->head != 0 && bufferPtr->head != bufferPtr->tail)
  {
    printf("%d rensades\n", bufferPtr->data[bufferPtr->head]);
    int value = bufferPtr->data[bufferPtr->head];
    bufferPtr->data[bufferPtr->head] = INT_MIN;
    bufferPtr->head = bufferPtr->head - 1;
    return value;
  }
  else if (bufferPtr->tail == bufferPtr->head)
  {
    printf("%d rensades\n", bufferPtr->data[bufferPtr->head]);
    int value = bufferPtr->data[bufferPtr->head];
    bufferPtr->data[bufferPtr->head] = INT_MIN;
    return value;
  }
  else
  {
    return INT_MIN;
  }
}

/* 
 * Print the elements in the buffer from head to tail. 
 */
void printBuffer(struct circularBuffer *bufferPtr)
{
  int length = bufferPtr->maxLength;
  int lastPos = bufferPtr->maxLength - 1;

  while (length > 0)
  {
    if (lastPos != -1 && bufferPtr->data[lastPos] != INT_MIN)
    {
      printf("Pos %d har value %d\n", lastPos, bufferPtr->data[lastPos]);
    }
    lastPos--;
    length--;
  }
  printf("Head: %d Tail: %d\n", bufferPtr->head, bufferPtr->tail);
}
