// subway.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include"headfile.h"
#include "station.h"
#include "line.h"
#include "system.h"


using namespace std;


int main()
{
	System* subway = new System;
	string order = "";
	
	string command;
	cin >> command;

	if (command == "/b")
	{
		string start_station, end_station;
		cin >> start_station >> end_station;

		int cost = subway->Find_the_route(start_station, end_station, 0, order);
		order = start_station + order;

		cout << cost << endl << order;
	}
	else if (command == "/a")

	{
		string start_station;
		cin >> start_station;

		fstream out;
		out.open("out.txt", ios::out);

		out << subway->Traversal(start_station, order) << endl << order;
		out.close();
	}

    return 0;
}

