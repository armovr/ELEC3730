//     $Date: 2018-03-11 05:18:25 +1100 (Sun, 11 Mar 2018) $
// $Revision: 1206 $
//   $Author: Peter $
//



//	Questions:
//		1.Allocates memory even with no words, problem?
//		2.uint8_t too small?
//		3. storing extra spaces as NULL's a problem?
//

#include "Ass-01.h"




int string_parser(char *inp, char **array_of_words_p[])
{

		uint8_t count = 0;														//word count
		uint8_t i = 0;															//character count
		char *inp_copy = malloc(strlen(inp)+1);									//allocate memory for input copy

		strcpy(inp_copy,inp);													//copy input string

		//find how many words and allocate memory
		//loop for entire input string (until EOF is found)
		while (inp[i]!= 0)
		{

			//finds first non-space character
			while(inp[i] == ' ')
			{
				i++;
			}

			//finds end of word (either space or NULL)
			while(inp[i] != ' ' && inp[i] != 0)
			{
				i++;
			}

			//increment word count
			count++;

		}


		//allocate memory for word strings
		(*array_of_words_p)= malloc((count) * sizeof(char *));

		i=0;									//reset variables
		count=0;


		//loop again to store locations of words in array_of_words
		//loop for entire input string (until EOF is found)
		while (inp_copy[i] != 0)
		{

			//replace all strings with NULL's
			while(inp_copy[i] ==' ' && inp_copy[i] != 0)
			{
				inp_copy[i] = 0; //Add null to end of word string
				i++;
			}

			//check if EOF, if not, start of word is found. store location and increment count
			if (inp_copy[i] != 0)
			{
				(*array_of_words_p)[count] = &inp_copy[i];
				count++;
			}

			//finds end of word (either space or NULL)
			while(inp_copy[i] != ' ' && inp_copy[i] != 0)
			{
				i++;
			}

		}


		return count;
}
