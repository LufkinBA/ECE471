/* temp_cool.c
 * Blake Lufkin
 * 10/27/2021
 * Interface with an MCP3008 ADC converter to read 
 * channel 2 of the ADC converter to read the 
 * voltage of a low voltage temperature sensor
 * degrees c = (100 * CH2volt) - 50
 * 
 * Displays a warningg message if temperature exceeds 27 deg C
 */
#include <stdio.h>

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#define LENGTH 3

int main(int argc, char **argv) {

	int adc_conv, lsb_mode, spi_fd, mode, result;
    float conv_num, deg_C;
    struct spi_ioc_transfer spi;
    unsigned char data_out[LENGTH] = {0x0,0x0,0x0};
    unsigned char data_in[LENGTH];

    /* Open SPI device */
    spi_fd = open("/dev/spidev0.0", O_RDWR);
    
    if (spi_fd<0)
    {   
        printf("Error Opening SPI Device\n");
        exit(spi_fd);
    }   

    /* Set SPI Mode_0 */
    mode = SPI_MODE_0;
    result = ioctl(spi_fd, SPI_IOC_WR_MODE, &mode);
    
    //Error Checking
    if (result<0)
    {   
        printf("Error communicating with SPI Device\n");
        exit(result);
    }   

    /* Set SPI to LSB FIRST */
    lsb_mode = 0;
    result = ioctl(spi_fd, SPI_IOC_WR_LSB_FIRST, &lsb_mode);
    
    //Error Checking
    if (result<0)
    {   
        printf("Error communicating with SPI Device\n");
        exit(result);
    }   
    
    /* Set SPI transfer structure */
    memset(&spi, 0, sizeof(struct spi_ioc_transfer));

    spi.tx_buf      = (unsigned long)&data_out;
    spi.rx_buf      = (unsigned long)&data_in;
    spi.len         = LENGTH;                   //3 bytes
    spi.delay_usecs     = 0;
    spi.speed_hz        = 100000;               //100kHz
    spi.bits_per_word   = 8;
    spi.cs_change       = 0;

    /* Loop forever printing the CH0 and CH1 Voltages   
     * Once per second.                 
     */
    while(1)
    {
        usleep(1000000); //Sleep for 1 second

        /* Single High read CH1. data out 0 has to be 1s, data_out[1] is set 
         * to single rather than differential 
         */
        data_out[0] = 0x1;  //0x1 as the start bit  
        data_out[1] = 0xA0; //1010 0000 as channel 1 select
        data_out[2] = 0x0;  //0x0 and/or dontcares

        /* Initiate SPI transfer */
        result = ioctl(spi_fd, SPI_IOC_MESSAGE(1), &spi);

        //Error Checking
        if (result<0)
        {
            printf("Error communicating with SPI Device\n");
            exit(result);
        }

        /* Get the Channel 1 data from the transfer */
        adc_conv = data_in[1] & 0x3;
        adc_conv = (adc_conv << 8) +data_in[2];

        /* convert 10 bit number to floating point number signifying 
         * the voltage on CH1
         */
        conv_num = ((float)adc_conv*(3.3))/1024;
        
		/* convert voltage from channel 2 to degrees C */
		deg_C = (100 * conv_num) - 50;

		/* SOMETHING COOOOOOOOL CODE */
		if(deg_C > 27) 
		{
			printf("Warning! Exceeding Arbitrary Temperature Limit!\n");
		}
	}
	return 0;
}
