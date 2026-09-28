#include <stdint.h>
#include <iostream>

struct Translation
{
    double x = 0.0;
    double y = 0.0;
    double z = 0.0;

    Translation& operator=(const Translation& val)
    {
        if(this != &val)
        {
            x = val.x;
            y = val.y;
            z = val.z;
        }

        return *this;
    }

    void disp()
    {
        std::cout
            << x << ", "
            << y << ", "
            << z << std::endl;
    }
};

struct Orientation
{
    double q0 = 1.0;
    double q1 = 0.0;
    double q2 = 0.0;
    double q3 = 0.0;

    Orientation& operator=(const Orientation& val)
    {
        if(this != &val)
        {
            q0 = val.q0;
            q1 = val.q1;
            q2 = val.q2;
            q3 = val.q3;
        }
        
        return *this;
    }

    void disp()
    {
        std::cout
            << q0 << ", "
            << q1 << ", "
            << q2 << ", "
            << q3 << std::endl;
    }
};

struct Pose
{
    uint64_t seq = 0;
    uint64_t time_stamp_ms = 0;
    Translation translation;
    Orientation orientation;

    Pose& operator=(const Pose& val)
    {
        if(this != &val)
        {
            seq           = val.seq;
            time_stamp_ms = val.time_stamp_ms;
            translation   = val.translation;
            orientation   = val.orientation;
        }
        
        return *this;
    }

    void disp()
    {
        std::cout
            << seq << ", "
            << time_stamp_ms << ", "
            << translation.x << ", "
            << translation.y << ", "
            << translation.z << ", "
            << orientation.q0 << ", "
            << orientation.q1 << ", "
            << orientation.q2 << ", "
            << orientation.q3 << std::endl;
    }
};