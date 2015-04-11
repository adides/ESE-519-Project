/*
 * ESE 519 - Project
 * A Real Time Home Infotainment System
 * Team Members: Aditya Deshpande, Nishank Shinde, Cheng Cheng
 *
*/ 

#include "mbed.h"
#include "MRF24J40.h"
    
MRF24J40 mrf(p11, p12, p13, p14, p21);  //RF transceiver
    
DigitalOut led1(LED1);                  //LED1
DigitalOut led2(LED2);                  //LED2
DigitalOut led3(LED3);                  //LED3
DigitalOut led4(LED4);                  //LED4
  
Timer timer;                            // To send data to the central node after regular intervals
  
AnalogIn ain(p20);  
// Serial port for showing RX data.
Serial pc(USBTX, USBRX); 


  
// Send / receive buffers.
// IMPORTANT: The MRF24J40 is intended as zigbee tranceiver; it tends
// to reject data that doesn't have the right header. So the first 
// 8 bytes in txBuffer look like a valid header. The remaining 120
// bytes can be used for anything you like.
uint8_t txBuffer[128]= {1, 8, 0, 0xA1, 0xB2, 0xC3, 0xD4, 0x00};
uint8_t rxBuffer[128];
uint8_t rxLen;


uint16_t temperature(void){
    uint16_t val = ain.read_u16();
    return val;
}      
int main (void)
{
  
  uint8_t count = 0;
  pc.baud(9600);
  timer.start();
  while(1)
  {
    // Check if any data was received. 
    rxLen = mrf.Receive(rxBuffer, 128);
    if(rxLen) 
    {
      // Toggle LED 1 upon each reception of data.
      led1 = led1^1;
      pc.printf("0x%02X", rxBuffer[8]);
    }
          
    // Each second, send some data.
    if(timer.read_ms() >= 1000){
      timer.reset();
      led2 = led2^1;
      
      txBuffer[8] = (uint8_t)temperature();
      mrf.Send(txBuffer, 9);
    }
  }
  
}
 
