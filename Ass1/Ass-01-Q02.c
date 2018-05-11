//     $Date: 2018-03-11 05:18:25 +1100 (Sun, 11 Mar 2018) $
// $Revision: 1206 $
//   $Author: Peter $

// Question 2

#include "Ass-01.h"
#define CHAR_BIT 1

int read_pcm_wavefile(pcm_wavefile_header_t *header_p, char **data_p, char *filename)
{
	int sampleSize;			//size of samples in bytes

		FILE* fp = fopen(filename, "rb");
		if (fp == NULL)
		{
			printf("Error opening file\n");
			return -1;
		}

		//read header
		if(read_header(header_p, filename, fp) != 0)
		{
			fclose(fp);
			printf("ERROR IN FILE HEADER: cannot process\n\n");
			return -1;
		}


		sampleSize 	= (header_p->BitsPerSample/CHAR_BIT); //size of sample in bytes

		//Data Allocation
		(*data_p) = (char *)malloc((size_t)sampleSize*header_p->Subchunk2Size);	//two bytes per sample

		if(data_p == NULL)
		{
			printf("Error Allocating Memory for Data\n");
			fclose(fp);
			return -1;
		}


		//Data Read
		if(fread(*data_p, sampleSize, header_p->Subchunk2Size, fp) != header_p->Subchunk2Size/sampleSize)
		{
			printf("Error reading data from file %s\n", filename);
			free(*data_p);
			fclose(fp);
			return -1;
		}

		//CLOSE FILE
		if(fclose(fp) != 0)//close file - check error value
		{
			printf("Error closing file\n");
			//free(*data_p);
			return -1;
		}

		printf("           ...reading audio file complete\n\n");

		return 0;
}

int write_pcm_wavefile(pcm_wavefile_header_t *header_p, char *data_p, char *filename)
{
		int sampleSize	= header_p->BitsPerSample/CHAR_BIT; 	//sample size in bytes
		int dataSize	= header_p->Subchunk2Size/sampleSize; 	//data size in bytes

		//Open file for writing
		FILE* fp = fopen(filename, "wb");
		if(fp == NULL)
		{
			printf("Error opening output (write) file %s\n", filename);
			return -1;
		}

		if(fwrite(header_p, sizeof(pcm_wavefile_header_t), 1, fp) != 1)
		{
			printf("Error writing header to %s\n", filename);
		}

		if(fwrite(data_p, sampleSize, dataSize, fp) != dataSize)
		{
			printf("Error writing data to %s\n", filename);
		}

		if(fclose(fp) != 0)//close file - check error value
		{
			printf("Error closing file\n");
			return -1;
		}

		printf("           ...writing audio file complete\n\n");

		return 0;
}

