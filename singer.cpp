#include <iostream>
#include <fstream>
#include <cmath>
#include "wav.h"

WAVHeader header;			// wav �����Y�A�ɮפ����q���ѧO�ɮ�����������
short data[MAXFILESIZE];	// �n��X������I��o

// �� cin Ū²�СA�ç�C�ӹ������W�v�g�� frequency[]
// �^�ǭ��ż�
int readNotes(double frequency[MAXNOTE]){
	//TODO �ȨѰѦ�
	int numberOfNotes=0,note=0;
	double pitch[8]={0,261.63, 293.66, 329.63, 349.23 ,392.00,440.00};
	while(std::cin >> note)
    {
        frequency[numberOfNotes] = pitch[note];
        numberOfNotes++;
    }
	return numberOfNotes;
}

// �}�ҦW�� fn �� .wav �ɡA���ɮ׸�T��� header
// �� *data_ptr �ܦ� fn �̭�����ơA�^�Ǹ���I��
int readWAVdata(short **data_ptr, std::string fn){
	std::ifstream file(fn.c_str(), std::ios::in | std::ios::binary);
	file.read((char *)&header, 12);
	getChunk(file, header.format, "fmt ");
	file.read((char *)&header.audioFormat, 16);
	getChunk(file, header.data, "data");

	int sampleNum = header.dataSize / 2;
	*data_ptr = new short[sampleNum];
	file.read((char *)(*data_ptr), sampleNum*sizeof(short));

	file.close();
	return sampleNum;
}

// �N data ���e n �Ӹ���I�g���@�ӦW�� fn �� .wav ��
// �����мg�A���|���b�᭱!
void writeWAVfile(short *data, int n, std::string fn){
	WAVheader outHeader = header;
	outHeader.chunkSize = n*16+36;
	outHeader.dataSize = n*16;

	std::ofstream file(fn.c_str(), std::ios::out | std::ios::binary);
	file.write((char *)&outHeader, sizeof(header));
	file.write((char *)data, sizeof(short)*n);

	file.close();
}

// �ǥ��W�v�M�u���p��n��X�� data ����I
// �^�ǭn��X������I��
int genData(double frequency[], int noteNum, short source[], int sourceNum){
	// TODO �ȨѰѦ�
	for(int i=0; i<noteNum; i++)
	{
	    double higher = frequency[i]/MYFREQ; //���v
		int times = higher * sourceNum ; //����
		int source_process[times+5];
		int counter = 0; //����index�Ϊ�
		for(int j=0;j<times;j++)
		{
			if(counter>sourceNum)
            {
                counter = 0;
            }
            else
            {
                counter ++;
            }
            source_process[j]=source[counter]; //�w���B�zsource�|�Ψ쪺data
		}
		double gap = (double)times/sourceNum; //���˶��j
		for(int j=0;j<sourceNum/2;j++)
        {
            int x = (int)(gap*j);
            data[i*(sourceNum/2)+j] = source_process[x];
        }
	}
	return sourceNum * noteNum;
}

int main(){
	double frequency[MAXNOTE];
	int noteNum = readNotes(frequency);

	short *myVoice;
	int myVoiceNum = readWAVdata(&myVoice, "myVoice.wav");
	// ���� myVoice[i] �i���o myVoice.wav ����i�Ӹ���I

	int sampleNum = genData(frequency, noteNum, myVoice, myVoiceNum);

	delete [] myVoice;
	writeWAVfile(data, sampleNum, "mySong.wav");
	return 0;
}
