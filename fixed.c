#include <stdio.h>
#include "ST7735.h"


void ST7735_sDecOut3(int num) {
	//output a decimal number from -9999 to 9999
	//print the number out first
	//printf("%d:", num);
	if(num > 9999 || num < -9999){
	   //output the *.***
		printf("*.***");
		return;
	}

	int numArray[4];
	numArray[0] = num/1000;
	numArray[1] = (num%1000)/100;
	numArray[2] = (num%100)/10;
	numArray[3] = num%10;
	
	if(num < 0){
		printf("-%d.%d%d%d", -1*numArray[0], -1*numArray[1], -1*numArray[2], -1*numArray[3]);
	}
	else {
		printf("%d.%d%d%d", numArray[0], numArray[1], numArray[2], numArray[3]);
	}
	
}
void ST7735_uBinOut8(unsigned num) {
     //unsigned so always positive
	//  printf("%d:", num);
	  if(num >= 256000) {
			//output the *.***
			printf("***.**");
			return;
		}
	  else {
			
			int dec = num%256 *100 /256;
			if(dec < 10){
				printf("%3d.%d%d", num/256, 0, dec);
			}
		//need to divide the numer by 256 for resolution. 
			// num/256 will give me the part before the decimal (whole number)
			// num%256 will give me the part after the decimal, but need to display as 0.** 
			else{
				printf("%3d.%2d", num/256, dec);
			}
			//num%256 = remainder. remainder * 100 / 256 = decimal part of the solution
			return;
			
		}
}

int yMin = 0;
int yMax = 0;
int xMin = 0;
int xMax = 0;

void ST7735_XYplotInit(char *title, int xmin, int xmax, int ymin, int ymax){
	yMin = ymin;
	yMax = ymax;
	xMin = xmin;
	xMax = xmax;
	ST7735_FillScreen(ST7735_WHITE);
	//ST7735_PlotClear(0, 59);
	
	//configure the plot and clear the area
	//output the title
	ST7735_SetCursor(0, 15);
	ST7735_OutString(title);
	//clear the screen and set the y boundaries
	//what do we do with the x min/max values? 
	
	//fill rectangle here
	
	//ST7735_FillRect(0, 0,59, 127, ST7735_WHITE );
	
	return;
	
}
void ST7735_XYplot(unsigned arrayPTS, signed xPts[], signed yPts[]){
  ST7735_SetCursor(0,0);
	for(int j=0;j<arrayPTS;j++){
		
   int xPT = (127*(xPts[j]-xMin))/(xMax-xMin);  
   // y=MaxY maps to j=32
   // y=MinY maps to j=159
   int yPT = (127*(yMax-yPts[j]))/(yMax-yMin);
   ST7735_DrawPixel(xPT,   yPT,   ST7735_BLUE);

  }   
	return;
	
}
