#ifndef AbyxMatrix_h
#define AbyxMatrix_h

#include "Arduino.h"

class AbyxMatrix
{
	public:
		AbyxMatrix(char WRP, int WR, char DTP, int DT, char CS1P, int CS1, char CS2P, int CS2);
		void setBit(char pinp, int pin, boolean bit);
		void sendBit(boolean bit);
		void sendScreen(uint16_t newA[]);
		void sendCom(long comand, boolean isCS1);
		void sendPacket(int index, uint16_t data);
		void sendScreenSmart(uint16_t current[], uint16_t newA[]);
		void initComands();
	private:
		int _WR;
		char _WRP;
		int _DT;
		char _DTP;
		int _CS1;
		char _CS1P;
		int _CS2;
		char _CS2P;
};

#endif