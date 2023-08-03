#include <msp430.h>
#include "stateMachine.h"
#include "lcddemo.h"


void
state_machine(int state)
{ 
    switch (state)
    {
    case 2:
      clearScreen(COLOR_BLACK);
      drawTriangle(40,40,40);
      state = 0;
      break;

    case 3:
      enableWDTInterrupts();
      clearScreen(COLOR_STEEL_BLUE);
      buzzer_init();
      generateSound(6000);
      drawString5x7(20,40, "Welcome to my", COLOR_WHITE, COLOR_STEEL_BLUE);
      drawString5x7(20,60, "project 3!!", COLOR_WHITE, COLOR_STEEL_BLUE);
      state =0;
      break;
      
    case 1:
      clearScreen(COLOR_CYAN);
      moveTriangle(40,40,40);
      state = 0;
      break;
    default:
      break;
    }

}

