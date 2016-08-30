/*
* ST7735_sDecOut3
* Takes a 32-bit signed number and outputs a fixed-point number to the screen
* Range of valid values: -9999 to 9999
* Values greater than 9999 or less than -9999 will be output as *.****
*
* Inputs: 32-bit signed number
* Outputs: none
*/

void ST7735_sDecOut3(signed num);

/*
* ST7735_uBinOut8
* Takes a 32-bit unsigned number and outputs a fixed-point number to the screen
* Range of valid values: 0 to 255999
* resolution for values is 1/256
* Values greater than 256000 are output as ***.**
*
* Inputs: 32-bit unsigned number
* Outputs: none
*/

void ST7735_uBinOut8(unsigned num);

/*
* ST7735_XYPlotInit
* 
* Outputs the title to the screen, and sets the plot parameters
* clears the screen
*
* Inputs: char* title, ymin, ymax, xmin, xmax
* Outputs: none
*/

void ST7735_XYplotInit(char* title, int xmin, int xmax, int ymin, int ymax);

/*
* ST7735_XYPlot
* 
* Outputs scatter plot points to the screen using the values in the  and y points arrays
*
* Inputs: numPts, xArray[], yArray[]
* Outputs: none
*/

void ST7735_XYplot(int numPts, int xPts[], int yPts[]);



