#define MYFREQ 195.0	// myVoice.wav к║└W▓v

#define MAXNOTE 65536	// н╡▓┼н╙╝╞нниюРхИ╢
#define MAXFILESIZE 1<<28	// ┐щеX└╔о╫дjдpннию 512MB

typedef struct WAVheader{
	char RIFF[4];
	int chunkSize;
	char wave[4];
	char format[4];
	int subChunkSize;
	short audioFormat;
	short numChannel;
	int sampleRate;
	int byteRate;
	short blockAlign;
	short bitsPerSample;
	char data[4];
	int dataSize;
} WAVHeader;

void getChunk(std::ifstream &fs, char *dest, std::string target){
	for(fs.read(dest, 8); std::string(dest, 4) != target; fs.read(dest, 8))
			fs.seekg(*(int *)(dest+4), std::ios::cur);
}
