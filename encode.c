#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "wav.h"

int rtfm(char exe[], int result) {
	printf("Usage: %s infile\nOutputs a .wav file including the source file.\n",exe);
	return result;
}

void convert_file_to_song(char infile[]) 
{
	char outfile[256];
	sprintf(outfile,"%s.wav",infile);

	FILE * fi = fopen(infile,"rb");
	FILE * fo = fopen(outfile,"wb");

	size_t in_size;
	struct stat stbuf;
	
	int wav_size;
	int sample_amount;
	short num_channels;
	char byte;
	size_t c;
	
	fstat(fileno(fi), &stbuf);
	in_size = stbuf.st_size;
	
	printf("infile size: %zu\n",in_size);
	wav_size = sizeof(struct wav_header)+in_size;
	sample_amount = in_size;
	fseek(fi,0L,SEEK_SET);
	if(in_size % 2) {
		num_channels = 1;
	} else {
		num_channels = 2;
	}
	struct wav_header wav = {	{82,73,70,70},
					wav_size,
					{87,65,86,69},
					{102,109,116,32},
					16,
					1,
					11050,
					11050*num_channels*1,
					num_channels*1,
					8,
					{100,97,116,97},
					sample_amount*num_channels};
	puts("Writing WAV header");
	fwrite(&wav,sizeof(struct wav_header),1,fo);
	printf("Copying %zu bytes\n",in_size);
	for(c = 0; c < in_size; ++c) {
		byte = fgetc(fi);
		fputc(byte,fo);
	}
	puts("Done");
	fclose(fi);
	fclose(fo);
}

int main(int argc, char * argv[]) {
	if(argc != 2) {
		return rtfm(argv[0],-1);
	}
	convert_file_to_song(argv[1]);
	return 0;
}
