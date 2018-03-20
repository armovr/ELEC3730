//     $Date: 2017-03-13 08:12:38 +1100 (Mon, 13 Mar 2017) $
// $Revision: 821 $
//   $Author: Peter $

#include "Ass-01.h"

int filter(char *filter_filename, char *input_wavefilename, char *output_wavefilename)
{
	int coeff_num;
	double *coeff_values; // Array of coefficient values

	pcm_wavefile_header_t header;
	char *dataIn;
	int16_t *dataCopy; //copy of dataIn in two bytes

	double acc; //accumulator of FIR filtered data (u in lecture notes)
	int16_t *dataOut; //filtered output data

	printf("\n--> Reading filter coefficients - '%s'\n", filter_filename);
	if(read_coefficients(&coeff_num, &coeff_values, &(*filter_filename)) != 0)
	{
		printf("ERROR: reading filter file");
		return -1;
	}

	printf("--> Reading audio file - '%s'\n", input_wavefilename);
	if(read_pcm_wavefile(&header, &dataIn, &(*input_wavefilename)) != 0)
	{
		printf("ERROR: reading audio file\n");
		return -1;
	}

	//allocate output memory
	dataOut = malloc(header.Subchunk2Size);

	dataCopy = (int16_t *)dataIn; //copy of dataIn in two bytes


	for(int n = 0; n < (header.Subchunk2Size)/2; n++) //n < data size in bytes
	{
		acc = 0;

		for(int k = 0; k < coeff_num; k++) //NOTE: if k is beyond the coeff_num, you add zero to acc
		{
			if (k > n) 		// if k is beyond dataset do nothing
			{
				acc += 0;
			}
			else 			// else apply filter
			{
				acc += (double)dataCopy[n-k]*(coeff_values[k]);
			}
		}

		dataOut[n] = (uint16_t)acc; //typecasting double acc to uint_16
	}

	printf("--> Writing filtered audio file - '%s'\n", output_wavefilename);
	if(write_pcm_wavefile(&header, (char*)dataOut, output_wavefilename) == -1)
	{
		printf("ERROR: writing audio file: %s\n", output_wavefilename);
	}

	free(dataCopy);

  return 0;
}
