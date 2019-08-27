
#include "scope.h"


int main (int argc, char * argv[]){
  
  char data1[BUFFER_SIZE];
  char data2[BUFFER_SIZE];
  
  int j=0;
  int flag=0;
  int fd;
  int samples;
  
  OscilArg parameters;
  potvals pot;
  dimensions dim;
  
  //intialize data buffers
  for (int i=0; i<BUFFER_SIZE; i++){
      data1[i]=0;
      data2[i]=0;
  }
  //grab the user parameters for the oscilloscope
  parameters=cmdargs(argc,argv);
  //set the number of samples per frame
  samples=(parameters.xscale*parameters.sample)/100;
  //intialize wave arrays
  char wave1[samples];
  char wave2[samples];
  //printf("%0.2f ", parameters.yscale);
  dim=Graphsetup(); // Save screen setup settings
  fd=I2CSetup(parameters.sample); //intialize the i2c link
  while(1){
    //read the USB buffer and fill it in with the data whilst reading pot values
    for (int i=0; i<samples; i+=64){
      Comms(data1,data2); //refill the 64byte buffers
      pot=I2C(fd); //grab the pot values
      //fill the wave arrays with the 64 bytes
      for (int j=0; j<64 && i+j<samples; j++){
        wave1[i+j]=data1[j];
        wave2[i+j]=data2[j];
      }
    }
    //process if user wants trigger or free
    if (parameters.mode==TRIGGER_MODE){
      //set flag to plot if trigger condition is satisfied
      flag=triggerprocess(parameters, wave1, wave2);
      if (flag){
        printf("check\n");
        Graphplot(wave1, wave2, parameters,pot, dim);
        flag=0; //reset flag
      }
      //if the trigger fails wait for another stack
      else
        printf("failed to satisfy trigger conditions, grabing another sample stack\n");
      }
      //free mode simply push out the graph
    else{
      Graphplot(wave1, wave2, parameters, pot, dim);
    }

  }
  

}




