#include "CardsParser.h"
#include "CSerial.h"
using namespace std;


void write_cards(CSerial cs, int thread_id, vector<std::string> cards_vector)
{
    int serial = serialOpen(cs.serial_address.c_str(), cs.serial_port);
    serialPrintf(serial, "CLR_CARD\n\r");
    sleep(1);
    int bytes_count = 0;
    for(int i=0; i<cards_vector.size(); i++)
    {
        cout<<"\nWRITING CARD "<<cards_vector[i]<<" FOR SERIAL: "<<serial<<" "<< cs.serial_address<<" "<<cs.serial_port<<"\n";
        serialPrintf(serial, "WR_CARD:%s\n\r", cards_vector[i].c_str());
        sleep(1);
        for(int cycle = 0; cycle<5; cycle++)
        {   
            cout<<"\nRUNNING CYCLE #"<< cycle << " FOR SERIAL: "<<serial<<" "<< cs.serial_address<<" "<<cs.serial_port<<"\n";
            bytes_count = serialDataAvail(serial);
            if(bytes_count > 0)
            {
                string respond;
                for(int byte = 0; byte<bytes_count;byte++)
                {
                    respond = respond + (char)serialGetchar(serial);
                }
                cout<<"\nRESPOND FROM SERIAL "<<serial<<" "<< cs.serial_address<<" "<<cs.serial_port<<" IS "<<respond<<"\n";
                sleep(1);
                break;
            }
            else if (bytes_count <=0)
            {
                serialPrintf(serial, "WR_CARD:%s\n\r", cards_vector[i].c_str());
                sleep(1);
            }
        }
        sleep(1);
    }
    
    cout<<"\nWRITING IS COMPLETED SUCCESSFULLY FOR SERIAL: "<<serial<<" "<< cs.serial_address<<" "<<cs.serial_port<<"\n";
    serialClose(serial);
}