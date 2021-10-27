#include <stdint.h>
#ifndef CIRCULAR_BUFFER_H
#define CIRCULAR_BUFFER_H


/* 
 * Data structure used to hold a circular buffer.
 */
struct circularBuffer{
  uint32_t * data;
  int head;
  int tail;
  int maxLength;
  int filledElements;
};

void initCircularBuffer(struct circularBuffer* bufferPtr, uint32_t* data, int maxLen);

uint32_t addElement(struct circularBuffer* bufferPtr, uint32_t);

uint32_t removeHead(struct circularBuffer* bufferPtr);

uint8_t isEmpty(struct circularBuffer *bufferptr);
#endif