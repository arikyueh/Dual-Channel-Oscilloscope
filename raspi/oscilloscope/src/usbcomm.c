#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <libusb.h>
#include <wiringPiI2C.h>
#include "scope.h"

#define  I2C_SLAVE_ADDR 0x08 // 7-bit I2C address of PSoC

int I2CSetup(int sample){
      //initialize I2C bus 
    int fd;

    // Initialize the I2C interface.
    // It returns a standard file descriptor.
    fd = wiringPiI2CSetup(I2C_SLAVE_ADDR);
    if (fd == -1){
	    perror("device not found\n");
    }
    //set command to send to PSoC to clock divide to desired sample rate
    unsigned int command =0;
    if (sample ==1) command=100;
    else if (sample ==10) command=10;
    else if (sample==20) command=5;
    else if (sample==50) command=2;
    else if (sample==100) command=1;
    
    // Write command to PSoC
    wiringPiI2CWrite(fd, command);
    return fd;
}

void Comms(char *rx_data1, char *rx_data2){
    
    libusb_device_handle* dev; // Pointer to data structure representing USB device
    libusb_init(NULL); // Initialize the LIBUSB library

    // Open the USB device (the Cypress device has
    // Vendor ID = 0x04B4 and Product ID = 0x8051)
    dev = libusb_open_device_with_vid_pid(NULL, 0x04B4, 0x8051);

    if (dev == NULL){
      perror("device not found\n");
    }

    // Reset the USB device.
    // This step is not always needed, but clears any residual state from
    // previous invocations of the program.
    if (libusb_reset_device(dev) != 0){
      perror("Device reset failed\n");
    } 

    // Set configuration of USB device
    if (libusb_set_configuration(dev, 1) != 0){
      perror("Set configuration failed\n");
    } 

    // Claim the interface.  This step is needed before any I/Os can be
    // issued to the USB device.
    if (libusb_claim_interface(dev, 0) !=0){
        perror("Cannot claim interface");
      }
    char rx_data[BUFFER_SIZE];
    int rcvd_bytes; // Bytes received
    int return_val;
    // Perform the 1st IN transfer (from device to host).
    // This will read the data back from the device.
    //printf("check\n");
    return_val = libusb_bulk_transfer(dev, // Handle for the USB device
		      (0x01 | 0x80), // Address of the Endpoint in USB device
		      // MS bit nust be 1 for IN transfers
		      rx_data, // address of 1st receive data buffer
		      64, // Size of data buffer
		      &rcvd_bytes, // Number of bytes actually received
		      0 // Timeout in milliseconds (0 to disable timeout)
		      );
    //printf("check\n");
    if (return_val != 0){
      printf("1st array error:  %s\n",libusb_strerror(return_val));
    }
    ///* FIRST 64 BYTE PRINT OUT
    if (return_val == 0){
      //rxdata1=rx_data;
      printf("==============wave1==============\n");
      for (int i=0; i<rcvd_bytes; i++){
	    rx_data1[i]=rx_data[i];
	    printf("%02x ", rx_data1[i]);
	    if (i % 16 == 15) printf("\n");
      }
      
      printf("\n");
    }
    //*/
    
    // Perform the 2nd IN transfer (from device to host).
    // This will read the data back from the device.
    return_val = libusb_bulk_transfer(dev, // Handle for the USB device
		      (0x02 | 0x80), // Address of the Endpoint in USB device
		      // MS bit nust be 1 for IN transfers
		      rx_data, // address of  2nd receive data buffer
		      64, // Size of data buffer
		      &rcvd_bytes, // Number of bytes actually received
		      0 // Timeout in milliseconds (0 to disable timeout)
		      );
    if (return_val != 0){
      printf("2nd array error:  %s\n",libusb_strerror(return_val));
    }
    ///* SECOND 64 BYTE PRINT OUT
    if (return_val == 0){
      printf("==============wave2==============\n");
      for (int i=0; i<rcvd_bytes; i++){
	    rx_data2[i]=rx_data[i];
	    printf("%02x ", rx_data2[i]);
	    if (i % 16 == 15) printf("\n");
      }
      
      printf("\n");
      }
    //*/    
    return;
}

potvals I2C(int fd){

    potvals pot;
    // Read potentiometer data
    pot.pot1 = wiringPiI2CRead(fd);
    pot.pot2 = wiringPiI2CRead(fd);
    
    //printf("Potentiometer 1 data = %d Potentiometer 2 data = %d \n", pot.pot1, pot.pot2);
    return pot;
  
  
}
