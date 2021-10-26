#include <stdio.h>

/* Define some states */
//#define STATE_IDLE 0
//#define STATE_MOVING 1
//#define STATE_LOADING 2
//#define STATE_UNLOADING 3

/* Define some position */
#define POSITION_LOWER_LEVEL 0
#define POSITION_UPPER_LEVEL 1
#define POSITION_MOVING_DOWN 2
#define POSITION_MOVING_UP 3

/* Define som events that are used to trigger the machine (i.e. the elevator)*/
#define EVENT_BUTTON_PUSH_LOWER 0
#define EVENT_BUTTON_PUSH_UPPER 1
#define EVENT_DONE_LOADING 2
#define EVENT_DONE_UNLOADING 3
#define EVENT_ARRIVE 4

/* We need some state function prototypes. */
void idle(int event);
void moving(int event);
void loading(int event);
void unloading(int event);

/* Function ptr*/
void (*stateFuncPtr)(int);

/* 
 * Since we are using a function pointer that points to the current state, we do not
 * need to keep track of the current state.
 */
//int currentState;

/* 
 * Since position is not captured in the states of the elevator, we need to keep track of 
 * where the elevator is located
 */
int currentPosition;

int main()
{

  int terminalInput;

  /* Initialize machine */
  currentPosition = POSITION_LOWER_LEVEL;
  stateFuncPtr = idle;

  /* 
   * We are actually simulating the elevator in a loop using terminal input.
   * We enter events, and based on the entered events, the elevator changes 
   * transits between states.
   *
   * In each iteration of the while loop, we first print some information about the 
   * current state of the elevator. 
   * Then the user is prompted for input. 
   */
  while (1)
  {

    if (stateFuncPtr == idle)
    {
      printf("\nCurrent state: IDLE\n");
      if (currentPosition == POSITION_LOWER_LEVEL)
      {
        printf("Current position: LOWER LEVEL\n");
      }
      else if (currentPosition == POSITION_UPPER_LEVEL)
      {
        printf("Current position: UPPER LEVEL\n");
      }
    }

    else if (stateFuncPtr == moving)
    {
      printf("Current state: MOVING\n");

      if (currentPosition == POSITION_MOVING_UP)
      {
        printf("Current position: MOVING UP\n");
      }

      else if (currentPosition == POSITION_MOVING_DOWN)
      {
        printf("Current position: MOVING DOWN\n");
      }
    }

    else if (stateFuncPtr == loading)
    {
      printf("Current state: LOADING\n");

      if (currentPosition == POSITION_LOWER_LEVEL)
      {
        printf("Current position: LOWER LEVEL\n");
      }
      else if (currentPosition == POSITION_UPPER_LEVEL)
      {
        printf("Current position: UPPER LEVEL\n");
      }
    }

    else if (stateFuncPtr == unloading)
    {
      printf("Current state: UNLOADING\n");

      if (currentPosition == POSITION_LOWER_LEVEL)
      {
        printf("Current position: LOWER LEVEL\n");
      }
      else if (currentPosition == POSITION_UPPER_LEVEL)
      {
        printf("Current position: UPPER LEVEL\n");
      }
    }

    printf("Available events:\n");
    printf("  Button pushed on lower level (%d)\n", EVENT_BUTTON_PUSH_LOWER);
    printf("  Button pushed on upper level (%d)\n", EVENT_BUTTON_PUSH_UPPER);
    printf("  Done loading (%d)\n", EVENT_DONE_LOADING);
    printf("  Done unloading (%d)\n", EVENT_DONE_UNLOADING);
    printf("  Arrive (%d)\n", EVENT_ARRIVE);

    printf("Choose event: ");
    scanf("%d", &terminalInput);
    printf("\n");

    /* 
     * Call the function of the current event. Depending on the combination of current state
     * and event, the machine will move between different states.
     */
    stateFuncPtr(terminalInput);

    printf("\n\n\n");
    getchar();
  }

} /* End main() */

/* 
 * This funtion is called if the machine is in idle state.
 *
 * It reacts to button push events.
 */
void idle(int event)
{

  if (event == EVENT_BUTTON_PUSH_LOWER)
  {
    if (currentPosition == POSITION_LOWER_LEVEL)
    {

      printf("Transition from IDLE to LOADING\n");
      stateFuncPtr = loading;
    }

    else if (currentPosition == POSITION_UPPER_LEVEL)
    {
      printf("Transition from IDLE to MOVING\n");

      stateFuncPtr = moving;
      currentPosition = POSITION_MOVING_DOWN;
    }
  }

  else if (event == EVENT_BUTTON_PUSH_UPPER)
  {
    if (currentPosition == POSITION_UPPER_LEVEL)
    {
      stateFuncPtr = loading;
      printf("Transition from IDLE to LOADING\n");
    }

    else if (currentPosition == POSITION_LOWER_LEVEL)
    {
      stateFuncPtr = moving;
      currentPosition = POSITION_MOVING_UP;
      printf("Transition from IDLE to MOVING\n");
    }
  }

} /* End idle()  */

/* 
 * This funtion is called if the machine is in moving state.
 *
 * It reacts to arrive event.
 */
void moving(int event)
{

  if (event == EVENT_ARRIVE)
  {
    if (currentPosition == POSITION_MOVING_DOWN)
    {
      currentPosition = POSITION_LOWER_LEVEL;
    }

    else if (currentPosition == POSITION_MOVING_UP)
    {
      currentPosition = POSITION_UPPER_LEVEL;
    }

    stateFuncPtr = unloading;

    printf("Transition from MOVING to UNLOADING %d\n", currentPosition);
  }
} /* End moving */

/* 
 * This funtion is called if the machine is in loading state.
 *
 * It reacts to loading done event.
 */
void loading(int event)
{

  if (event == EVENT_DONE_LOADING)
  {
    if (currentPosition == POSITION_LOWER_LEVEL)
    {
      currentPosition = POSITION_MOVING_UP;
    }

    else if (currentPosition == POSITION_UPPER_LEVEL)
    {
      currentPosition = POSITION_MOVING_DOWN;
    }

    stateFuncPtr = moving;
    printf("Transition from LOADING to MOVING\n");
  }

} /* End loading */

/* 
 * This funtion is called if the machine is in unloading state.
 *
 * It reacts to unloading done event.
 */
void unloading(int event)
{

  if (event == EVENT_DONE_UNLOADING)
  {
    stateFuncPtr = idle;
    printf("Transition from UNLOADING to IDLE\n");
  }

} /* End unloading */
