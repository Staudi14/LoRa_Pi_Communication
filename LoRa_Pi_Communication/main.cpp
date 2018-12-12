#include <iostream>

using namespace std;

// LED Pin - wiringPi pin 0 is BCM_GPIO 17.
// we have to use BCM numbering when initializing with wiringPiSetupSys
// when choosing a different pin number please use the BCM numbering, also
// update the Property Pages - Build Events - Remote Post-Build Event command 
// which uses gpio export for setup for wiringPiSetupSys



int main(void)
{
	wiringPiSetupSys();
	

	while (true)
	{

	}
	return 0;
}