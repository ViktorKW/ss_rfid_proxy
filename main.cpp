#include "basic_lib.h"
using namespace std;
#include "CSerialInitializer.h"
#include "write_init.h"
#include "read_init.h"





int main()
{
    CSerialInitializer csi;
	sleep(5);
	write_init(csi);
	sleep(5);
	read_init(csi);
	
    return 0;   
}