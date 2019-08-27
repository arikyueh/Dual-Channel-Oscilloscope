#include "scope.h"

int triggerprocess(OscilArg parameters, char *data1, char *data2){
	
	//converting trigger level to pixel value
	float trigger=parameters.trigger * (256 / (5 * parameters.yscale));
	int samples=(parameters.xscale*parameters.sample)/100;
	//if channel 1 and positive slope check data1 for pos slope at desired trigger level
	if (parameters.channel==1 && parameters.slope==POS_SLOPE){
		for (int i=1; i<samples; i++){
			if (data1[i-1]<trigger && data1[i]>=trigger){
				return 1;
			}
		}
	}
	//if channel 1 and negative slope check data1 for neg slope at desired trigger level
	else if (parameters.channel==1 && parameters.slope==NEG_SLOPE){
		for (int i=1; i<samples; i++){
			if (data1[i-1]>trigger && data1[i]<=trigger){
				return 1;
			}
		}
	}
	//if channel 2 and positive slope check data2 for pos slope at desired trigger level
	else if (parameters.channel==2 && parameters.slope==POS_SLOPE){
		for (int i=1; i<samples; i++){
			if (data2[i-1]<trigger && data2[i]>=trigger){
				return 1;
			}
		}
	}
	//if channel 2 and negative slope check data2 for neg slope at desired trigger level
	else if (parameters.channel==2 && parameters.slope==NEG_SLOPE){
		for (int i=1; i<samples; i++){
			if (data2[i-1]>trigger && data2[i]<=trigger){
				return 1;
			}
		}
	}
	return 0;
}
