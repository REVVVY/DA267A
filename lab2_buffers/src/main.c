#include <stdio.h>
#include <stdlib.h>
#include <esp32/rom/ets_sys.h>
#include "circular_buffer.h"

/*
 * Change the value of BUFFER_SIZE if you want to change the size of the buffer. 
 */
#define BUFFER_SIZE 4

void app_main()
{

    struct circularBuffer buffer;
    int *buffer_data = (int *)malloc(BUFFER_SIZE * sizeof(int));
    initCircularBuffer(&buffer, buffer_data, BUFFER_SIZE);
    
     //WHITE BOX TESTING
    printf("\ninitCircularBuffer test\n");
    if (buffer.maxLength == BUFFER_SIZE)
    {
        printf("maxLength == bufferSize\nSucess\n");
    }
    if (buffer.head == 0)
    {
        printf("head == 0\nSucess\n");
    }
    if (buffer.tail == 0)
    {
        printf("tail == 0\nSucess\n");
    }
    


    
    

     //WHITE BOX TESTING END
    
     //BLACK BOX TESTING
    //1
    printf("Test 1\n");
    addElement(&buffer, 13);
    removeHead(&buffer);
    printBuffer(&buffer);
    //2
    printf("\nTest 2\n");
    addElement(&buffer, 13);
    addElement(&buffer, 5);
    removeHead(&buffer);
    printBuffer(&buffer);
    removeHead(&buffer);
    printBuffer(&buffer);

    //3
    printf("\nTest 3\n");
    int testValue3 = 10;
    for (int i = BUFFER_SIZE; i > 0; i--)
    {
        addElement(&buffer, testValue3);
        testValue3 = testValue3 - 2;
        printBuffer(&buffer);
        printf("\n");
    }

    for (int i = BUFFER_SIZE; i > 0; i--)
    {
        removeHead(&buffer);
        printBuffer(&buffer);
        printf("\n");
    }

    //4
    printf("\nTest 4\n");
    for (int i = BUFFER_SIZE + 1; i > 0; i--)
    {
        addElement(&buffer, 13);
    }

    printf("Making buffer empty for next test");
    removeHead(&buffer);
    removeHead(&buffer);
    removeHead(&buffer);
    removeHead(&buffer);

    //5
    printf("\nTest 5\n");
    for (int i = BUFFER_SIZE + 1; i > 0; i--)
    {
        addElement(&buffer, 13);
        removeHead(&buffer);
        printBuffer(&buffer);
        printf("\n");
    }

    //6
    printf("\nTest 6\n");
    int test = contains(&buffer, 13);
    printf("%d\n", test);

    //7
    printf("\nTest 7\n");
    addElement(&buffer, 13);
    contains(&buffer, 13);

    printf("Making buffer empty for next test");
    removeHead(&buffer);

    //8
    printf("\nTest 8\n");
    addElement(&buffer, 13);
    addElement(&buffer, 5);
    contains(&buffer, 5);
    printBuffer(&buffer);

    printf("Making buffer empty for next test");
    removeHead(&buffer);
    removeHead(&buffer);

    //9
    printf("\nTest 9\n");
    int testValue9 = 20;
    for (int i = BUFFER_SIZE; i > 0; i--)
    {
        addElement(&buffer, testValue9);
        testValue9--;
    }
    contains(&buffer, testValue9 + 1);

    //BLACK BOX END 
    

    free(buffer_data);
}