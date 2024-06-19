#include "basic_lib.h"
#include <mutex>
using namespace std;

class SynchronizedFile {
public:
    SynchronizedFile (const string& path) : _path(path) {
        // Open file for writing...
        //file_to_write.open(_path, ios::out | ios::app);
    }

    void write (const string& dataToWrite) {
        // Ensure that only one thread can execute at a time
        std::lock_guard<std::mutex> lock(_writerMutex);
        // Write to the file...
        file_to_write.open(_path, ios::out | ios::app);
        file_to_write<< dataToWrite << endl;
        file_to_write.close();
    }

    void empty_file()
    {
        std::lock_guard<std::mutex> lock(_writerMutex);
        //file_to_write.close();
		file_to_write.open(_path, ofstream::out | ofstream::trunc);
        file_to_write.close();
        //file_to_write.open(_path, ios::out | ios::app);
    }

    bool is_empty_file()
    {
        std::lock_guard<std::mutex> lock(_writerMutex);
        int result;
        //file_to_write.close();
        ifstream file_to_read; 
		file_to_read.open(_path);
        if(file_to_read.peek()==EOF)
		{
            result = false;
        }
        else
        {
            result = true;
        }
        file_to_read.close();

        //file_to_write.open(_path, ios::out | ios::app);
        return result;
    }

    string get_file_data()
    {
        file_to_read.open(_path);
        string content((istreambuf_iterator<char>(file_to_read)), istreambuf_iterator<char>());
        file_to_read.close();
        return content;    
    }
private:
    string _path;
    std::mutex _writerMutex;
    ofstream file_to_write;
    ifstream file_to_read;
};

class Writer {
public:
    Writer (std::shared_ptr<SynchronizedFile> sf) : _sf(sf) {}

    void write_to_file (const string& dataToWrite) {
        _sf->write(dataToWrite);
    }
    void empty_file()
    {
        _sf->empty_file();
    }
    bool is_file_empty()
    {
        return _sf->is_empty_file();
    }
    string get_file_content()
    {
        return _sf->get_file_data();
    }
private:
    std::shared_ptr<SynchronizedFile> _sf;
};