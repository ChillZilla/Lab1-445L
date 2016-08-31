// fixed.c
// Amanda Akin, Jiacheng Jason He
// Created: August 29, 2016
// Printing fixed point numbers and plotting coordinates on LCD
// Lab 1
// TA: Mahesh Srinivasan
// Last Revised: August 31, 2016

// Hardware Configuration:
// Backlight (pin 10) connected to +3.3 V
// MISO (pin 9) unconnected 
// SCK (pin 8) connected to PA2 (SSI0Clk)
// MOSI (pin 7) connected to PA5 (SSI0Tx)
// TFT_CS (pin 6) connected to PA3 (SSI0Fss)
// CARD_CS (pin 5) unconnected
// Data/Command (pin 4) connected to PA6 (GPIO)
// RESET (pin 3) connected to PA7 (GPIO)
// VCC (pin 2) connected to +3.3 V
// Gnd (pin 1) connected to ground

#include <stdint.h>
#include <stdio.h>
#include "fixed.h"

/****************ST7735_sDecOut3***************
 converts fixed point number to LCD
 format signed 32-bit with resolution 0.001
 range -9.999 to +9.999
 Inputs:  signed 32-bit integer part of fixed-point number
 Outputs: none
 send exactly 6 characters to the LCD 
Parameter LCD display
 12345    " *.***"
  2345    " 2.345"  
 -8100    "-8.100"
  -102    "-0.102" 
    31    " 0.031" 
-12345    " *.***"
 */ 
void ST7735_sDecOut3(int32_t n) {
  char fixedPt[6];
  
  if ((n > 9999) || (n < -9999)) {
    fixedPt[0] = 32;
    fixedPt[1] = 42;
    fixedPt[2] = 46;
    fixedPt[3] = 42;
    fixedPt[4] = 42;
    fixedPt[5] = 42;
    for (int i = 0; i < 6; i++) {
      ST7735_OutChar(fixedPt[i]);
    }
    return;
  }
  
  if (n < 0) {
    fixedPt[0] = 45;
    n = -n;
  } else {
    fixedPt[0] = 32;
  }
  fixedPt[1] = (n/1000) + 48;
  fixedPt[2] = 46;
  fixedPt[3] = ((n%1000)/100) + 48;
  fixedPt[4] = ((n%100)/10) + 48;
  fixedPt[5] =  (n%10) + 48;
	
  for (int i = 0; i < 6; i++) {
    ST7735_OutChar(fixedPt[i]);
  }
}

/**************ST7735_uBinOut8***************
 unsigned 32-bit binary fixed-point with a resolution of 1/256. 
 The full-scale range is from 0 to 999.99. 
 If the integer part is larger than 256000, it signifies an error. 
 The ST7735_uBinOut8 function takes an unsigned 32-bit integer part 
 of the binary fixed-point number and outputs the fixed-point value on the LCD
 Inputs:  unsigned 32-bit integer part of binary fixed-point number
 Outputs: none
 send exactly 6 characters to the LCD 
Parameter LCD display
     0	  "  0.00"
     2	  "  0.01"
    64	  "  0.25"
   100	  "  0.39"
   500	  "  1.95"
   512	  "  2.00"
  5000	  " 19.53"
 30000	  "117.19"
255997	  "999.99"
256000	  "***.**"
*/
void ST7735_uBinOut8(uint32_t n) {
  char fixedPt[6];
	
  if (n >= 256000) {
    fixedPt[0] = 42;
    fixedPt[1] = 42;
    fixedPt[2] = 42;
    fixedPt[3] = 46;
    fixedPt[4] = 42;
    fixedPt[5] = 42;
    for (int i = 0; i < 6; i++) {
      ST7735_OutChar(fixedPt[i]);
    }
    return;
  }
  if (((n>>8)/100) != 0) {
    fixedPt[0] = ((n>>8)/100) + 48;
  } else {
    fixedPt[0] = 32;
  }
	
  if (((n>>8)/10)  != 0) {
    fixedPt[1] = (((n>>8)%100)/10) + 48;
  } else {
    fixedPt[1] = 32;
  }
  fixedPt[2] = ((n>>8)%10) + 48;
  fixedPt[3] = 46;
  fixedPt[4] = (((n%256)*10)/256) + 48;
  fixedPt[5] = (((((n%256)*10)%256)*10)/256) + 48;
  // if the last spot was a 0, this checks the digit after that to see if it needs to round up!
  if (((((((n%256)*10)%256)*10)/256) == 0) && ((((((((n%256)*10)%256)*10)%256)*10)/256) >= 5)) {
    fixedPt[5] = (((((n%256)*10)%256)*10)/256) + 49; // plus 49 is an extra 1 to round up!
  }
	
  for (int i = 0; i < 6; i++) {
    ST7735_OutChar(fixedPt[i]);
  }
}

int32_t XMin = 0;
int32_t XMax = 0;
int32_t YMin = 0;
int32_t YMax = 0;

/**************ST7735_XYplotInit***************
 Specify the X and Y axes for an x-y scatter plot
 Draw the title and clear the plot area
 Inputs:  title  ASCII string to label the plot, null-termination
          minX   smallest X data value allowed, resolution= 0.001
          maxX   largest X data value allowed, resolution= 0.001
          minY   smallest Y data value allowed, resolution= 0.001
          maxY   largest Y data value allowed, resolution= 0.001
 Outputs: none
 assumes minX < maxX, and miny < maxY
*/
void ST7735_XYplotInit(char *title, int32_t minX, int32_t maxX, int32_t minY, int32_t maxY) {
  ST7735_InitR(INITR_REDTAB);
  ST7735_OutString(title);
  //ST7735_PlotClear(0,4095);
  ST7735_FillRect(0, 32, 128, 128, ST7735_Color565(255,165,0));
	
  XMin = minX;
  XMax = maxX;
  YMin = minY;
  YMax = maxY;
}

/**************ST7735_XYplot***************
 Plot an array of (x,y) data
 Inputs:  num    number of data points in the two arrays
          bufX   array of 32-bit fixed-point data, resolution= 0.001
          bufY   array of 32-bit fixed-point data, resolution= 0.001
 Outputs: none
 assumes ST7735_XYplotInit has been previously called
 neglect any points outside the minX maxY minY maxY bounds
*/
void ST7735_XYplot(uint32_t num, int32_t bufX[], int32_t bufY[]) {
  for (int i = 0; i < num; i++) {
    if ((bufX[i] < XMin) || (bufX[i] > XMax) || (bufY[i] < YMin) || (bufY[i] > YMax)) {
      continue;
		}
    int xPt = (127*(bufX[i]-XMin))/(XMax-XMin);
    int yPt = 32+(127*(YMax-bufY[i]))/(YMax-YMin);
    ST7735_DrawPixel(xPt, yPt, ST7735_WHITE);
    ST7735_DrawPixel(xPt+1, yPt, ST7735_WHITE);
    ST7735_DrawPixel(xPt, yPt+1, ST7735_WHITE);
    ST7735_DrawPixel(xPt+1, yPt+1, ST7735_WHITE);
  }
}
