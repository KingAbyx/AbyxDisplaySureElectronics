#include "Arduino.h"
#include "AbyxMatrix.h"



AbyxMatrix::AbyxMatrix(char WRP, int WR, char DTP, int DT, char CS1P, int CS1, char CS2P, int CS2) {
	_WR = WR;
	_WRP = WRP;
	_DT = DT;
	_DTP = DTP;
	_CS1 = CS1;
	_CS1P = CS1P;
	_CS2 = CS2;
	_CS2P = CS2P;

	if (_WRP == 'B') DDRB |= _BV(_WR);
	if (_WRP == 'C') DDRC |= _BV(_WR);
	if (_WRP == 'D') DDRD |= _BV(_WR);

	if (_DTP == 'B') DDRB |= _BV(_DT);
	if (_DTP == 'C') DDRC |= _BV(_DT);
	if (_DTP == 'D') DDRD |= _BV(_DT);

	if (_CS1P == 'B') DDRB |= _BV(_CS1);
	if (_CS1P == 'C') DDRC |= _BV(_CS1);
	if (_CS1P == 'D') DDRD |= _BV(_CS1);

	if (_CS2P == 'B') DDRB |= _BV(_CS2);
	if (_CS2P == 'C') DDRC |= _BV(_CS2);
	if (_CS2P == 'D') DDRD |= _BV(_CS2);

	set_bit(_CS1P, _CS1, 1);
	set_bit(_CS2P, _CS2, 1);
}

void AbyxMatrix::set_bit(char pinp, int pin, boolean bit) {
	switch (pinp) {
		case 'B':
			if (bit) {
				PORTB |= _BV(pin);
			} else {
				PORTB &= ~_BV(pin);
			}
			break;
		case 'C':
			if (bit) {
				PORTC |= _BV(pin);
			} else {
				PORTC &= ~_BV(pin);
			}
			break;
		case 'D':
			if (bit) {
				PORTD |= _BV(pin);
			} else {
				PORTD &= ~_BV(pin);
			}
			break;
	}
}

void AbyxMatrix::send_bit(boolean bit) {
	set_bit(_DTP,_DT,bit);
	set_bit(_WRP,_WR,0);
	set_bit(_WRP,_WR,1);
}

void AbyxMatrix::send_screen(uint16_t newA[]) {
  int dat = 0b101;
  int addr = 0;
  int sWide = 48;
  int sHigh = 16;
  set_bit(_CS1P,_CS1,0);
  for (int i = 2; i >= 0; i--) {
    send_bit(bitRead(dat, i));
  }
  //Addresse
  for (int i = 6; i >= 0; i--) {
    send_bit(bitRead(addr, i));
  }
  //Data
  for (int i = 0; i < sWide / 2; i++) {
    for (int j = 0; j < sHigh; j++) {
      send_bit(bitRead(newA[i], j));
    }
  }
  set_bit(_CS1P,_CS1,1);

  set_bit(_CS2P,_CS2,0);
  for (int i = 2; i >= 0; i--) {
    send_bit(bitRead(dat, i));
  }
  //Addresse
  for (int i = 6; i >= 0; i--) {
    send_bit(bitRead(addr, i));
  }
  //Data
  for (int i = 0; i < sWide / 2; i++) {
    for (int j = 0; j < sHigh; j++) {
      send_bit(bitRead(newA[i + 24], j));
    }
  }
  set_bit(_CS2P,_CS2,1);
}

void AbyxMatrix::send_com(long comand, boolean isCS1) {
	if(isCS1) {
		set_bit(_CS1P, _CS1, 0);
	} else {
		set_bit(_CS2P, _CS2, 0);
	}
	for (int i = 11; i >= 0; i--) {
		send_bit(bitRead(comand, i));
	}
	if(isCS1) {
		set_bit(_CS1P, _CS1, 1);
	} else {
		set_bit(_CS2P, _CS2, 1);
	}
}

