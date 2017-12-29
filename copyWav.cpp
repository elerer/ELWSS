#include<fstream>
#include<vector>
#include<iostream>
#include"WavHeader.h"
#include "SampleBuffer.h"

int main()
{
  Wav_header wavHeader;
  SampleBuffer sb("/home/eranl/server/demo.wav");

  std::cout << "buffer size " << sb.size() << '\n';

  wavHeader.FillHeader(sb.getPointertToStartOfBuffer());

  std::ofstream output("/home/eranl/server/demo48.wav",std::ios::binary);
  char* pBuff = 0;
  size_t maxChunkSize = sb.size()/8;
  size_t actuallChunkSize = 0;
  size_t chunkNum = 0;
  while(actuallChunkSize = sb.getPointerToChunk(&pBuff,chunkNum++,maxChunkSize))
  {
    std::cout << "writing bytes " << actuallChunkSize<< '\n';
    output.write(pBuff,actuallChunkSize);
  }



}
