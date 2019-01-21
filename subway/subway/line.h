#pragma once
#include"stdafx.h"
#include "headfile.h"

class Line
{
private:
	int num;  // 记录该条线上有多少个站点

public:
	string name;  // 线路名称
	vector<int> station_order;  // 站点的id组成的序列

	Line();
	Line(int, string);
	Line(const Line&);
	Line& operator = (const Line&);
	int Get_num();  // 返回该线路上站点的数量
};