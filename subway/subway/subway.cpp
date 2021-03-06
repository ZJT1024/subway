// subway.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include"headfile.h"
#include "system.h"


using namespace std;


int main()
{
	System* subway = new System;
	string order = "";  // 用于存放最后的输出结果

	fstream in, out;
	in.open("command.txt", ios::in);  // 输入命令的文件
	out.open("station_order.txt", ios::out);  //  输出结果的文件

	string command;
	in >> command;

	if (command == "/b")  // 求两点间的最短路
	{
		string start_station, end_station;
		int type;  //  换成车站是否消耗额外精力,type为0表示不消耗,为1表示消耗
		in >> start_station >> end_station >> type;

		int cost = subway->Find_the_route(start_station, end_station, type, order);
		order = start_station + order;

		out << cost << endl << order;
	}
	else if (command == "/a")  //  遍历整个地铁网

	{
		string start_station;
		in >> start_station;

		out << subway->Traversal(start_station, order) << endl << order;
	}
	else if (command == "/c")  // 输出一条线上的所有站点
	{
		string line_name;
		in >> line_name;

		int station_num = subway->Print_line(line_name, order);

		out << station_num << endl << order << endl;
	}
	else if (command == "/z")  //  测试遍历文件是否正确
	{
		string filename;
		in >> filename;

		subway->Check(filename , order);

		out << order << endl;
	}
	else  //  指令输入错误
	{
		out << "error command!" << endl;
	}

	in.close();
	out.close();

	return 0;
}

