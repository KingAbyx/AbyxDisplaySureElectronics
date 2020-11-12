#ifndef AbyxMatrix_h
#define AbyxMatrix_h

#include "Arduino.h"

class AbyxMatrix
{
	public:
		AbyxMatrix(char WRP, int WR, char DTP, int DT, char CS1P, int CS1, char CS2P, int CS2);
		void set_bit(char pinp, int pin, boolean bit);
		void send_bit(boolean bit);
		void send_screen(uint16_t newA[]);
		void send_com(long comand, boolean isCS1);
		void send_packet(int index, uint16_t data);
		void send_screenSmart(uint16_t current[], uint16_t newA[]);
		void init_comands();
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