#include <string>
#include <vector>

class SampleBuffer
{
public:
  SampleBuffer(std::string wavFile);
  size_t size();
  //returns the size of the chunk can be maxSize or less, if 0 than end of buffer.
  size_t getPointerToChunk(char** chunk,const size_t& chunkNumber,const size_t& maxSize);

  char* getPointertToStartOfBuffer();

  void truncateBytes(const size_t& offset,const size_t& numOfBytes);

private:
  std::vector<char> _buffer;
  size_t _bufferLength;
  std::vector<char>::iterator _start;
};
