myscope: main.o cmdargs.o usbcomm.o graphics.o data.o
	gcc -o myscope main.o cmdargs.o usbcomm.o graphics.o data.o -lshapes -lm -lusb-1.0 -lwiringPi

main.o: main.c scope.h
	gcc -c main.c

cmdargs.o: cmdargs.c scope.h
	gcc -c cmdargs.c

usbcomm.o: usbcomm.c scope.h
	gcc -c usbcomm.c -lwiringPi -I/usr/include/libusb-1.0/

graphics.o: graphics.c scope.h
	gcc -c graphics.c -I /opt/vc/include -I /usr/include 

data.o: data.c scope.h
	gcc -c data.c

clean: 
	rm *.o 


    

