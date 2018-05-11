//     $Date: 2018-03-11 05:18:25 +1100 (Sun, 11 Mar 2018) $
// $Revision: 1206 $
//   $Author: Peter $

// Question 1

#include "Ass-01.h"

int read_coefficients(int *coeff_num_p, double **coeff_values_p, char *filename)
{
	*coeff_num_p = 0; //Insures not printing previous data

	int size;
	FILE* fp; //pointer to stream
	fp = fopen(filename, "rb"); //open given file, read binary
	if (fp == NULL)
	{
		printf("Error opening file\n");
		return -1;
	}
		//Check size is correct BEFORE malloc
		fseek(fp, 0, SEEK_END); 	//go to end of file
		size = ftell(fp)/8; 		//read position (in bytes)
		fseek(fp, 0, SEEK_SET); 	//go back to start

		fread(coeff_num_p, 4, 1, fp); //Number of Coefficients

		if (*coeff_num_p == size) //if correct size (as given)
		{
			if ((*coeff_num_p) > 0)
			{
				(*coeff_values_p) = (double *)malloc((size_t)(*coeff_num_p) * sizeof(double));

				fread(*coeff_values_p, 8, size, fp);
			}
		}
		else
		{
			printf("CORRUPTED FILE: given size does not match data\n");
			fclose(fp);
			return -1; //Size doesn't match, corrupted file
		}

	if(fclose(fp) != 0)//close file - check error value
	{
		printf("Error closing file\n");
		return -1;
	}

	printf("           ..reading filter file complete\n\n");

	//Successful end of function
	return 0;
}
