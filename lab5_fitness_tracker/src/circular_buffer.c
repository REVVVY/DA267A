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
  if (bufferPtr->filledElements == 0) //buffern är tom, lägg till värde men ändra ej head och tail
  {
    bufferPtr->data[bufferPtr->tail] = value;
    bufferPtr->filledElements++;
  }
  else
  {
    if (bufferPtr->filledElements == bufferPtr->maxLength) //om buffern är full
    {
      uint32_t overwritedElement = bufferPtr->data[bufferPtr->head];
      bufferPtr->tail = bufferPtr->head;        //överskriver äldsta elementet därav hamnar
      bufferPtr->data[bufferPtr->tail] = value; //nya värdet på heads plats, och head flyttas

      if (bufferPtr->head == bufferPtr->maxLength - 1) //om head är på max, börja om på 0
      {
        bufferPtr->head = 0;
      }
      else //annars öka med 1
      {
        bufferPtr->head = bufferPtr->head + 1;
      }
    }
    else //om buffern ej är full
    {
      if (bufferPtr->tail == bufferPtr->maxLength - 1) //är tail på max, börja om på 0
      {
        bufferPtr->tail = 0;
        bufferPtr->data[bufferPtr->tail] = value;
      }
      else //tail ej max, öka med 1
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
  if (bufferPtr->filledElements == 0) //buffern är tom
  {
    return UINT32_MAX;
  }
  else if (bufferPtr->filledElements == 1) //finns endast 1 element står head och tail på samma
  {
    value = bufferPtr->data[bufferPtr->head];
    bufferPtr->data[bufferPtr->head] = UINT32_MAX;
    bufferPtr->filledElements = bufferPtr->filledElements - 1;
    return value;
  }
  else //finns fler element, skall head flyttas till näst äldsta
  {    //element
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
uint8_t isEmpty(struct circularBuffer *bufferptr){
  return bufferptr->filledElements==0;
}