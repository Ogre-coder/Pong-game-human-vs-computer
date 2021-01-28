#pragma once

#include "Serial.h"
#ifndef CCONTROL_H
#define CCONTROL_H

class CControl
{

private:
	Serial _com;

public:

	int num1, num2, num3, num4;
	int pos1, pos2;
	long time = 0;
	long debounceDelay = 25;
	long time2 = 0;;
	long debounceDelay1 = 13;

	CControl();
	~CControl();
	void init_com(int comport);
	bool get_data(int type, int channel, int& result);
	bool set_data(int type, int channel, int val);
	bool get_analog();
	bool get_digital();
	bool get_button(int channel);
	bool get_servo();
	bool button_count;

};

#endif
