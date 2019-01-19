#include"stdafx.h"
#include"station.h"
#include"line.h"
#include"system.h"
#include"headfile.h"

using namespace std;

System::System()
{
	fstream in;
	in.open("in.txt", ios::in);

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
		if (type == '#')
		{
			in >> station_id >> station_name >> station_x >> station_y;
			
			station_num++; // Ϊ�˺͵������ͳһ��վ���Ŵ�1��ʼ
			station_set[station_num] = Station(station_id, station_x, station_y, station_name);
			station_dic[station_name] = station_id;
		}
		else if (type == '%')
		{
			in >> line_name >> line_id;


			line_set[line_num] = Line(line_id, line_name);
			line_dic[line_name] = line_id;
			line_num++;
		}
		else if(type == '@')
		{
			in >> station_x >> station_y >> station_num;

			if (station_name == "������")  // ӦΪ�������ǵ����ߣ���Ҫ�ֿ�����
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

int System::distance[Maxn]; //�ڴ�������̬����

void System::Find_the_route(const string& start_station, const string& end_station, int transform)
{
	if (station_dic.find(start_station) == station_dic.end() || station_dic.find(end_station) == station_dic.end())
	{
		cout << "fail to find the correct station." << endl;
	}
	 
	int sid = station_dic[start_station], eid = station_dic[end_station];
	
	int transform_station[Maxn]; // ����·����ӡ
	int visited[Maxn];

	memset(distance, INF, sizeof(int) * Maxn);
	memset(visited, 0, sizeof(int) * Maxn);
	memset(transform_station, 0, sizeof(int) * Maxn);

	vector<pair<int, int>> path(Maxn);	// ��¼·��  pair.first��վ��id��pair.second��������
	pair<int, int > now, to; // ��ǰ�ڵ㣬��ѡ�ڵ㣨վ�㣩firstΪվ��id��secondΪ������

	priority_queue<pair<int, int>, vector<pair<int, int>>, Cmp> que;
	que.push(make_pair(sid, 0));
	distance[sid] = 0;
	path[sid] = make_pair(sid, 0);

	int exchange_times = 0;

	int flag = 0;
	while (!que.empty())
	{
		now = que.top();
		que.pop();

		if (visited[now.first])
		{
			continue;
		}
		else
		{
			visited[now.first] = 1;
		}

		// Ѱ�Ҵӵ�ǰ�ڵ㿪ʼ�ܵ����վ������·
		for (int i = 0; i < graph[now.first].size(); i++)
		{
			to = graph[now.first][i];

			if (visited[to.first])
			{
				continue;
			}
			else
			{
				int cost = distance[now.first] + 1;  // Ĭ��ÿ��·����ȨֵΪ1

				if (path[now.first].second != 0 && path[now.first].second != to.second)
				{
					// �ж��Ƿ���Ҫ���ɣ������Ҫ�Ļ������Ķ���ľ���
					cost += extral_cost * transform;
				}

				if (distance[to.first] > cost)
				{
					distance[to.first] = cost;  // �������·
					que.push(to);

					path[to.first] = make_pair(now.first, to.second);
					if (path[now.first].second == 0)
					{
						transform_station[to.first] = 0;
					}
					else if (path[now.first].second == to.second)
					{
						transform_station[to.first] = 0;
					}
					else
					{
						transform_station[to.first] = to.second;
					}

					if (to.first == eid)
					{
						flag = 1;
						break;
					}
				}
			}
		}
		if (flag == 1)
		{
			break;
		}
	}

	/*for (int i = 0; i < path.size(); i++)
	{
		int temp_id = path[i].first;
		string temp_name = station_set[temp_id].name;
	}*/


	string str = "\n";
	int temp_id = eid;
	int cost = 0;

	while (temp_id != sid)
	{
		cost++;
		str = "\n" + station_set[temp_id].name + str;

		if (transform_station[temp_id] != 0)
		{
			str = " ����" + line_set[path[temp_id].second].name + str;
		}

		if (transform_station[temp_id] != 0)
		{
			exchange_times++;
		}
		temp_id = path[temp_id].first;
	}

	str = station_set[temp_id].name + str;
	if (transform)
	{
		cout << exchange_times * extral_cost + cost + 1 << endl << str;
	}
	else
	{
		cout << cost + 1 << endl << str;
	}
}
