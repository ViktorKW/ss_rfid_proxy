#include "basic_lib.h"
#include "CSerial.h"
#include "CSynchronizedFile.h"
#include "string_ToVector.h"
using namespace std;
using namespace curlpp::options;
static string secret_code = "";
static string SIMPLY_ADD_CARDS_URL = "";
static auto synchronizedFile = std::make_shared<SynchronizedFile>("buffer.dat");
static auto synchronizedCorruptedFile = std::make_shared<SynchronizedFile>("logs.dat");

string clear_data(string data, char delimiter)
{
	try
	{
		data.erase(std::remove(data.begin(), data.end(), delimiter), data.end());
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	return data;
}


vector<string> get_serial_data(int amount_of_bytes, int serial)
{
	string str;
	try
	{
		for(int i = 0; i < amount_of_bytes; i++)
		{	 	
			str = str + (char)serialGetchar(serial);
		}
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}

	vector<string> split_data = string_ToVector(str, 'R'); 


	return split_data;
}


void send_url(string url)
{
	try
	{
		curlpp::Cleanup myCleanup;
		curlpp::Easy myRequest;
		myRequest.setOpt<Url>(url);
		myRequest.perform();
	}
	catch(curlpp::RuntimeError & e)
	{
		//std::cout << e.what() << std::endl;
		//cout<<"\nno internet connection"<<endl;
		Writer acsess_to_file(synchronizedFile);
		acsess_to_file.write_to_file(url);
	}
	catch(curlpp::LogicError & e)
	{
		//std::cout << e.what() << std::endl;
		Writer acsess_to_file(synchronizedFile);
		acsess_to_file.write_to_file(url);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		Writer acsess_to_file(synchronizedFile);
		acsess_to_file.write_to_file(url);
	}
}


void report_corrupted_card(string buff, string error)
{
	cout<<"Corrupted data: "<<error<<endl;
	time_t rawtime;
	struct tm * timeinfo;
	char time_str[80];

	time (&rawtime);
	timeinfo = localtime(&rawtime);

	strftime(time_str,sizeof(time_str),"%d-%m-%Y %H:%M:%S",timeinfo);
	std::string time_result(time_str);
	string result  = buff + " " + time_result;
	Writer acsess_to_file(synchronizedCorruptedFile);
	acsess_to_file.write_to_file(result);
}


void initialize_send(string data, int thread_id)
{
	string result;
	char entrance;
	try
	{

		vector<string> split_data = string_ToVector(data, ':'); 
		

		if(split_data.size() >= 3)
		{
			for(int i = 0; i<split_data.size(); i++)
			{
				split_data[i] = clear_data(split_data[i], '\n');
				split_data[i] = clear_data(split_data[i], '\r');
				split_data[i].erase(std::remove_if(split_data[i].begin(), split_data[i].end(),[](char c) { return !std::isdigit(c); }),split_data[i].end());
			}
			
			if (stoi(split_data[1]) % 2 == 0)
			{
				entrance = '2';
			}
			else 
			{
				entrance = '1';
			}


			if(thread_id>0)
			{
				result = SIMPLY_ADD_CARDS_URL + "/" + split_data[2] + "/" + entrance + "/" + to_string(thread_id)+split_data[1] + "/" + secret_code+"?date_time="+to_string(chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count());
			}
			else
			{
				result = SIMPLY_ADD_CARDS_URL + "/" + split_data[2] + "/" + entrance + "/" + split_data[1] + "/" + secret_code+"?date_time="+to_string(chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count());
			}

			cout<<"\n"<<"SENDING URL "<<result<<"\n";
			if(result.size() > 0)
			{
				send_url(result);
			}
		}
		else
		{
			report_corrupted_card(data, "request doesn't have 3 full parts");
		}
	}
	catch(const std::exception& e)
	{
		report_corrupted_card(data, "some of the parts are empty or have wrong symbols");
	}
}


void read_cards(CSerial cs, int thread_id)
{
    try
    {
		cout<<"\nSETTING UP READER FOR SERIAL "<<cs.serial_address<<" "<<cs.serial_port<<"\n";
		sleep(1);
		int serial = serialOpen(cs.serial_address.c_str(), cs.serial_port);
		if(serial>0)
        {
			int amount;

			while(true)
			{ 
				amount = serialDataAvail(serial); 
				//printf("\nAmount of bytes: %d\n", amount);
				if(amount>10)
				{
					try
					{
						vector<string> gained_cards = get_serial_data(amount, serial);
						/*string buff = get_serial_data(amount,  serial);*/
						for(int i = 0; i < gained_cards.size(); i++)
						{	
							if(gained_cards[i].length()>10)
							{	
								thread init_send(initialize_send, gained_cards[i], thread_id);
								init_send.detach();
							}
						}
					}
					catch(const std::exception& e)
					{
						std::cerr << e.what() << '\n';
					}
				}
				usleep(200000);
			}
		}
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
}