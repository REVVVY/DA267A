#include "random.h"
#include "esp_system.h"

/* 
Returns a random number contained between min and max.min: 
minimum number of msmax: 
maximum number of ms
*/

int getRandommsecs(int min, int max)
{
    float randomNr = (float) esp_random() / __UINT32_MAX__;   
    return min + randomNr * (max - min);
}