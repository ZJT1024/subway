#pragma once
#include "stdafx.h"
#include "headfile.h"
#include"station.h"
#include "line.h"

class System
{
private :
	int station_num;
	Station * station_set;
	map<string, int > station_dic;

	int line_num;
	Line * line_set;
	map<string, int> line_dic;

	vector<pair<int, int >> *graph;

public :
	System();
	void Find_the_route(const string& start_station , const string& end_station , int transform);
};
