//scope.h
//Header file for Lab Project

#ifndef SCOPE_H
#define SCOPE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>	
#include <termios.h>
#include <wiringPi.h>
#include <errno.h>


#define BUFFER_SIZE 64
#define FREE_MODE 1
#define TRIGGER_MODE 2
#define POS_SLOPE 1
#define NEG_SLOPE 2

//oscilloscope parameters
typedef struct OscilArg{
	int mode;
	float trigger;
	int slope;
	int sample;
	int channel;
	int xscale;
	float yscale;
}OscilArg;

//potentiometer values
typedef struct potvals{
	int pot1;
	int pot2;
}potvals;

// screen dimensions
typedef struct dimensions{
	int width;
	int height;
}dimensions;

//graphics.c functions
dimensions Graphsetup();
void Graphplot (char *data1, char *data2, OscilArg parameters, potvals pot, dimensions dim);

//usbcomm.c functions
int I2CSetup(int sample);
void Comms(char *rx_data1, char *rx_data2);
potvals I2C(int fd);

//cmdargs.c functions
OscilArg cmdargs (int argc, char **argv);

//data.c functions
int triggerprocess(OscilArg parameters, char *data1, char *data2);
#endif

