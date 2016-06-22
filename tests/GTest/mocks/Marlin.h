#ifndef MARLIN_H
#define MARLIN_H


#include <iostream>
using namespace std;


#define SERIAL_PROTOCOLLNPGM(x) cout << x
#define SERIAL_EOL cout << " "
#define READ(x)  x
#define LOW 0
#define HIGH 1
#define WRITE(x,y) 0
#define PSTR(x) x
#define MSG_T_CARTRIDGE_REMOVED "test"
#define SERIAL_ERROR_START cout << "error start"

bool Running = true;

int CART0_SIG2_PIN = LOW;
int CART1_SIG2_PIN = LOW;
int CART1_SIG1_PIN = LOW;

void kill(const char* message) {
	cout << message;
}
void disable_all_heaters() {

}

void disable_all_steppers() {

}

void quickStop() {

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