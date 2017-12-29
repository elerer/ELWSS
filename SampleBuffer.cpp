#include "SampleBuffer.h"
#include<fstream>


SampleBuffer::SampleBuffer(std::string wavFile)
{
  std::ifstream input(wavFile.c_str(),std::ios::binary);
  _buffer.insert(_buffer.begin(),std::istreambuf_iterator<char>(input),std::istreambuf_iterator<char>());
  _bufferLength = _buffer.size();
  _start = _buffer.begin();
}

void SampleBuffer::truncateBytes(const size_t& offset,const size_t& numOfBytes)
{
  std::vector<char>::iterator itStart = _buffer.begin() + offset;
  std::vector<char>::iterator itEnd = _buffer.begin() + offset + numOfBytes;
  _buffer.erase(itStart,itEnd);
}

size_t SampleBuffer::size()
{
  return _bufferLength;
}

size_t SampleBuffer::getPointerToChunk(char** chunk,const size_t& chunkNumber,const size_t& maxSize)
{
  int remains = _bufferLength - chunkNumber*maxSize;
  if (remains <= 0)
  {
    *chunk = 0;
    return 0;
  }

  *chunk = &(*_start) + chunkNumber*maxSize;
  return remains > maxSize ? maxSize : remains;
}

char* SampleBuffer::getPointertToStartOfBuffer()
{
  return &(*_start);
}
