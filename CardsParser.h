#include "basic_lib.h"
#include "get_request.h"
#include "string_ToVector.h"

const string SIMPLY_SCHOOL_BASE_URL = "https://simply.school";
const string GET_CARDS_URL = "";

class CardsParser
{
    private:
        std::vector<std::string> cards;
        bool flag; 

        
    public:
        CardsParser()
        {
            try
            {
                const std::string rawJson = get_request(SIMPLY_SCHOOL_BASE_URL + "/" + GET_CARDS_URL);
                const auto rawJsonLength = static_cast<int>(rawJson.length());
                constexpr bool shouldUseOldWay = false;
                JSONCPP_STRING err;
                Json::Value root;

                if (shouldUseOldWay) 
                {
                    Json::Reader reader;
                    reader.parse(rawJson, root);
                } 
                else 
                {
                    Json::CharReaderBuilder builder;
                    const std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
                    if (!reader->parse(rawJson.c_str(), rawJson.c_str() + rawJsonLength, &root,
                                    &err)) {
                    std::cout << "error" << std::endl;
                    
                    }
                }

                flag = root["is_update"].asBool();
                cards = string_ToVector(root["cards"].asString(), ',');
                //cards.push_back("723332123");
            }
            catch(const std::exception& e)
            {
                std::cerr << e.what() << '\n';
            }
        }


        std::vector<std::string> get_cards()
        {
            return cards;
        }
        bool get_flag()
        {
            return flag;
        }
};
