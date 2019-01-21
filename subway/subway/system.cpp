#include"stdafx.h"
#include"station.h"
#include"line.h"
#include"system.h"
#include"headfile.h"

using namespace std;

System::System()
{
	fstream in;
	in.open("beijing-subway.txt", ios::in);  //  ����������

	if (!in.is_open())
	{
		cout << "fail to open the file." << endl;
		return;
	}

	// ��ʼ��
	station_num = 0;
	line_num = 0;
	station_dic.clear();
	line_dic.clear();


	char type;  //  �������ݵı�ʾ��
	int station_x, station_y;  // վ������
	int station_on_line;  // ĳ����·��վ������
	int station_id;  // վ��id
	int station_from, station_to;  //  ��ʼվ��id������վ��id
	string station_name, line_name;  //  վ�����ƣ���·����

	while (in >> type)
	{
		if (type == '#')  //  վ��
		{
			in >> station_id >> station_name >> station_x >> station_y;

			station_num++; // Ϊ�˺͵������ͳһ��վ���Ŵ�1��ʼ
			station_set[station_num] = Station(station_id, station_x, station_y, station_name);
			station_dic[station_name] = station_id;
			decode_station[station_id] = station_name;  //  ���룬���ڸ���վ��id�ҵ�վ������

			// ��ʼʱÿ��վ�㶼û�з��ʹ�
			station_que.push(station_id);
			station_book[station_id] = 0;

		}
		else if (type == '%')  //  ��·
		{
			in >> line_name >> station_on_line;
			line_set[line_num] = Line(station_on_line, line_name);

			for (int i = 0; i < station_on_line; i++)  // �������ϵ�վ����ӵ�������
			{
				int temp_station_id;
				in >> temp_station_id;
				line_set[line_num].station_order.push_back(temp_station_id);
			}

			line_dic[line_name] = line_num;
			line_num++;
		}
		else if (type == '@')   // ·��
		{
			// station_from ��ʼվ��id   station_to ����վ��id
			in >> station_from >> station_to >> line_name;

			if (line_name == "������")  // ӦΪ�������ǵ����ߣ���Ҫ�ֿ�����
			{
				graph[station_from].push_back(make_pair(station_to, line_dic[line_name]));
				continue;
			}


			//  ���˻�����֮�⣬������Ϊ˫��ͨ��
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

int System::Find_the_route(const string& start_station, const string& end_station, int transform, string& order)  // ʵ����Dijkstra
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

	priority_queue<pair<int, int>, vector<pair<int, int>>, Cmp> que;  //  ���ȶ��У������Ż�Dijkstra�㷨
	que.push(make_pair(sid, 0));  // �����������
	distance[sid] = 0;
	path[sid] = make_pair(sid, 0);
	station_book[sid] = 1;

	int exchange_times = 0;  // ��¼���˴���

	int flag = 0;  // ��¼�Ƿ��ҵ�Ŀ��վ��
	while (!que.empty())
	{
		now = que.top();  //  ��ǰ����վ��id
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
			to = graph[now.first][i];  // �ӵ�ǰվ����������ڵ�վ��id

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

					path[to.first] = make_pair(now.first, to.second);  //  ��¼·�����������
					if (path[now.first].second == 0)  // �ӿ�ʼ�����ڶ�û�л��˹�
					{
						transform_station[to.first] = 0;
					}
					else if (path[now.first].second == to.second)  //  �ӿ�ʼ�����ڣ��л��˹������ڸ�վ�㲻����
					{
						transform_station[to.first] = 0;
					}
					else  // �ڸ�վ�㻻��
					{
						transform_station[to.first] = to.second;
					}

					if (to.first == eid)  // �ҵ�Ŀ��վ��
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


	int temp_id = eid;  // ��ӡ·��
	int cost = 0;

	while (temp_id != sid)
	{
		cost++;
		order = "\n" + station_set[temp_id].name + order;
		station_book[temp_id] = 1;

		if (transform_station[temp_id] != 0)  // �л���
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
	if (transform)  //  ���û�Ҫ���������ģʽ
	{
		return (exchange_times * extral_cost + cost + 1);
	}
	else
	{
		return cost + 1;
	}
}

int System::Traversal(const string& now_station, string& order)  // ȫ����
{
	int cost = 0;  // �������ⲻ�ÿ��ǻ������⣨һ��Ҫ���ɵġ�����
	int now = station_dic[now_station];  // ��ǰ����վ��id
	string output;  // ���ڼ�¼û�ε���  ��������·�������

	station_book[now] = 1;
	cost++;

	order = station_set[now].name + order;  // �ܵ����

	while (!station_que.empty())
	{
		int to = station_que.front();  //  ��ʣ�µ�վ����ȡ��һ��
		station_que.pop();

		output = "";
		string start_station = decode_station[now], end_station = decode_station[to];
		cost += Find_the_route(start_station, end_station, 0, output);
		cost--;//  ��ʼ��������һ��
		station_book[to] = 1;

		order += output;

		now = to;
	}

	output = "";
	cost += Find_the_route(decode_station[now], now_station, 0, output);  //  �ص����
	order += output;

	return cost;
}

int System::Print_line(const string& line_name, string& order)
{
	if (line_dic.find(line_name) == line_dic.end())
	{
		order = "Error input!";
		return -1;
	}

	int line_id = line_dic[line_name];
	int num = line_set[line_id].Get_num();
	for (int i = 0; i < num; i++)
	{
		int station_id = line_set[line_id].station_order[i];

		order += station_set[station_id].name + "\n";
	}

	return num;
}

void System::Check(const string& filename, string& order)
{
	fstream file;
	file.open(filename, ios::in);

	if (!file.is_open())
	{
		order = "Something wrong with the file.";
		return;
	}

	int cost;
	string station_name;
	file >> cost;

	int visited[Maxn];
	int cnt = 0;
	int front, next;  // ��һվ����һվ����վ���ڣ�
	memset(visited, 0, sizeof(int)* Maxn);

	if (cost > 0)
	{
		file >> station_name;
		front = station_dic[station_name];
	}

	while (file >> station_name)
	{
		if (station_dic.find(station_name) == station_dic.end())  // ���ܻ������������ĳĳ�ߡ����ַ��������˵�
		{
			continue;
		}

		int next = station_dic[station_name];
		int flag = 0;

		for (int i = 0; i < graph[front].size(); i++)  // �������վ���Ƿ�����
		{
			if (next == graph[front][i].first)
			{
				flag = 1;
				break;
			}
		}

		if (flag == 0)
		{
			order = "error";
			return;
		}
		
		if (!visited[next])
		{
			visited[next] = 1;
			cnt++;
		}

		front = next;
	}

	int flag = 1;
	for (int i = 1; i <= station_dic.size(); i++)  // ����Ƿ�����վ�㶼�����ˣ����û�У���û������վ��д��order
	{
		if (!visited[i])
		{
			flag = 0;
			order += station_set[i].name + "\n";
		}
	}

	if (flag == 1)
	{
		order = "true";
	}
	else
	{
		order = "false\n" + order;
	}

	return;
}