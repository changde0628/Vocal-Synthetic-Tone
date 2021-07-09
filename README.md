# 這是一個可以用自己的聲音和簡譜，產生wav檔的程式。

- bee.txt:
    - 小蜜蜂的簡譜
- myVoice.wav:
    - 頻率固定(以MYFREQ定義在wav.h)的單聲道音訊檔
- singer.cpp:
    - 編譯後產生執行檔，其輸入為簡譜，輸出為單聲道音訊檔 mySong.wav
- wav.h:
    - 	給 wav.cpp 用的標頭檔，定義 .wav 檔的格式

我的做法是將 k = frequency[i]/MYFREQ 找到差的倍率  
然後再用一個陣列source_process先把資料處理好  
把source的資料放k次 在進行取樣 產生頻率的不同  
