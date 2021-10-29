#include <stdio.h>
#include <stdint.h>

uint32_t print_integer(uint32_t value) {

	char output[10];		/* a buffer to hold our result */
	char *output_pointer;		/* a character pointer */
	uint32_t quotient,remainder;


	/* NUL terminate the string */
	output[9]=0;

	/* point to end of string, we'll work backwards */
	output_pointer=output+8;

	while(1) {
		/* to get the low digit, divide by 16 */
		/* and the remainder is the digit */
		quotient=value/16;				//changed to 16 to calculate hex value 
		remainder=value%16;				//changed to 16 to calculate hex value 

		/* convert to ASCII by adding 0x30 */
		if(remainder > 9)						//if value would be ABCDE or F
			{
				*output_pointer=remainder+'7';	//skip 7 spots in the ascii table to acess the characters
			}
		else
			{
				*output_pointer=remainder+'0';
			}
		/* if quotient is 0 we are done with the number */
		if (quotient==0) break;

		/* point pointer to next digit */
		output_pointer--;

		/* repeat using the quotient this time */
		value=quotient;
	}

	/* print the string. */
	printf("0x%s\n",output_pointer);

	return 0;
}


int main(int argc, char **argv) {

	/* test the print integer code */
	print_integer(1978);

	return 0;
}
