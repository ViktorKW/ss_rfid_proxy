#include "basic_lib.h"
using namespace std;

#pragma once
class CSerial
{
    public:
        string serial_address;
        int serial_port;


        CSerial(string _serial_address, int _serial_port)
        {
            serial_address = _serial_address;
            serial_port = _serial_port;
        }
};