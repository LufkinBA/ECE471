//Blake Lufkin
//ECE471
//Assignment 4
//Something cool
// Alternating blinking leds
//9/29/2021

#include <stdio.h>
#include <stdlib.h>

#include <fcntl.h>  /* open() */
#include <unistd.h> /* close() */
#include <string.h>
#include <errno.h>

#include "linux/gpio.h"
#include "sys/ioctl.h"

int main(int argc, char **argv) {

    //Declare Variables
    int fd, rv;
    struct gpiohandle_request req;
    struct gpiohandle_data data;

    //Open GPIO device
    fd = open("/dev/gpiochip0",O_RDWR);

    //check to see if open fails    
    if (fd < 0)
    {
        printf("Failed to open GPIO file");
        exit(fd);
    }

    //Set up the request struct
    memset(&req,0,sizeof(struct gpiohandle_request));
    req.flags = GPIOHANDLE_REQUEST_OUTPUT;
    req.lines = 2;
    req.lineoffsets[0] = 18;    //GPIO 18
    req.lineoffsets[1] = 23;	//GPIO 23
	req.default_values[0] = 0;
    strcpy(req.consumer_label, "ECE471");
    rv = ioctl(fd, GPIO_GET_LINEHANDLE_IOCTL, &req);

    //rv error check
    if (rv < 0)
    {
        printf("Error ioctl %s\n", strerror(errno));
        exit(-1);
    }
	
	while(1)
    {
        //LEDs on
        data.values[0] = 1; //turn GPIO 18 on
        data.values[1] = 0; //turn GPIO 23 off
		rv = ioctl(req.fd,GPIOHANDLE_SET_LINE_VALUES_IOCTL, &data);
        if (rv < 0)
        {
            printf("Error ioctl %s\n", strerror(errno));
            exit(-1);
        }

        //delay for .5 sec
        usleep(500000);

        data.values[0] = 0;	//turn GPIO 18 off
        data.values[1] = 1; //turn GPIO 23 on
		rv = ioctl(req.fd,GPIOHANDLE_SET_LINE_VALUES_IOCTL, &data);
        if (rv < 0)
        {
            printf("Error ioctl %s\n", strerror(errno));
            exit(-1);
        }

        //delay for .5 sec
        usleep(500000);
    }

    //close the file 
    close(fd);
    return 0;
}