int read_header(pcm_wavefile_header_t *header_p, char *filename, FILE *fp)
{
	int fileSize; 			//checked size of whole file
	int currentSize; 		//output of last ftell
	int flag = 0;

	//Check size is correct BEFORE malloc
	fseek(fp, 0, SEEK_END); 	//go to end of file
	fileSize = ftell(fp); 			//read position
	fseek(fp, 0, SEEK_SET); 	//go back to start

	printf("\n--> Reading header:\n");


	// CAN READ IN WHOLE THING AND THROW BACK ALL THE PROBLEMS?
	//
	// header_p = malloc((size_t)sizeof(pcm_wavefile_header_t));
    //
	// fread(&(header_p), sizeof(pcm_wavefile_header_t), 1, fp);
	//
	// Makes program crash.


	//Read Header and error check along the way
	fread(&(header_p->ChunkID), sizeof(header_p->ChunkID), 1, fp);
	printf("---> ChunkID        = %c%c%c%c\n", header_p->ChunkID[0],header_p->ChunkID[1],header_p->ChunkID[2],header_p->ChunkID[3]);
	if(header_p->ChunkID[0] != 'R' || header_p->ChunkID[1] != 'I' || header_p->ChunkID[2] != 'F' || header_p->ChunkID[3] != 'F')
	{
		printf("ERROR: Not a RIFF based file, or File Corrupted\n");
		flag =  -1;
	}

	fread(&(header_p->ChunkSize), sizeof(header_p->ChunkSize), 1, fp);
	printf("---> ChunkSize      = %d\n", header_p->ChunkSize);
	//check whole file size is what it claims
	if(fileSize != 8 + header_p->ChunkSize)
	{
		printf("Size = %d; GivenSize = %d ---", fileSize, 8 + header_p->ChunkSize);
		printf("ERROR: File size is mismatched; appears File Corrupted\n");
		flag =  -1;
	}

	fread(&(header_p->Format), sizeof(header_p->Format), 1, fp);
	printf("---> Format         = %c%c%c%c\n", header_p->Format[0],header_p->Format[1],header_p->Format[2],header_p->Format[3]);
	if(header_p->Format[0] != 'W' || header_p->Format[1] != 'A' || header_p->Format[2] != 'V' || header_p->Format[3] != 'E')
	{
		printf("ERROR: Not a WAVE file, or File Corrupted\n");
		flag =  -1;
	}

	fread(&(header_p->Subchunk1ID), sizeof(header_p->Subchunk1ID), 1, fp);
	printf("---> Subchunk1ID    = %c%c%c%c\n", header_p->Subchunk1ID[0],header_p->Subchunk1ID[1],header_p->Subchunk1ID[2],header_p->Subchunk1ID[3]);
	if(header_p->Subchunk1ID[0] != 'f' || header_p->Subchunk1ID[1] != 'm' || header_p->Subchunk1ID[2] != 't' || header_p->Subchunk1ID[3] != ' ')
	{
		printf("ERROR: Subchunk1ID incorrect: File Corrupted\n");
		flag =  -1;
	}

	fread(&(header_p->Subchunk1Size), sizeof(header_p->Subchunk1Size), 1, fp);
	printf("---> Subchunk1Size  = %d\n", header_p->Subchunk1Size);
	//Is this even worth doing?? Expect size 16 when PCM encoded
	if(header_p->Subchunk1Size > 16) printf("Subchunk1Size too large, suspect not PCM encoded; \n");
	else if(header_p->Subchunk1Size < 16) printf("Subchunk1Size too small, suspect not PCM encoded; \n");

	fread(&(header_p->AudioFormat), sizeof(header_p->AudioFormat), 1, fp);
	printf("---> AudioFormat    = %d\n", header_p->AudioFormat);
	//Check is PCM encoded
	if(header_p->AudioFormat != 1)
	{
		printf("ERROR: Audio file not PCM audio format.\n");
		flag =  -1;
	}

	fread(&(header_p->NumChannels), sizeof(header_p->NumChannels), 1, fp);
	printf("---> NumChannels    = %d\n", header_p->NumChannels);
	//one channel?
	if(header_p->NumChannels != 1)
	{
		printf("ERROR: This program is unable to handle multi-channel audio files\n\n");
		flag =  -1;
	}


	fread(&(header_p->SampleRate), sizeof(header_p->SampleRate), 1, fp);
	printf("---> SampleRate     = %d\n", header_p->SampleRate);

	fread(&(header_p->ByteRate), sizeof(header_p->ByteRate), 1, fp);
	printf("---> ByteRate       = %d\n", header_p->ByteRate);

	fread(&(header_p->BlockAlign), sizeof(header_p->BlockAlign), 1, fp);
	printf("---> BlockAlign     = %d\n", header_p->BlockAlign);

	fread(&(header_p->BitsPerSample), sizeof(header_p->BitsPerSample), 1, fp);
	printf("---> BitsPerSample  = %d\n", header_p->BitsPerSample);

	//Ensure format numbers are consistent REF: http://soundfile.sapp.org/doc/WaveFormat/
	if(header_p->ByteRate != header_p->SampleRate * header_p->NumChannels * header_p->BitsPerSample/8 || header_p->BlockAlign != header_p->NumChannels * header_p->BitsPerSample/8)
	{
		printf("ERROR: format seems to be misaligned: File Corrupted \n");
		flag =  -1;
	}

	fread(&(header_p->Subchunk2ID), sizeof(header_p->Subchunk2ID), 1, fp);
	printf("---> Subchunk2ID    = %c%c%c%c\n", header_p->Subchunk2ID[0],header_p->Subchunk2ID[1],header_p->Subchunk2ID[2],header_p->Subchunk2ID[3]);
	if(header_p->Subchunk2ID[0] != 'd' || header_p->Subchunk2ID[1] != 'a' || header_p->Subchunk2ID[2] != 't' || header_p->Subchunk2ID[3] != 'a')
	{
		printf("ERROR: Subchunk2ID incorrect: corrupted file\n");
		flag =  -1;
	}

	fread(&(header_p->Subchunk2Size), sizeof(header_p->Subchunk2Size), 1, fp);
	printf("---> Subchunk2Size  = %d\n", header_p->Subchunk2Size);
	currentSize = ftell(fp);
	//check it is consistent with whole size: note && for even byte boundry
	if(fileSize != currentSize + header_p->Subchunk2Size && fileSize != currentSize + header_p->Subchunk2Size + 1)
	{
		printf("ERROR: Actual Size = %d; Current Size + Subchunk2Size = %d; --- ", fileSize, currentSize + header_p->Subchunk2Size);
		printf("File size is mismatched; appears File Corrupted\n");
		flag =  -1;
	}

	return flag;
}
