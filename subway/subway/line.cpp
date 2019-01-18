
#include "stdafx.h"
#include "line.h"
#include "headfile.h"

using namespace std;

Line:: Line()
{
	num = 0;
	name = "";
	station_order.clear();
}

Line::Line(int temp_num = 0, string temp_name = "")
{
	num = temp_num;
	name = temp_name;
	station_order.clear();
}

Line::Line(const Line& a)
{
	num = a.num;
	name = a.name;
	station_order = a.station_order;
}

Line& Line::operator=(const Line& a)
{
	num = a.num;
	name = a.name;
	station_order = a.station_order;

	return *this;
}
