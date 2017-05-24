#pragma once

#ifndef D_INPUT
#define D_INPUT
#include <dinput.h>
#include <iostream>
#endif

#include "Device.h"





class Joystick : public Device
{

	// ustawienia
	DIDEVCAPS capabilities;




public:
	Joystick();
	HRESULT poll(DIJOYSTATE *js);
	void GetDesktopResolution(int & horizontal, int & vertical);
	void close();
	std::list<int> getData() {
		std::list<int> lista;
		return lista;
	}
	int start();
	~Joystick();


};

