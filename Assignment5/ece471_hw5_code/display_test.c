#include <stdio.h>

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/ioctl.h>

#include <linux/i2c-dev.h>

//Define Commands for I2C control
#define oscil_on ((0x2<<4) | (0x1))		//System Setup Register value 0x21
#define display_setup ((0x8<<4) | (0x1))//Display Setup Register value 0x81
#define brightness ((0xE<<4) | (0xB))	//Digital Dimming  Data Input value 0xEB 12/16 duty
int main(int argc, char **argv) {

	int fd, result, i;

	char i2c_device[]="/dev/i2c-1";

	unsigned char buffer[17];


	/* Open i2c device */

	fd = open(i2c_device, O_RDWR);
	if (fd < 0) 
	{
        printf("Failed to open GPIO file");
        exit(fd);
	}

	/* Set slave address */

	result = ioctl(fd, I2C_SLAVE, 0x70);

	/* Turn on oscillator */

	buffer[0] = oscil_on; 
	result = write(fd, buffer, 1);
	if (result<0)
	{
		exit(result);
	}
	
	/* Turn on Display, No Blink */
	
	buffer[0] =  display_setup; 
	result = write(fd, buffer, 1);
	if (result<0)
	{
		exit(result);
	}	
	
	/* Set Brightness */

 	buffer[0] = brightness;
	result = write(fd, buffer, 1);
	if (result<0)
	{
		exit(result);
	}	

	/* Write 1s to all Display Columns */

	buffer[0] = 0x0;
	for(i=0;i<16;i++)
	{
		buffer[1+i] = 0xff;
	}
	result = write(fd,buffer,17);	
	if (result<0)
	{
		exit(result);
	}	

	/* Close device */

	result = close(fd);
	if (result<0)
	{
		exit(result);
	}	
	
	return 0;
}
