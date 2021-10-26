#include <stdio.h>
#include <limits.h>
#include "circular_buffer.h"
#include <stdint.h>

void initCircularBuffer(struct circularBuffer *bufferPtr, u_int32_t *data, int maxLength)
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
 * This function should add the value specified by the 'value' 
 * argument at the tail of the buffer.
 *
 * The function should return:
 *  - 'value' if the value was successfully added to the queue.
 *  - INT_MIN (defined in limits.h) if the value was not added.
 */
uint32_t addElement(struct circularBuffer *bufferPtr, uint32_t value)
{
  if (bufferPtr->tail == bufferPtr->head && bufferPtr->data[bufferPtr->tail] == INT_MIN)
  {
    bufferPtr->data[bufferPtr->tail] = value;
    printf("%d läggs till\n", value);
    return (uint32_t) value;
  }
  else if (bufferPtr->tail == bufferPtr->head && bufferPtr->data[bufferPtr->tail] != INT_MIN)
  {
    if (bufferPtr->tail == 0)
    {
      bufferPtr->tail = bufferPtr->maxLength - 1;
      bufferPtr->data[bufferPtr->tail] = value;
      printf("%d läggs till\n", value);
      return (uint32_t) value;
    }
    else
    {
      bufferPtr->tail = bufferPtr->tail - 1;
      bufferPtr->data[bufferPtr->tail] = value;
      printf("%d läggs till\n", value);
      return (uint32_t) value;
    }
  }

  else if (bufferPtr->tail == 0 && bufferPtr->maxLength - 1 != bufferPtr->head)
  {
    bufferPtr->tail = bufferPtr->maxLength - 1;
    bufferPtr->data[bufferPtr->tail] = value;
    printf("%d läggs till\n", value);
    return (uint32_t) value;
  }
  else if (bufferPtr->tail - 1 != bufferPtr->head)
  {
    bufferPtr->tail = bufferPtr->tail - 1;
    bufferPtr->data[bufferPtr->tail] = value;
    printf("%d läggs till\n", value);
    return (uint32_t) value;
  }
  else
  {
    printf("%d läggs ej till lista full\n", value);
    return INT_MIN;
  }
}

/* 
 * Remove the oldest element, which is at the head of the queue. 
 * 
 * The function should return:
 *   - 'value' if the head element was successfully removed
 *   - INT_MIN (defined in limits.h) if no element was removed (i.e., the
 *     queue was empty when the function was called.       
 */
uint32_t removeHead(struct circularBuffer *bufferPtr)
{
  if (bufferPtr->head == bufferPtr->tail && bufferPtr->data[bufferPtr->head] != INT_MIN)
  {
    printf("%d rensades\n", bufferPtr->data[bufferPtr->head]);
    int value = bufferPtr->data[bufferPtr->head];
    bufferPtr->data[bufferPtr->head] = INT_MIN;
    return (uint32_t) value;
  }
  else if (bufferPtr->head == 0 && bufferPtr->tail != bufferPtr->maxLength - 1)
  {
    printf("%d rensades\n", bufferPtr->data[bufferPtr->head]);
    int value = bufferPtr->data[bufferPtr->head];
    bufferPtr->data[bufferPtr->head] = INT_MIN;
    bufferPtr->head = bufferPtr->maxLength - 1;
    return (uint32_t) value;
  }
   else if (bufferPtr->head == 0 && bufferPtr->tail == bufferPtr->maxLength - 1)
  {
    printf("%d rensades\n", bufferPtr->data[bufferPtr->head]);
    int value = bufferPtr->data[bufferPtr->head];
    bufferPtr->data[bufferPtr->head] = INT_MIN;
    bufferPtr->head = bufferPtr->maxLength - 1;
    return (uint32_t) value;
  }
  else if (bufferPtr->head != 0 && bufferPtr->head != bufferPtr->tail)
  {
    printf("%d rensades\n", bufferPtr->data[bufferPtr->head]);
    int value = bufferPtr->data[bufferPtr->head];
    bufferPtr->data[bufferPtr->head] = INT_MIN;
    bufferPtr->head = bufferPtr->head - 1;
    return (uint32_t) value;
  }
  else if (bufferPtr->tail == bufferPtr->head)
  {
    printf("%d rensades\n", bufferPtr->data[bufferPtr->head]);
    int value = bufferPtr->data[bufferPtr->head];
    bufferPtr->data[bufferPtr->head] = INT_MIN;
    return (uint32_t) value;
  }
  else
  {
    return (uint32_t)INT16_MIN;
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
