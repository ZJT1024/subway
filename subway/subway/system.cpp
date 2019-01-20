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


	char type;
	int station_x, station_y;
	int station_on_line;
	int station_id;
	int station_from, station_to;
	string station_name, line_name;

	while (in >> type)
	{
		if (type == '#')
		{
			in >> station_id >> station_name >> station_x >> station_y;

			station_num++; // Ϊ�˺͵������ͳһ��վ���Ŵ�1��ʼ
			station_set[station_num] = Station(station_id, station_x, station_y, station_name);
			station_dic[station_name] = station_id;
			decode_station[station_id] = station_name;

			// ��ʼʱÿ��վ�㶼û�з��ʹ�
			station_que.push(station_id);
			station_book[station_id] = 0;

		}
		else if (type == '%')
		{
			in >> line_name >> station_on_line;
			line_set[line_num] = Line(station_on_line, line_name);

			for (int i = 0; i < station_on_line; i++)
			{
				int temp_station_id;
				in >> temp_station_id;
				line_set[line_num].station_order.push_back(temp_station_id);
			}

			line_dic[line_name] = line_num;
			line_num++;
		}
		else if (type == '@')
		{
			// station_x   station_y Ϊ����վ��
			in >> station_from >> station_to >> line_name;

			if (line_name == "������")  // ӦΪ�������ǵ����ߣ���Ҫ�ֿ�����
			{
				graph[station_from].push_back(make_pair(station_to, line_dic[line_name]));
				continue;
			}

			graph[station_from].push_back(make_pair(station_to, line_dic[line_name]));
			graph[station_to].push_back(make_pair(station_from, line_dic[line_name]));

		}
	}

	in.close();
}

System::~System()
{
	if (station_set != NULL)
	{
		delete[] station_set;
	}
	if (line_set != NULL)
	{
		delete[] line_set;
	}
	if (station_book != NULL)
	{
		delete[] station_book;
	}

}

int System::distance[Maxn]; //�ڴ�������̬����

int System::Find_the_route(const string& start_station, const string& end_station, int transform, string& order)
{
	if (station_dic.find(start_station) == station_dic.end() || station_dic.find(end_station) == station_dic.end())
	{
		cout << "fail to find the correct station." << endl;
		return -1;
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
	station_book[sid] = 1;

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


	int temp_id = eid;
	int cost = 0;

	while (temp_id != sid)
	{
		cost++;
		order = "\n" + station_set[temp_id].name + order;
		station_book[temp_id] = 1;

		if (transform_station[temp_id] != 0)
		{
			order = " ����" + line_set[path[temp_id].second].name + order;
		}

		if (transform_station[temp_id] != 0)
		{
			exchange_times++;
		}
		temp_id = path[temp_id].first;
	}

	//order = station_set[temp_id].name + order;
	if (transform)
	{
		return (exchange_times * extral_cost + cost + 1);
	}
	else
	{
		return cost + 1;
	}
}

int System::Traversal(const string& now_station, string& order)
{
	int cost = 0;  // �������ⲻ�ÿ��ǻ������⣨һ��Ҫ���ɵġ�����
	int now = station_dic[now_station];
	string output;

	station_book[now] = 1;
	cost++;

	order = station_set[now].name + order;

	while (!station_que.empty())
	{
		int to = station_que.front();
		station_que.pop();

		string check = station_set[to].name;

		if (check == "�����")
		{
			int flag = 1;
			int flag1 = 1;
		}

		if (station_book[to])
		{
			continue;
		}

		output = "";
		string start_station = decode_station[now], end_station = decode_station[to];
		cost += Find_the_route(start_station, end_station, 0, output);
		cost--;//  ��ʼ��������һ��
		station_book[to] = 1;

		order += output;

		now = to;
	}

	output = "";
	cost += Find_the_route(decode_station[now], now_station, 0, output);
	order += output;

	return cost;
}
