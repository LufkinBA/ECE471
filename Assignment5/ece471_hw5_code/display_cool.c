#include <stdio.h>

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include <sys/ioctl.h>
#include <errno.h>

#include <linux/i2c-dev.h>

//Define Commands for I2C control
#define oscil_on ((0x2<<4) | (0x1))     //System Setup Register value 0x21
#define display_setup ((0x8<<4) | (0x1))//Display Setup Register value 0x81
#define brightness ((0xE<<4) | (0xB))   //Digital Dimming  Data Input value 0xEB 12/16 duty

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

    while (1)
    {
    /* Write BLAK to the Display */
    // (P, G, F, E, D, C, B, A) for the 8 bits
        buffer[0] = 0x0;
        buffer[1] = 0x7F;   //B
        buffer[2] = 0x0;
        buffer[3] = 0x38;   //L
        buffer[4] = 0x0;
        buffer[5] = 0x0;    //No colon
        buffer[6] = 0x0;
        buffer[7] = 0x77;   //A
		buffer[8] = 0x0;
		buffer[9] = 0x75;	//K

        for(i=10;i<17;i++)
        {
            buffer[i] = 0x0;
        }

        result = write(fd,buffer,17);
        if (result<0)
        {
            exit(result);
        }

        //Delay 1 second
        usleep(1000000);
    /* Write LAKE to the Display */

        buffer[0] = 0x0;
        buffer[1] = 0x38; //L
        buffer[2] = 0x0;
        buffer[3] = 0x77; //A
        buffer[4] = 0x0;
        buffer[5] = 0x0;
        buffer[6] = 0x0;
        buffer[7] = 0x75; //K
		buffer[8] = 0x0;
		buffer[9] = 0x79; //E

        for(i=10;i<17;i++)
        {
            buffer[i] = 0x0;
        }

        result = write(fd,buffer,17);
        if (result<0)
        {
            exit(result);
        }

        //Delay 1 second
        usleep(1000000);
    }
    /* Close device */

    result = close(fd);
    if (result<0)
    {
        exit(result);
    }

    return 0;
}

