#pragma once
#include "stdafx.h"
#include "headfile.h"

class Station
{
private:
	int id;  //  վ��id
	int x, y;  //  վ������

public:
	string name;  // վ������
	Station();
	Station(int, int, int, string);
	Station(const Station&);
	Station& operator =(const Station&);
};