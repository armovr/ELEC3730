//     $Date: 2017-03-13 08:12:38 +1100 (Mon, 13 Mar 2017) $
// $Revision: 821 $
//   $Author: Peter $

#include "Ass-01.h"

int filter(char *filter_filename, char *input_wavefilename, char *output_wavefilename)
{

	int coeff_num;
	double *coeff_values; // Array of coefficient values

	pcm_wavefile_header_t header;
	char *data;

	printf("\n--> Reading filter coefficients - '%s'\n", filter_filename);
	if (read_coefficients(&coeff_num, &coeff_values, filter_filename) != 0)
	{
		free(coeff_values);
		return -1;
	}

	printf("--> Reading audio file - '%s'\n", input_wavefilename);
	if (read_pcm_wavefile(&header, &data, input_wavefilename) != 0)
	{
		free(coeff_values);
		free(data);
		return -1;
	}

	// printf("--> Filtering audio file - '%s'\n", output_wavefilename);
	//
	// FILTER DATA HERE
	//
	// printf("             ...done\n");

	printf("--> Writing filtered audio file - '%s'\n", output_wavefilename);
	if (write_pcm_wavefile(&header, data, output_wavefilename) != 0)
	{
		free(coeff_values);
		free(data);
		return -1;
	}


	free(coeff_values);
	free(data);

	return 0;
}