void AbyxMatrix::send_packet(int index, uint16_t data) {
  int dat = 0b101;
  int sWide = 48;
  int addr;
  if (index < sWide / 2) {
    set_bit(_CS1P,_CS1,0);
    addr = index * 4;
  } else {
    set_bit(_CS2P,_CS2,0);
    addr = ((index % 24) - 1) * 4;
  }
  for (int i = 2; i >= 0; i--) {
    send_bit(bitRead(dat, i));
  }
  //Addresse
  for (int i = 6; i >= 0; i--) {
    send_bit(bitRead(addr, i));
  }
  //Data
  for (int i = 0; i < 16; i++) {
    send_bit(bitRead(data, i));
  }
  if (index <= 23) {
    set_bit(_CS1P,_CS1,1);
  } else {
    set_bit(_CS2P,_CS2,1);
  }

}

void AbyxMatrix::send_screenSmart(uint16_t current[], uint16_t newA[]) {
  int sWide = 48;
  for (int x = 0; x < sWide; x++) {
    if (current[x] != newA[x]) {
      send_packet(x, newA[x]);
	  current[x] = newA[x];
    }
  }

}

void AbyxMatrix::init_comands() {
	long SYS_EN     = 0b100000000010;   //Enable Display System Oszillator
	long M_MODE     = 0b100000110000;   //Set Display MASTER-Mode
	long LED_ON     = 0b100000000110;   //Enable Display LEDs
	long LED_OFF    = 0b100000000100;   //Disable Display LEDs
	long NMOS_COM16 = 0b100001001000;   //Display Mode 24x16 NMOS  open Drain Output
	long PWM_04     = 0b100101000110;   //PWM:  4/16 dutycycle
	long PWM_08     = 0b100101001110;   //PWM:  8/16 dutycycle
	long PWM_16     = 0b100101011110;   //PWM: 16/16 dutycycle
	long BLK_ON     = 0b100000010010;   //Blink-Mode On
	long BLK_OFF    = 0b100000010000;   //Blink-Mode Off

	long MSBMASK   = 0b100000000000;            //Bitmaske zur MSB Selektierung bei Kommando 12 bit
	long MASK16    = 0b1000000000000000;        //Bitmaske zur MSB Selektierung bei Kommando 16 bit
	long MASK01    = 0b0000000000000001;        //Bitmaske zur MSB Selektierung bei Kommando 16 bit
	long MASK10    = 0b1000000000;              //Bitmaske zur MSB Selektierung bei Kommando 10 bit
	long MASK03    = 0b100;                     //Bitmaske zur MSB Selektierung bei Kommando 3 bit
	long MASKOBEN  = 0b0000000011111111;
	long MASKUNTEN = 0b1111111100000000;
	//Display 1
	send_com(SYS_EN, 1);                //Display System Oszillator einschalten
	send_com(M_MODE, 1);                   //Display auf MASTER-Mode einstellen
	send_com(NMOS_COM16, 1); //Display Modus 24x16 NMOS open Drain Output wÃ¤hlen
	send_com(LED_ON, 1);                            //Display LEDs freischalten
	send_com(PWM_16, 1);   //PWM: 16/16 dutycycle einstellen (volle Helligkeit)
	send_com(BLK_OFF, 1);                                //Blink-Mode aus

	//Display 2
	send_com(SYS_EN, 0);                //Display System Oszillator einschalten
	send_com(M_MODE, 0);                   //Display auf MASTER-Mode einstellen
	send_com(NMOS_COM16, 0); //Display Modus 24x16 NMOS open Drain Output wÃ¤hlen
	send_com(LED_ON, 0);                            //Display LEDs freischalten
	send_com(PWM_16, 0);   //PWM: 16/16 dutycycle einstellen (volle Helligkeit)
	send_com(BLK_OFF, 0);                                //Blink-Mode aus
}
