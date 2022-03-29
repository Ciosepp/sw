#ifndef rEncoder_h
#define rEncoder_h

#include "Arduino.h"

class rEncoder
{
	public:
		rEncoder(uint8_t pinA, uint8_t pinB, uint8_t pinPB, bool EN_STATE);

		int updateEncoder(int oldVal);

		bool getPBState();

		bool getLongPress(uint8_t lpTime);

	private:
		uint8_t Apin;
		uint8_t Bpin;
		uint8_t PBpin;
		
		bool HIstate ;
		uint32_t tBuffer;
		bool oldPress = false;

};
#endif