
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <shapes.h>
#include <fontinfo.h>
#include <math.h>
#include "scope.h"


typedef struct{
  VGfloat x, y;
} data_point;

// Draw grid lines
void grid(VGfloat x, VGfloat y, // Coordinates of lower left corner
	  int nx, int ny, // Number of x and y divisions
	  int w, int h) // screen width and height

{
  VGfloat ix, iy;
  Stroke(128, 128, 128, 0.5); // Set color
  StrokeWidth(2); // Set width of lines
  for (ix = x; ix <= x + w; ix += w/nx) {
    Line(ix, y, ix, y + h);
  }

  for (iy = y; iy <= y + h; iy += h/ny) {
    Line(x, iy, x + w, iy);
  }
}

// Draw the background for the oscilloscope screen
void drawBackground(int w, int h, // width and height of screen
		    int xdiv, int ydiv,// Number of x and y divisions
		    int margin){ // Margin around the image
  VGfloat x1 = margin;
  VGfloat y1 = margin;
  VGfloat x2 = w - 2*margin;
  VGfloat y2 = h - 2*margin;

  Background(128, 128, 128);

  Stroke(204, 204, 204, 1);
  StrokeWidth(1);
  
  Fill(44, 77, 120, 1);
  Rect(10, 10, w-20, h-20); // Draw framing rectangle

  grid(x1, y1, xdiv, ydiv, x2, y2); // Draw grid lines
}

// Display x and scale settings
void printScaleSettings(OscilArg args, int xposition, int yposition, VGfloat tcolor[4]) {
  char str[100];
  setfill(tcolor);
  //if the scale is above 1000 use ms/div
  if (args.xscale >= 1000)
    sprintf(str, "X scale = %0d ms/div", args.xscale/1000);
  else //else keep in us/div
    sprintf(str, "X scale = %0d us/div", args.xscale);
  Text(xposition, yposition, str, SansTypeface, 18);

  sprintf(str, "Y scale = %2.2f V/div", args.yscale);
  Text(xposition, yposition-50, str, SansTypeface, 18);
  //if mode is in trigger output the args related to trigger
  if (args.mode==TRIGGER_MODE){
    sprintf(str, "Mode=Trigger");
    Text(xposition-250, yposition, str, SansTypeface, 18);
    sprintf(str, "Trigger=%0.2f V",args.trigger);
    Text(xposition-250, yposition-50, str, SansTypeface, 18);
    if (args.slope==POS_SLOPE)
      sprintf(str, "Positive Slope");
    else
      sprintf(str, "Negative Slope");
    Text(xposition-500, yposition, str, SansTypeface, 18);
    sprintf(str, "Channel %d", args.channel);
    Text(xposition-500, yposition-50, str, SansTypeface, 18);
    sprintf(str, "%d ksamples/sec", args.sample);
    Text(xposition-750, yposition, str, SansTypeface, 18);
  //else in free mode
  }
  else{
    sprintf(str, "Mode=Free");
    Text(xposition-250, yposition, str, SansTypeface, 18);
  }


}

// Convert waveform samples into screen coordinates
void processSamples(char *data, // sample data
	      int nsamples, // Number of samples
	      int xstart, // starting x position of wave
	      int xfinish, // Ending x position of wave
	      float yscale, // y scale in pixels per volt
		    data_point *point_array){
  VGfloat x1, y1;
  data_point p;

  for (int i=0; i< nsamples; i++){
    x1 = xstart + (xfinish-xstart)*i/nsamples;
    y1 = data[i] * 5 * yscale/256;
    p.x = x1;
    p.y = y1;
    point_array[i] = p;
    }
}


// Plot waveform
void plotWave(data_point *data, // sample data
	      int nsamples, // Number of samples
	      int yoffset, // y offset from bottom of screen
	      VGfloat linecolor[4] // Color for the wave
	      ){

  data_point p;
  VGfloat x1, y1, x2, y2;

  Stroke(linecolor[0], linecolor[1], linecolor[2], linecolor[3]);
  StrokeWidth(4);

  p = data[0];
  x1 = p.x;
  y1 = p.y + yoffset;

  for (int i=1; i< nsamples; i++){
    p = data[i];
    x2 = p.x;
    y2 = p.y + yoffset;
    Line(x1, y1, x2, y2);
    x1 = x2;
    y1 = y2;
    }
}
dimensions Graphsetup(){
  dimensions dim;
  saveterm(); // Save current screen 
  init(&dim.width, &dim.height); // Initialize display and get width and height
  Start(dim.width, dim.height);
  rawterm(); // Needed to receive control characters from keyboard, such as ESC
  return dim;
}

void Graphplot (char *data1, char *data2, OscilArg parameters, potvals pot, dimensions dim){
	

  int margin = 10; // Margin spacing around screen
  int xdivisions = 10; // Number of x-axis divisions
  int ydivisions = 10; // Number of y-axis divisions
  char str[100];
  int width=dim.width;
  int height=dim.height;
   //offset of the waveform heights
  int waveheight1=0;
  int waveheight2=0;
  int xscale = parameters.xscale; // x scale (in units of us)
  float yscale = parameters.yscale; //y scale in V
  VGfloat textcolor[4] = {0, 200, 200, 0.5}; // Color for displaying text
  VGfloat wave1color[4] = {240, 0, 0, 0.5}; // Color for displaying Channel 1 data
  VGfloat wave2color[4] = {200, 200, 0, 0.5}; // Color for displaying Channel 2 data
  
  //intialize the number of samples and the channel points
  int samples=xscale*parameters.sample/100;
  data_point channel1_points[samples]; 
  data_point channel2_points[samples];
  //set limits of coordinates
  int xstart = margin;
  int xlimit = width - 2*margin;
  int ystart = margin;
  int ylimit = height - 2*margin;
  //set how many pixels per volt
  int pixels_per_volt = (ylimit-ystart)/(ydivisions*yscale);

  drawBackground(width, height, xdivisions, ydivisions, margin);
  
  printScaleSettings(parameters, width-300, height-50, textcolor);
  //take the values and plot into coordinates
  processSamples(data1, samples, margin, width-2*margin, pixels_per_volt, channel1_points);
  processSamples(data2, samples, margin, width-2*margin, pixels_per_volt, channel2_points);	   
  
  //set the height based on the pot values
  waveheight1=pot.pot1*4;
  waveheight2=pot.pot2*4;
  // Display 2 waves
  plotWave(channel1_points, samples, waveheight1, wave1color);
  plotWave(channel2_points, samples, waveheight2, wave2color);
  End(); 


}
