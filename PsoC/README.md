# Dual-Channel-Oscilloscope
A Dual-Channel Oscilloscope with a PSoC-5 and a Raspberry Pi 3 B+

The program takes an analog input with the user's command line arguments for the desired settings on the oscilloscope. Various 
settings are avalaible such as the sample rate, trigger modes, and the x,y scale for displaying the waveform. The PSoC handles 
converting the signal to digital and sends the data over to the Pi through USB protocols. There are ping pong buffers in the 
PSoC to ensure no bytes are lost when converting and sending. The Pi then takes the digital values and converts them into 
coordinates to be displayed through OpenVG. The Pi also handles the command line arguments to execute the specific task needed 
to set the user's parameters. The program also handles two waveforms so there are two endpoints specified for the USB transfer. 
I2C link is used to change the sample rate by clock dividing the DMA thats in charge of converting the analog samples to digital. 
The link is also used to sample the two potentiometers that control the heights of the two waveforms.
