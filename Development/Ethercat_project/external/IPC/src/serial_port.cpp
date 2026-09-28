#include "serial_port.h"

#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <cstring>
#include <termios.h>
#include <sys/ioctl.h>

namespace
{

speed_t baudrateToPosix(uint32_t baudrate)
{
    switch (baudrate)
    {
        case 50: return B50;
        case 75: return B75;
        case 110: return B110;
        case 134: return B134;
        case 150: return B150;
        case 200: return B200;
        case 300: return B300;
        case 600: return B600;
        case 1200: return B1200;
        case 1800: return B1800;
        case 2400: return B2400;
        case 4800: return B4800;
        case 9600: return B9600;
        case 19200: return B19200;
        case 38400: return B38400;
        case 57600: return B57600;
        case 115200: return B115200;
#ifdef B230400
        case 230400: return B230400;
#endif
#ifdef B460800
        case 460800: return B460800;
#endif
        default:
            return B115200;
    }
}

}

SerialPort::SerialPort()
    : m_fd(-1)
{
}

SerialPort::~SerialPort()
{
    close();
}

bool SerialPort::open(const std::string& device,
                      uint32_t baudrate,
                      Parity parity,
                      StopBits stopBits,
                      uint8_t dataBits)
{
    close();

    m_fd = ::open(device.c_str(), O_RDWR | O_NOCTTY);

    if (m_fd < 0)
        return false;

    termios tty;

    memset(&tty, 0, sizeof(tty));

    if (tcgetattr(m_fd, &tty) != 0)
    {
        close();
        return false;
    }

    cfsetispeed(&tty, baudrateToPosix(baudrate));
    cfsetospeed(&tty, baudrateToPosix(baudrate));

    tty.c_iflag = 0;
    tty.c_oflag = 0;
    tty.c_lflag = 0;

    tty.c_cflag |= (CREAD | CLOCAL);

    tty.c_cflag &= ~CSIZE;

    switch (dataBits)
    {
        case 5: tty.c_cflag |= CS5; break;
        case 6: tty.c_cflag |= CS6; break;
        case 7: tty.c_cflag |= CS7; break;
        default: tty.c_cflag |= CS8; break;
    }

    switch (parity)
    {
        case Parity::None:
            tty.c_cflag &= ~PARENB;
            break;

        case Parity::Even:
            tty.c_cflag |= PARENB;
            tty.c_cflag &= ~PARODD;
            break;

        case Parity::Odd:
            tty.c_cflag |= PARENB;
            tty.c_cflag |= PARODD;
            break;
    }

    if (stopBits == StopBits::Two)
        tty.c_cflag |= CSTOPB;
    else
        tty.c_cflag &= ~CSTOPB;

    tty.c_cc[VMIN] = 1;
    tty.c_cc[VTIME] = 0;

    tcflush(m_fd, TCIOFLUSH);

    if (tcsetattr(m_fd, TCSANOW, &tty) != 0)
    {
        close();
        return false;
    }

    return true;
}

void SerialPort::close()
{
    if (m_fd >= 0)
    {
        ::close(m_fd);
        m_fd = -1;
    }
}

bool SerialPort::isOpen() const
{
    return (m_fd >= 0);
}

int SerialPort::write(const void* data, size_t size)
{
    if (!isOpen())
        return -1;

    return ::write(m_fd, data, size);
}

int SerialPort::write(const std::string& text)
{
    return write(text.data(), text.size());
}

int SerialPort::read(void* data, size_t size)
{
    if (!isOpen())
        return -1;

    return ::read(m_fd, data, size);
}

bool SerialPort::readLine(std::string& line)
{
    line.clear();

    char ch;

    while (true)
    {
        int n = read(&ch, 1);

        if (n <= 0)
            return false;

        if (ch == '\n')
            return true;

        if (ch != '\r')
            line += ch;
    }
}

int SerialPort::bytesAvailable()
{
    if (!isOpen())
        return 0;

    int bytes = 0;

    if (ioctl(m_fd, FIONREAD, &bytes) == -1)
        return 0;

    return bytes;
}

void SerialPort::flushRx()
{
    if (isOpen())
        tcflush(m_fd, TCIFLUSH);
}

void SerialPort::flushTx()
{
    if (isOpen())
        tcflush(m_fd, TCOFLUSH);
}
