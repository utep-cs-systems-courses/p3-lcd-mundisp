/** \file lcddemo.c
 *  \brief A simple demo that draws a string and square
 */
#include <msp430.h>
#include <libTimer.h>
#include "lcdutils.h"
#include "lcddraw.h"
#include "buzzer.h"
#include "buzzer.c"
//#include "stateMachine.h"

//#include "stateMachine.h"


// WARNING: LCD DISPLAY USES P1.0.  Do not touch!!! 

#define LED BIT6		/* note that bit zero req'd for display */

//#define SWITCHES 15


#define SW1 BIT1
#define SW2 BIT2
#define SW3 BIT3
#define SW4 BIT4
#define SWITCHES (SW1 | SW2 | SW3 | SW4)

void drawTriangle(int limit, int controlRow, int controlCol)
{

  int row = controlRow;
  int col = controlCol;
  int fillCol = limit;
  int fill = limit * 2;
  int fillRow = limit;
  int distance = limit;
  
  //filling upper triangle
   for (int j = 0; j < limit; j++)
   {
     for (int i = 0; i <=fill; i++){
       drawPixel(fillCol, fillRow, COLOR_BLUE);
       fillCol++;
     }
     distance++;
     fillCol = distance;
     fillRow++;
     fill -= 2;
   }

   //readjusting column level and max width of triangle
   fill = limit*2;
   fillCol = limit;
   distance = limit;

   //filling lower triangle
   for (int j = 0; j < limit; j++)
   {
     for (int i = 0; i <=fill; i++){
       drawPixel(fillCol, fillRow, COLOR_PURPLE);
       fillCol++;
     }
     distance++;
     fillCol = distance;
     fillRow++;
     fill -= 2;
   }  
 }

void
moveTriangle(int limit, int row, int col){
  
  drawTriangle(limit, row +1, col+1);
    
}

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


static char
switch_update_interrupt_sense()
{
  char p2val = P2IN;
  /* update switch interrupt to detect changes from current buttons */
  P2IES |= (p2val & SWITCHES);	/* if switch up, sense down */
  P2IES &= (p2val | ~SWITCHES);	/* if switch down, sense up */

  return p2val;
}


void
switch_interrupt_handler()
{
  char p2val = switch_update_interrupt_sense();
  //switches = ~p2val & SWITCHES;
  
  if (p2val & SW1 ? 0: 1){
    state_machine(1);
  }
  if (p2val & SW2 ? 0: 1){
    state_machine(2);
  }
  if (p2val & SW3 ? 0: 1){
    state_machine(3);
  }
}



int redrawScreen = 1;

int switches = 0;

void 
switch_init()			/* setup switch */
{  
  P2REN |= SWITCHES;		/* enables resistors for switches */
  P2IE |= SWITCHES;		/* enable interrupts from switches */
  P2OUT |= SWITCHES;		/* pull-ups for switches */
  P2DIR &= ~SWITCHES;		/* set switches' bits for input */
  switch_update_interrupt_sense();
  switch_interrupt_handler();
}




// Switch on S2 
void
__interrupt_vec(PORT2_VECTOR) Port_2(){
  if (P2IFG & SWITCHES) {	      // did a button cause this interrupt? 
    P2IFG &= ~SWITCHES;		      // clear pending sw interrupts 
    switch_interrupt_handler();	// single handler for all switches
    switch_update_interrupt_sense();
  }
}


/** Initializes everything, clears the screen, draws "hello" and a square */
int
main()
{
  P1DIR |= LED;		// Green led on when CPU on 
  P1OUT |= LED; 
  switch_init();
  enableWDTInterrupts();      //enable periodic interrupt 
  or_sr(0x8);	              //GIE (enable interrupts) 

  configureClocks();
  lcd_init();
  u_char width = screenWidth, height = screenHeight;
  clearScreen(COLOR_ORANGE);

  

  //fillRectangle(30,30, 60, 60, COLOR_ORANGE);
  int controlCol = 61;
  int controlRow = 61;
  int limit = 30;
  int c = 0;
  int r = 0;
  drawTriangle(20, 20, 20);

}




