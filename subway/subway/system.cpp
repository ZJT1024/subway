#include"stdafx.h"
#include"station.h"
#include"line.h"
#include"system.h"
#include"headfile.h"

using namespace std;

System::System()
{
	fstream in;
	in.open("beijing-subway.txt", ios::in);

	if (!in.is_open())
	{
		cout << "fail to open the file." << endl;
		return;
	}

	station_num = 0;
	line_num = 0;
	station_dic.clear();
	line_dic.clear();

	station_set = new Station[Maxn];
	line_set = new Line[Maxm];


	char type;
	int station_id, station_x, station_y, line_id;
	string station_name, line_name;
	
	while (in >> type)
	{
		if (type == '@')
		{
			in >> station_id >> station_name >> station_x >> station_y;
			
			station_set[station_num] = Station(station_id, station_x, station_y, station_name);
			station_num++;
			station_dic[station_name] = station_id;
		}
		else if (type == '$')
		{
			in >> line_name >> line_id;

			line_set[line_num] = Line(line_id, line_name);
			line_num++;
			line_dic[line_name] = line_id;
		}
		else if(type == '&')
		{
			in >> station_x >> station_y >> station_num;

			if (station_name == "机场线")  // 应为机场线是单程线，需要分开处理
			{
				graph[station_x].push_back(make_pair(station_y, line_dic[line_name]));
				continue;
			}

			graph[station_x].push_back(make_pair(station_y, line_dic[line_name]));
			graph[station_y].push_back(make_pair(station_x, line_dic[line_name]));

		}
	}

	in.close();
}
