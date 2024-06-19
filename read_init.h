#include "read_cards.h"
#include "CSerialInitializer.h"
#include "string_ToVector.h"


void buffer_read()
{
	try
	{
        Writer file_operator(synchronizedFile);
		while(true)
		{
			cout<<"\n\nreader_threat      started\n"<<endl;
			if(file_operator.is_file_empty())
			{
				if(!system("ping -c1 -s1 www.google.com"))
				{	
					string content = file_operator.get_file_content();
					file_operator.empty_file();
					vector<string> split_data = string_ToVector(content, '\n'); 
					for(int i =0; i<split_data.size(); i++)
					{
						send_url(split_data[i]);
						std::this_thread::sleep_for(std::chrono::seconds(1)); 
					}
				}
			}
			cout<<"\n\nreader_threat      finish\n"<<endl;
			std::this_thread::sleep_for(std::chrono::seconds(120)); 
		}
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
}


void read_init(CSerialInitializer _csi)
{
	std::vector<std::thread> threads;

    for(int i =0; i<_csi.v_serial.size(); i++)
    { 
        threads.push_back(std::thread(read_cards, _csi.v_serial[i], i));
    }
     
	thread check_internet(buffer_read);
    check_internet.detach();

    for(auto& thread : threads){
        thread.join();
    }
}