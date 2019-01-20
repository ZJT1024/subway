// subway.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include"headfile.h"
#include "system.h"


using namespace std;


int main()
{
	System* subway = new System;
	string order = "";

	fstream in, out;
	in.open("command.txt", ios::in);
	out.open("station_order.txt", ios::out);

	string command;
	in >> command;

	if (command == "/b")
	{
		string start_station, end_station;
		int type;
		in >> start_station >> end_station >> type;

		int cost = subway->Find_the_route(start_station, end_station, type, order);
		order = start_station + order;

		out << cost << endl << order;
		//cout << cost << endl << order << endl;
	}
	else if (command == "/a")

	{
		string start_station;
		in >> start_station;

		out << subway->Traversal(start_station, order) << endl << order;
	}

	in.close();
	out.close();

	return 0;
}

