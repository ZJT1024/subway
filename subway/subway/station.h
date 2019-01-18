#pragma once
#include "stdafx.h"
#include "headfile.h"

class Station
{
private:
	int id;
	int x, y;

public:
	string name;
	Station();
	Station(int, int, int, string);
	Station(const Station&);
	Station& operator =(const Station&);
};