#include "basic_lib.h"


class CardsFileHandler
{
    private: 
        std::ofstream file_to_write;

        void write (const std::string& dataToWrite) {
            file_to_write<< dataToWrite << std::endl;        
        }

    
    public:
        CardsFileHandler()
        {
           
        }
        void cardsTo_File(std::vector<std::string> cards_vector)
        {
            file_to_write.open("cards.txt", std::ios::out | std::ios::app);

            for(int i = 0; i<cards_vector.size();i++)
            {
                write(cards_vector[i]);
            }
            file_to_write.close();
        }


};