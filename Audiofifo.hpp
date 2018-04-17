#ifndef AUDIOFIFO_HPP
#define AUDIOFIFO_HPP
#include <stdint.h>
#include <stdlib.h>
#include <memory.h>
class AudioFifo
{
public:
    // initial fifo size
    AudioFifo():_fifoInputIndex(0),
                _fifoOutputIndex(0),
                _bufferSize(0)
    {

    }
    ~AudioFifo()
    {
        //delete[]_fifoBuffer;
        //_fifoBuffer = nullptr;
        if(_fifoBuffer!= NULL){
            free(_fifoBuffer);
            _fifoBuffer = NULL;
        }
    }

    int getBufferSampleSize() {
        return getInputSize()/2;
    }

    bool initFifo(int size) {
        _fifoBuffer = (char*)malloc(sizeof(char)*size);
        if(_fifoBuffer == NULL) {
            return false;
        }
        _bufferSize = size;
        return true;
    }

    //check whether the fifo is write full
    bool isWriteFull()
    {
        auto inputIndex = _fifoInputIndex;

        auto temp = (++inputIndex >= _bufferSize) ? 0 : inputIndex;

        if (temp == _fifoOutputIndex)
        {
            return true;
        }

        return false;
    }
    //check whether the fifo is read empty
    bool isReadEmpty()
    {
        return (_fifoInputIndex == _fifoOutputIndex) ? true : false;
    }

    bool writeData(const char* pdata, unsigned int len, bool toRealloc = true)
    {
        auto inData = pdata;

        if (toRealloc)
        {
            if (!isWriteSpaceEnough(len))
            {
                //return false;
                _bufferSize += len*3;
                setFifoSize(_bufferSize);
            }
            for (unsigned int i=0; i < len; i++)
            {
                _fifoBuffer[_fifoInputIndex++] = *inData++;

                if (_fifoInputIndex >= _bufferSize)
                {
                    _fifoInputIndex = 0;
                }
            }
        }
        else
        {
            for (unsigned int i=0; i < len; i++)
            {
                if (!isWriteFull())
                {
                    _fifoBuffer[_fifoInputIndex++] = *inData++;

                    if (_fifoInputIndex >= _bufferSize)
                    {
                        _fifoInputIndex = 0;
                    }
                }
                else
                {
                    return false;
                }
            }

        }
        return true;
    }
    //write one data only
    bool writeOneData(const char data)
    {
        if (!isWriteFull())
        {
            _fifoBuffer[_fifoInputIndex++] = data;

            if (_fifoInputIndex >= _bufferSize)
            {
                _fifoInputIndex = 0;
            }

            return true;
        }

        else
        {
            return false;
        }
    }


    //read data from fifo ,if it is not read empty,return true ,or else return false
    int readData(char* pdata,unsigned int len)
    {
        int index = 0;
        if (!isReadSpaceEnough(len))
        {
            return false;
        }
        auto pOutBuffer = pdata;

        for (unsigned int i = 0; i < len;i++)
        {
            *pOutBuffer++ = _fifoBuffer[_fifoOutputIndex++];

            if (_fifoOutputIndex>=_bufferSize)
            {
                _fifoOutputIndex = 0;
            }
        }
        memmove(_fifoBuffer, _fifoBuffer+len, _bufferSize-len);
        if(_fifoOutputIndex <= _fifoInputIndex ) {
            _fifoInputIndex -= _fifoOutputIndex;
            index = _fifoOutputIndex;
            _fifoOutputIndex = 0;
        }
        return index;

    }
    //read one data only
//    bool readOneData(char& outData)
//    {
//        if (!isReadSpaceEnough(1))
//        {
//            return false;
//        }

//        outData = _fifoBuffer[_fifoOutputIndex++];

//        if (_fifoOutputIndex >= _bufferSize)
//        {
//            _fifoOutputIndex = 0;
//        }

//        return true;

//    }

    //get fifo size
    const unsigned int getBufferSize(){ return _bufferSize; }

    //clear fifo
    void resetFifo()
    {
        _fifoInputIndex = 0;
        _fifoOutputIndex = 0;
        //memset(_fifoBuffer, 0, sizeof(_fifoBuffer));
        memset(_fifoBuffer, 0, getBufferSize());
    }

    //resize the fifo size
    void resizeFifo(unsigned int len)
    {
        resetFifo();

        if (len+1 == _bufferSize)
        {
            return;
        }

        if (_fifoBuffer)
        {
            //delete[]_fifoBuffer;
            free(_fifoBuffer);
            _fifoBuffer = NULL;
        }
        _bufferSize = len+1;

       // _fifoBuffer = new char[_bufferSize];
        _fifoBuffer = (char*)malloc(sizeof(char)*_bufferSize);

    }




private:
    char *_fifoBuffer;
    unsigned int _fifoInputIndex;
    unsigned int _fifoOutputIndex;
    unsigned int _bufferSize;

    void setFifoSize(unsigned int len)
    {
       _fifoBuffer = (char*)realloc(_fifoBuffer,sizeof(char)*len);
    }

    const unsigned int getWriteSpace()
    {

        if (isWriteFull())
        {
            return 0;
        }

        auto surplus = (_fifoInputIndex >= _fifoOutputIndex)
         ? (_bufferSize - _fifoInputIndex + _fifoOutputIndex)
            : (_fifoOutputIndex - _fifoInputIndex);

        return surplus;
    }

    const bool isWriteSpaceEnough(unsigned int len)
    {
        return (getWriteSpace() >= len) ? true : false;
    }

    const unsigned int getReadSpace()
    {

        if (isReadEmpty())
        {
            return 0;
        }

        auto surplus = (_fifoInputIndex >= _fifoOutputIndex) ? (_fifoInputIndex - _fifoOutputIndex)
            : (_bufferSize - _fifoOutputIndex + _fifoInputIndex);

        return surplus;
    }

    const bool isReadSpaceEnough(unsigned int len)
    {
        return (getReadSpace() >= len) ? true : false;
    }

     const unsigned int getInputSize(){ return _fifoInputIndex; }


};

#endif // AUDIOFIFO_HPP
