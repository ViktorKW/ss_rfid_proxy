#include "CSerialInitializer.h"
#include "write_cards.h"
#include "CardsFileHandler.h"


bool check_env_state()
{

    time_t now = time(0);    
    tm *ltm = localtime(&now);

    if(system("ping -c1 -s1 www.google.com"))
    {
        cout<<"\nNo internet connection\n";
        return false;
    }
    else if(ltm->tm_hour > 6&&ltm->tm_hour < 21)
    {   
        cout<<"\nIt's not a night time\n";
        return false;
    }
    else
    {
        cout<<"\nIt's a good time to update cards\n";
        return true;
    }
}


void write_init(CSerialInitializer _csi)
{
    try
    {
        if(check_env_state())
        {
            CardsParser cp;
            std::vector<std::string> cards_vector = cp.get_cards();
            int flag = cp.get_flag();
            int number_of_cards = cards_vector.size();
            
            if(flag <= 0)
            {
                cout<<"\nDatabase is updated\n";
                return;
            }
            else if(cards_vector.size() <= 0)
            {
                cout<<"\nNot enough cards received\n";
                cout<<"Cards received: "<<number_of_cards;
                return;
            }
            else
            {
                std::vector<std::thread> threads;
                CardsFileHandler cfh;
                cfh.cardsTo_File(cards_vector);

                for(int i = 0; i<_csi.v_serial.size(); i++)
                { 
                    threads.push_back(std::thread(write_cards, _csi.v_serial[i], i, cards_vector));
                }

                for(auto& thread : threads){
                    thread.join();
                }
            }
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}
