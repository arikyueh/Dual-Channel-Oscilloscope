#include "scope.h"
#include <getopt.h>


OscilArg cmdargs (int argc, char **argv){
		int option;
		int c;
		opterr=0;
		OscilArg args;
		
		//setting default values
		args.mode=TRIGGER_MODE; //trigger mode
		args.trigger=2.5; //2.5V
		args.slope=POS_SLOPE; //positive slope
		args.sample=1000; //1k sample rate
		args.channel=1; //channel 1
		args.xscale=1000; //1000us per division
		args.yscale=1; //1V per division
		
		while ((c=getopt (argc, argv, "m:t:s:r:c:x:y:"))!=-1){
			switch (c){
				//choosing modes, either free or trigger
				case 'm':
					if (strcmp (optarg,"free")==0){
						printf("free mode\n");
						args.mode=FREE_MODE;
					}
					else if (strcmp (optarg,"trigger")==0){
						printf("trigger mode\n");
						args.mode=TRIGGER_MODE;
					}
					//print out requirement if neither free or trigger
					else {
						printf("modes can only be 'free' or 'trigger'\n");
						abort();
					}
					break;
				//choosing trigger level from a range of 0mv to 5000mV
				case 't':
					//check if out of bounds of the trigger level
					if (5000<atof(optarg) || 0>atof(optarg))
						printf("trigger out of bounds,range from 0 to 5000mV\n");
					//if not out of bounds check if the step is 100mV
					else if(atoi(optarg)%100==0){
						args.trigger=atof(optarg);
					} 
					//print out requirement if step is not 100mV
					else {
						printf("trigger must be within 0 to 5000mV in steps of 100mV\n");
						abort();
					}
					break;
				//choosing slope, either positive or negative
				case 's':
					if (strcmp (optarg,"pos")==0){
						printf("positive slope\n");
						args.slope=POS_SLOPE;
					}
					else if (strcmp (optarg,"neg")==0){
						printf("negative slope\n");
						args.slope=NEG_SLOPE;
					}
					////print out requirement if neither pos or neg
					else{
						printf("slope must be either 'pos' or 'neg'\n");
						abort();
					}
					break;
				//choosing sample rate, either 1,10,20,50,100 ksamples
				case 'r':
					if (atoi(optarg)==1 || atoi(optarg)==10 || atoi(optarg)==20 ||
					atoi(optarg)==50 || atoi(optarg)==100){
						args.sample=atoi(optarg);
					}
					//print out requirement if none of the options above
					else{
						printf("sample rate can be 1, 10, 20, 50, 100ksamples per second\n");
						abort();
					}
					break;
				//choosing channel for source of trigger, either 1 or 2
				case 'c':
					if (atof(optarg)==1 || atof(optarg)==2){
						args.channel=atof(optarg);
					}
					//print out requirement if neither 1 or 2
					else{
						printf("choose channel 1 or 2\n");
						abort();
					}
					break;
				//choosing x scale in microseconds for the graphics
				case 'x':
					if (atof(optarg)==100 || atof(optarg)==500 ||atof(optarg)==1000 || 
					atof(optarg)==2000 || atof(optarg)==5000 || atof(optarg)==10000 || 
					atof(optarg)==50000 || atof(optarg)==100000){
						args.xscale=atof(optarg);
					}
					//print out requirement if none of the options above were triggered
					else {
						printf("x scale can only be 100, 500, 1000, 2000, 5000, 10000, 50000 or 100000\n");
						abort();
					}
					break;
				//choosing y scale in mV for graphics
				case 'y':
					if (atof(optarg)==100 || atof(optarg)==500 || atof(optarg)==1000 || 
					atof(optarg)==2000 || atof(optarg)==2500){
						args.yscale=atof(optarg)/1000;
						printf("%f \n", args.yscale);
					}
					//print out requirement if none of the options above were triggered
					else{
						printf("y scale can only be 100, 500, 1000, 2000 or 2500\n");
						abort();
					}
					break;
				//any invalid character aborts program
				default:
					printf("invalid character option\n");
					abort();
			}
	
		}
		return args; //return the struct of the desired parameters
		
}
