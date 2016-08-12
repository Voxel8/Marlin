#ifndef MARLIN_H
#define MARLIN_H


#include <iostream>
using namespace std;


#define SERIAL_PROTOCOLLNPGM(x) cout << x << endl
#define SERIAL_EOL cout << endl;
#define READ(x)  x
#define LOW 0
#define HIGH 1
#define WRITE(x,y) 0
#define PSTR(x) x
#define SERIAL_ERROR_START cout << "error start" << endl;

bool Running = true;

int CART0_SIG2_PIN = LOW;
int CART1_SIG2_PIN = LOW;
int CART1_SIG1_PIN = LOW;

void kill(const char* message) {
	cout << message << endl;
}



void serialprintPGM( const char* x) {
	cout << x;
}

bool IsRunning() {
	return true;
}

bool IsSafetyCriticalSection(){
	return false;
}


#endif
