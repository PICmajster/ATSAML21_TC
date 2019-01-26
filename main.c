/***************************************************************************
  FileName:        main.c
  Processor:       ATSAML21G18B
  IDE :            SEGGER ver 4.12
  Created by:      http://strefapic.blogspot.com
 ***************************************************************************/
/*-------------------------------------------------------------------------*/
/* TC0 configuration and blink LED                                         */
/*-------------------------------------------------------------------------*/
//      
//    Reqired connections :
//     - LED   - PB11
//
//***************************************************************************
#include <stdio.h>
#include <stdlib.h>
#include <saml21.h>

volatile uint8_t Timer1_Soft , Timer2_Soft ;

void main(void) {
/* Initialize the SAM system */
SystemInit();
/* Set pin PB11 direction to Output */
PORT->Group[1].DIRSET.reg |= PORT_PB11; 
/*enable NVIC interrupt for TC0 module*/
NVIC_EnableIRQ(TC0_IRQn);
/*Initialize the TC0*/
GCLK->PCHCTRL[27].bit.CHEN = 1; /*enable the channel 27 for GCLK_TC0 clock*/
TC0->COUNT16.CC[0].bit.CC = 40000; /*load value for compare*/
TC0->COUNT16.COUNT.reg = 0x0000; /*clear counter*/
TC0->COUNT16.INTENSET.bit.MC0 = 1; /*enable interrupt for Compare mode*/
//TC0->COUNT16.WAVE.bit.WAVEGEN = 0x1; /*clear counter if compare match*/
TC0->COUNT16.CTRLA.bit.ENABLE = 1; /*start TC0*/
while(TC0->COUNT16.SYNCBUSY.bit.ENABLE == 1){} /*wait for TC0 synchro*/


   while (1)
   {
     if(!Timer1_Soft) {
              Timer1_Soft = 50 ; /*TC0 hardware x Timer1_soft = 10ms x 50 = 500 ms*/
              PORT->Group[1].OUTTGL.reg |= PORT_PB11; /*Toggle pin PB11*/
          }
   }

}

/*Routime Interrupt*/
 void TC0_IRQHandler(void){
 
 if ( TC0->COUNT16.INTFLAG.bit.MC0 == 1 )
    {
        uint8_t x;
        x = Timer1_Soft ;
        if (x) Timer1_Soft = --x ;
        x = Timer2_Soft ;
        if (x) Timer2_Soft = --x ;
        
        TC0->COUNT16.COUNT.reg = 0x0000; /*clear counter*/
    }

  TC0->COUNT16.INTFLAG.reg = 0xFF; /*clear all interrupt flags*/
}