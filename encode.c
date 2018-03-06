#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "wav.h"
int rtfm(char exe[], int result) {
	printf("Usage: %s infile\nOutputs a .wav file including the source file.\n",exe);
	return result;
}

void convert_file_to_song(char infile[]) {
	char * outfile = strcat(infile,".wav");
	FILE * fi = fopen(infile,"rb");
	FILE * fo = fopen(outfile,"wb");
	size_t in_size;
	int wav_size;
	int sample_amount;
	short num_channels;
	char byte;
	size_t c;
	printf("Size before seek end: %zu\n",ftell(fi));
	fseek(fi,0L,SEEK_END);
	in_size = ftello(fi);
	printf("Size after seek end: %zu\n",in_size);
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
