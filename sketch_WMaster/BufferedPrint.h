#ifndef BufferedPrint_h
#define BufferedPrint_H_



#include <ESP8266WiFi.h>



template <size_t CAPACITY>
class BufferedPrint : public Print
{
    public:
    BufferedPrint(Print &destination) : _destination(destination), _size(0)
    {
    }

    virtual size_t write(uint8_t c)
    {
        _buffer[_size++] = c;

        if (_size + 1 == CAPACITY) {
            this->flush();
        }
    }

    void flush(void)
    {
        buffer[_size] = '\0';
        _destination.print(_buffer);
        _size = 0;
    }

    private:
    Print& _destination;
    size_t _size;
    char _buffer[CAPACITY];
    };


#endif