#include <iostream>
#include <fstream>
#include <cmath>
#include "wav.h"

WAVHeader header;			// wav 的檔頭，檔案中讓電腦識別檔案類型的部份
short data[MAXFILESIZE];	// 要輸出的資料點放這

// 用 cin 讀簡譜，並把每個對應的頻率寫到 frequency[]
// 回傳音符數
int readNotes(double frequency[MAXNOTE]){
	//TODO 僅供參考
	int numberOfNotes=0,note=0;
	double pitch[8]={0,261.63, 293.66, 329.63, 349.23 ,392.00,440.00};
	while(std::cin >> note)
    {
        frequency[numberOfNotes] = pitch[note];
        numberOfNotes++;
    }
	return numberOfNotes;
}

// 開啟名為 fn 的 .wav 檔，把檔案資訊放到 header
// 讓 *data_ptr 變成 fn 裡面的資料，回傳資料點數
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

// 將 data 的前 n 個資料點寫成一個名為 fn 的 .wav 檔
// 直接覆寫，不會接在後面!
void writeWAVfile(short *data, int n, std::string fn){
	WAVheader outHeader = header;
	outHeader.chunkSize = n*16+36;
	outHeader.dataSize = n*16;

	std::ofstream file(fn.c_str(), std::ios::out | std::ios::binary);
	file.write((char *)&outHeader, sizeof(header));
	file.write((char *)data, sizeof(short)*n);

	file.close();
}

// 藉由頻率和短音計算要輸出的 data 資料點
// 回傳要輸出的資料點數
int genData(double frequency[], int noteNum, short source[], int sourceNum){
	// TODO 僅供參考
	for(int i=0; i<noteNum; i++)
	{
	    double higher = frequency[i]/MYFREQ; //倍率
		int times = higher * sourceNum ; //次數
		int source_process[times+5];
		int counter = 0; //紀錄index用的
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
            source_process[j]=source[counter]; //預先處理source會用到的data
		}
		double gap = (double)times/sourceNum; //取樣間隔
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
	// 此後 myVoice[i] 可取得 myVoice.wav 的第i個資料點

	int sampleNum = genData(frequency, noteNum, myVoice, myVoiceNum);

	delete [] myVoice;
	writeWAVfile(data, sampleNum, "mySong.wav");
	return 0;
}
