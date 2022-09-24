September, 23rd, 2022
EC463
Henry Galindo
Tejasva Agarwal

MiniProject Report


	To modify the code for the second part of the mini project, we wanted to make use of not only the Default LED on the raspberry pi Pico board, but also of its multiple GPIO pins. For this purpose, we decided to build a simple binary counter which would increment output value by one each second. To display the output, we decided to use four GPIO pins, each of which represents one bit of information by turning one LED on or off.
  
  After getting the fade example to work, we decided that we can use the existing code for the default LED pin and repeat over for the first four pins of the raspberry pi Pico. To implement the counter feature, we decided to use an int value “count” which increments each time the fade value goes to 0 (inside the else statement). To input the binary value of “count” inte each GPIO pin, we performed bitwise operations to shift the value for each subsequent pin and take its modulus by 2 (so that the value can only be 0 or 1). After many trials and errors, we finally got it to work. The biggest hassle was to get the correct bit value to the correct LED, but after a few changes in the code, we came up with the idea of the bitwise operations which was also the most efficient way as it is closest to machine code. For getting the hardware to work, we soldered on header pins and connected the first 4 GPIO outputs to anodes of the LEDs and connected their cathodes to the ground, using appropriate resistors.
	We both made contributions to the whole project but Tejasva worked more with the software and Henry worked more with hardware.


[MiniProject Report.pdf](https://github.com/hga1indo/2022-hw-mini/files/9637742/MiniProject.Report.pdf)
