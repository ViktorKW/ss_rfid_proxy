#include "basic_lib.h"



std::string get_request(std::string url)
{
    curlpp::Cleanup cleanup;
    curlpp::Easy request;
    std::stringstream result;

    try
    {
        request.setOpt<curlpp::options::Url>(url);
        request.setOpt(cURLpp::Options::WriteStream(&result));
        request.perform();
    }
    catch(curlpp::RuntimeError & e)
    {
        std::cout << e.what() << std::endl;
    }
    catch(curlpp::LogicError & e)
    {
        std::cout << e.what() << std::endl;
    }
    return result.str();
}
