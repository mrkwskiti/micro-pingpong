#include <16F886.h>
#device ADC=10 *=16
#FUSES NOWDT                    //No Watch Dog Timer
#FUSES PUT                      //Power Up Timer
#FUSES NOMCLR                   //Master Clear pin not enabled
#FUSES NOPROTECT                //Code not protected from reading
#FUSES NOCPD                    //No EE protection
#FUSES BROWNOUT                 //Brownout reset
#FUSES IESO                     //Internal External Switch Over mode enabled
#FUSES FCMEN                    //Fail-safe clock monitor enabled
#FUSES NOLVP                    //No low voltage prgming, B3(PIC16) or B5(PIC18) used for I/O
#FUSES NODEBUG                  //No Debug mode for ICD
#FUSES NOWRT                    //Program memory not write protected
#FUSES BORV40                   //Brownout reset at 4.0V
#FUSES RESERVED                 //Used to set the reserved FUSE bits
#FUSES INTRC_IO

#include "config.h"
#include "render.h" // render lib
#include "controller.h"

#bit RP0 = STATUS.5
#bit RP1 = STATUS.6

#use delay(clock=4M)

#INT_TIMER1
void timer1_isr() {
   set_timer1(65036); // re-set the timer1 counter to maintain a stable period
   
   counterColumeController();
}


void bounchBall() {

   createPixel(x, y);
   clearBall(x, y);
   
   // calculate next position
   if (x == endX) xVector = -1;
   if (x == startX) xVector = 1;
   x += xVector;
   
   if (y == endY) yVector = -1;
   if (y == startY) yVector = 1;
   y += yVector;

}

void main() {
   setup_timer_1(T1_INTERNAL | T1_DIV_BY_1);
   set_timer1(65036);
   enable_interrupts(INT_TIMER1);
   enable_interrupts(GLOBAL);
   
   initialController();
   initialTRIS();
   initialBoard();

//!   initialLine();
   
   CS1 = 1;
   CS2 = 0;
   
   // wait for intial glcd module
   delay_us(1000);

   // initial players' board
   createBoard(player1, 0);
   createBoard(player2, 15);
   
   while(1) {
      bounchBall();
   }
}
