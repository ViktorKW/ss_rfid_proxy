#include "basic_lib.h"

#pragma once
std::vector<std::string> string_ToVector(std::string str, char delimiter)
{
    std::vector<std::string> internal; 
    try
    {
        std::stringstream ss(str);
        std::string tok; 
        
        while(getline(ss, tok, delimiter)) 
        { 
            internal.push_back(tok); 
        } 
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    return internal;
}   
