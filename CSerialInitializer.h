#include "basic_lib.h"
using namespace std;
#include "CSerial.h"

#pragma once
class CSerialInitializer
{
    private:
        void check_uart_connection(string connection_type, int serial_address)      //receives specific uart connection and checks if it's valid 
        {
            for(int i = 0; i<5; i++) //there can be up to 5 uart connections of a specific type, (for example ttyAMA1, ttyAMA2 and so on), so we have to validate each
            {
                CSerial cs("/dev/" + connection_type  + to_string(i), serial_address);
                int serial = serialOpen(cs.serial_address.c_str(), cs.serial_port);

                if(serial<0)
                {   
                    cout<<"\n" + cs.serial_address +" is NOT established\n";
                }
                else
                {
                    cout<<"\nChecking connection for serial "<<cs.serial_address <<" "<< serial<<"\n";
                    int bytes_amount = 0;
                    int cycle = 0;
                    while(cycle<5&&bytes_amount<=0)
                    {
                        cout<<"\nRunning cycle for serial "<<cs.serial_address<<" "<<serial<<"\n";
                        cycle++;
                        serialPrintf(serial, "LIST:\n\r");
                        sleep(1); 
                        bytes_amount = serialDataAvail(serial);
                        cout<<"\nBytes gained "<<bytes_amount<<"\n";
                        if(bytes_amount>0)
                        {
                            char respond[bytes_amount+1];
                            for(int byte = 0; byte<bytes_amount;byte++)
                            {
                                respond[byte] = (char)serialGetchar(serial);
                            }
                            cout<<"RESPOND FROM SERIAL "<<serial<<" "<< cs.serial_address<<" "<<cs.serial_port<<" IS "<<"\n"<<respond<<"\n";

                            cout<<"\nConnection established for serial "<<cs.serial_address<<" "<<serial<<"\n";
                            v_serial.push_back(cs); 
                            sleep(5);                    
                        }
                        usleep(350000); //amount of time essential for 8 card readers to reply
                    }           
                }
                cout<<"\nClosing connection with "<<cs.serial_address<<" "<< serial<<"\n";
                serialClose(serial);  
                sleep(1);      
 
            }
        }


    public:
        std::vector<CSerial> v_serial;
        CSerialInitializer()
        {
            check_uart_connection("ttyAMA", 115200);
            check_uart_connection("ttyS", 115200);
        }
};