//Blake Lufkin
//ECE471
//Assignment 4
//Question 3
//9/29/2021


#include <stdio.h>
#include <stdlib.h>

#include <fcntl.h>	/* open() */
#include <unistd.h>	/* close() */
#include <string.h>
#include <errno.h>

#include <sys/ioctl.h>
#include <linux/gpio.h>
#include <sys/time.h>

static void switch_pressed(void) {

	struct timeval press_time;

	gettimeofday(&press_time,NULL);

	printf("Switch pressed at  %ld.%06ld\n",press_time.tv_sec,press_time.tv_usec);

}

static void switch_released(void) {

	struct timeval release_time;

	gettimeofday(&release_time,NULL);

	printf("Switch released at %ld.%06ld\n",release_time.tv_sec,release_time.tv_usec);

}


int main(int argc, char **argv) {
	
	//Declare Variables
    int fd, rv;
    struct gpiohandle_request req;
    struct gpiohandle_data data;
	unsigned int  buff;
	
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
    req.flags = GPIOHANDLE_REQUEST_INPUT;
    req.lines = 1;
    req.lineoffsets[0] = 17;    //GPIO Number
    strcpy(req.consumer_label, "ECE471");
    rv = ioctl(fd, GPIO_GET_LINEHANDLE_IOCTL, &req);

	//Clear our data
	memset(&data, 0, sizeof(data));
	buff = 0;
	
	while (1)
	{
		rv = ioctl(req.fd, GPIOHANDLE_GET_LINE_VALUES_IOCTL, &data);	
        if (rv < 0)
        {
            printf("Error ioctl %s\n", strerror(errno));
            exit(-1);
        }


		//Debouncing
		usleep(2);
		if (data.values[0] == 1) 
		{
			buff = buff << 1;
			buff++;
		}
		else
		{
			buff = buff << 1;
			buff = 0;
		}
		
		//If input is high and buffer 0xFFFF FFFF
		if (data.values[0] == 1 && (buff & 0xFF) == 1)
		{
			switch_pressed();
			while(1)
			{		
				rv = ioctl(req.fd, GPIOHANDLE_GET_LINE_VALUES_IOCTL, &data);
				if (rv < 0)
				{
					printf("Error ioctl %s\n", strerror(errno));
					exit(-1);
				}
		
				//Debouncing
				usleep(2);
				if (data.values[0] == 1) 
				{
					buff = buff << 1;
					buff++;
				}
				else
				{
					buff = buff << 1;
				}
				//If input is low and buffer 0x0
				if (data.values[0] == 0 && (buff & 0x0) == 0)
				{
					switch_released();
					break;
				}
			}
		}
	}
	
	return 0;
}
