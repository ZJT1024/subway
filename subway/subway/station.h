#pragma once
#include "stdafx.h"
#include "headfile.h"

class Station
{
private:
	int id;  //  站点id
	int x, y;  //  站点坐标

public:
	string name;  // 站点名称
	Station();
	Station(int, int, int, string);
	Station(const Station&);
	Station& operator =(const Station&);
};