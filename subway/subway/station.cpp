#include"stdafx.h"
#include "headfile.h"
#include "station.h"

using namespace std;

Station::Station()
{
	id = 0;
	x = 0;
	y = 0;
	name = "";
}

Station::Station(int temp_id, int temp_x, int temp_y, string temp_name)
{
	id = temp_id;
	x = temp_x;
	y = temp_y;
	name = temp_name;
}

Station::Station(const Station& a)
{
	id = a.id;
	x = a.x;
	y = a.y;
	name = a.name;
}

Station& Station::operator = (const Station& a)
{
	id = a.id;
	x = a.x;
	y = a.y;
	name = a.name;

	return *this;
}