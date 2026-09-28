#ifndef SERIAL_PORT_H
#define SERIAL_PORT_H

#include <string>
#include <stdint.h>

class SerialPort
{
public:

    enum class Parity
    {
        None,
        Even,
        Odd
    };

    enum class StopBits
    {
        One,
        Two
    };

public:

    SerialPort();
    ~SerialPort();

    // Non-copyable
    SerialPort(const SerialPort&) = delete;
    SerialPort& operator=(const SerialPort&) = delete;

    bool open(const std::string& device,
              uint32_t baudrate,
              Parity parity = Parity::None,
              StopBits stopBits = StopBits::One,
              uint8_t dataBits = 8);

    void close();

    bool isOpen() const;

    int write(const void* data, size_t size);

    int write(const std::string& text);

    int read(void* data, size_t size);

    bool readLine(std::string& line);

    int bytesAvailable();

    void flushRx();

    void flushTx();

private:

    int m_fd;
};

#endif
