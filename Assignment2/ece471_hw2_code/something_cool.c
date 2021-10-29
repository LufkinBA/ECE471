#include <stdio.h>

int main(int argc, char **argv) {

	int	i=0, j=0, c=1;
	for (i=0;i<2;i++)
	{
		for (j=1;j<8;j++)
			{
				printf("\e[%d;3%dm#%d: Don't fear, I AM HERE\n",i,j,c);
				c++;
				if(c>=14)
					return 0;
			}
	}
	return 0;
}
