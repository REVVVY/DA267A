#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "circular_buffer.h"

/*
 * Initialize an empty buffer.
 */
void initCircularBuffer(struct circularBuffer *bufferPtr, uint32_t *data, int maxLength)
{
  bufferPtr->data = data;
  for (int i = 0; i < bufferPtr->maxLength; i++)
  {
    bufferPtr->data[i] = UINT32_MAX;
  }
  bufferPtr->head = 0;
  bufferPtr->tail = 0;
  bufferPtr->maxLength = maxLength;
  bufferPtr->filledElements = 0;
}

uint32_t addElement(struct circularBuffer *bufferPtr, uint32_t value)
{
  if (bufferPtr->filledElements == 0) 
  {
    bufferPtr->data[bufferPtr->tail] = value;
    bufferPtr->filledElements++;
  }
  else
  {
    if (bufferPtr->filledElements == bufferPtr->maxLength)
    {
      bufferPtr->tail = bufferPtr->head;        
      bufferPtr->data[bufferPtr->tail] = value;

      if (bufferPtr->head == bufferPtr->maxLength - 1) 
      {
        bufferPtr->head = 0;
      }
      else 
      {
        bufferPtr->head = bufferPtr->head + 1;
      }
    }
    else
    {
      if (bufferPtr->tail == bufferPtr->maxLength - 1) 
      {
        bufferPtr->tail = 0;
        bufferPtr->data[bufferPtr->tail] = value;
      }
      else 
      {
        bufferPtr->tail = bufferPtr->tail + 1;
        bufferPtr->data[bufferPtr->tail] = value;
      }

      bufferPtr->filledElements++;
    }
  }
  return value;
}

uint32_t removeHead(struct circularBuffer *bufferPtr)
{
  uint32_t value;
  if (bufferPtr->filledElements == 0)
  {
    return UINT32_MAX;
  }
  else if (bufferPtr->filledElements == 1)
  {
    value = bufferPtr->data[bufferPtr->head];
    bufferPtr->data[bufferPtr->head] = UINT32_MAX;
    bufferPtr->filledElements = bufferPtr->filledElements - 1;
    return value;
  }
  else
  {
    value = bufferPtr->data[bufferPtr->head];
    bufferPtr->data[bufferPtr->head] = UINT32_MAX;

    if (bufferPtr->head == bufferPtr->maxLength - 1)
    {
      bufferPtr->head = 0;
    }
    else
    {
      bufferPtr->head = bufferPtr->head + 1;
    }
    bufferPtr->filledElements = bufferPtr->filledElements - 1;
    return value;
  }
}
uint8_t isEmpty(struct circularBuffer *bufferptr)
{
  return bufferptr->filledElements == 0;
}