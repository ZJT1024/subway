#pragma once
#include"stdafx.h"
#include "headfile.h"

class Line
{
private:
	int num;

public:
	string name;
	vector<int> station_order;

	Line();
	Line(int, string);
	Line(const Line&);
	Line& operator = (const Line&);
};