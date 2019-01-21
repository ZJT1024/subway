#include"stdafx.h"
#include"station.h"
#include"line.h"
#include"system.h"
#include"headfile.h"

using namespace std;

System::System()
{
	fstream in;
	in.open("beijing-subway.txt", ios::in);  //  构建地铁网

	if (!in.is_open())
	{
		cout << "fail to open the file." << endl;
		return;
	}

	// 初始化
	station_num = 0;
	line_num = 0;
	station_dic.clear();
	line_dic.clear();


	char type;  //  区分数据的表示符
	int station_x, station_y;  // 站点坐标
	int station_on_line;  // 某条线路上站点总数
	int station_id;  // 站点id
	int station_from, station_to;  //  起始站点id，到达站点id
	string station_name, line_name;  //  站点名称，线路名称

	while (in >> type)
	{
		if (type == '#')  //  站点
		{
			in >> station_id >> station_name >> station_x >> station_y;

			station_num++; // 为了和地铁编号统一，站点编号从1开始
			station_set[station_num] = Station(station_id, station_x, station_y, station_name);
			station_dic[station_name] = station_id;
			decode_station[station_id] = station_name;  //  解码，用于根据站点id找到站点名字

			// 初始时每个站点都没有访问过
			station_que.push(station_id);
			station_book[station_id] = 0;

		}
		else if (type == '%')  //  线路
		{
			in >> line_name >> station_on_line;
			line_set[line_num] = Line(station_on_line, line_name);

			for (int i = 0; i < station_on_line; i++)  // 将该线上的站点添加的序列中
			{
				int temp_station_id;
				in >> temp_station_id;
				line_set[line_num].station_order.push_back(temp_station_id);
			}

			line_dic[line_name] = line_num;
			line_num++;
		}
		else if (type == '@')   // 路径
		{
			// station_from 起始站点id   station_to 到达站点id
			in >> station_from >> station_to >> line_name;

			if (line_name == "机场线")  // 应为机场线是单程线，需要分开处理
			{
				graph[station_from].push_back(make_pair(station_to, line_dic[line_name]));
				continue;
			}


			//  除了机场线之外，其他的为双向通行
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

int System::distance[Maxn]; //在此申明静态变量

int System::Find_the_route(const string& start_station, const string& end_station, int transform, string& order)  // 实质是Dijkstra
{
	if (station_dic.find(start_station) == station_dic.end() || station_dic.find(end_station) == station_dic.end())
	{
		cout << "fail to find the correct station." << endl;
		return -1;
	}

	int sid = station_dic[start_station], eid = station_dic[end_station];

	int transform_station[Maxn]; // 用于路径打印
	int visited[Maxn];

	memset(distance, INF, sizeof(int) * Maxn);
	memset(visited, 0, sizeof(int) * Maxn);
	memset(transform_station, 0, sizeof(int) * Maxn);

	vector<pair<int, int>> path(Maxn);	// 记录路径  pair.first是站点id，pair.second是所在线
	pair<int, int > now, to; // 当前节点，待选节点（站点）first为站点id，second为所在线

	priority_queue<pair<int, int>, vector<pair<int, int>>, Cmp> que;  //  优先队列，用于优化Dijkstra算法
	que.push(make_pair(sid, 0));  // 将起点加入队列
	distance[sid] = 0;
	path[sid] = make_pair(sid, 0);
	station_book[sid] = 1;

	int exchange_times = 0;  // 记录换乘次数

	int flag = 0;  // 记录是否找到目标站点
	while (!que.empty())
	{
		now = que.top();  //  当前所以站点id
		que.pop();

		if (visited[now.first])
		{
			continue;
		}
		else
		{
			visited[now.first] = 1;
		}

		// 寻找从当前节点开始能到达的站点的最短路
		for (int i = 0; i < graph[now.first].size(); i++)
		{
			to = graph[now.first][i];  // 从当前站点出发，相邻的站点id

			if (visited[to.first])
			{
				continue;
			}
			else
			{
				int cost = distance[now.first] + 1;  // 默认每条路径的权值为1

				if (path[now.first].second != 0 && path[now.first].second != to.second)
				{
					// 判断是否需要换成，如果需要的话，消耗额外的精力
					cost += extral_cost * transform;
				}

				if (distance[to.first] > cost)
				{
					distance[to.first] = cost;  // 更新最短路
					que.push(to);

					path[to.first] = make_pair(now.first, to.second);  //  记录路径，用于输出
					if (path[now.first].second == 0)  // 从开始到现在都没有换乘过
					{
						transform_station[to.first] = 0;
					}
					else if (path[now.first].second == to.second)  //  从开始到现在，有换乘过，但在该站点不换乘
					{
						transform_station[to.first] = 0;
					}
					else  // 在该站点换乘
					{
						transform_station[to.first] = to.second;
					}

					if (to.first == eid)  // 找到目标站点
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


	int temp_id = eid;  // 打印路径
	int cost = 0;

	while (temp_id != sid)
	{
		cost++;
		order = "\n" + station_set[temp_id].name + order;
		station_book[temp_id] = 1;

		if (transform_station[temp_id] != 0)  // 有换乘
		{
			order = " 换乘" + line_set[path[temp_id].second].name + order;
		}

		if (transform_station[temp_id] != 0)
		{
			exchange_times++;
		}
		temp_id = path[temp_id].first;
	}

	//order = station_set[temp_id].name + order;
	if (transform)  //  看用户要求的是那种模式
	{
		return (exchange_times * extral_cost + cost + 1);
	}
	else
	{
		return cost + 1;
	}
}

int System::Traversal(const string& now_station, string& order)  // 全遍历
{
	int cost = 0;  // 遍历问题不用考虑换乘问题（一定要换成的。。）
	int now = station_dic[now_station];  // 当前所在站点id
	string output;  // 用于记录没次调用  两点间最短路径的输出

	station_book[now] = 1;
	cost++;

	order = station_set[now].name + order;  // 总的输出

	while (!station_que.empty())
	{
		int to = station_que.front();  //  从剩下的站点中取出一个
		station_que.pop();

		output = "";
		string start_station = decode_station[now], end_station = decode_station[to];
		cost += Find_the_route(start_station, end_station, 0, output);
		cost--;//  起始结点多算了一次
		station_book[to] = 1;

		order += output;

		now = to;
	}

	output = "";
	cost += Find_the_route(decode_station[now], now_station, 0, output);  //  回到起点
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
	int front, next;  // 上一站，下一站（两站相邻）
	memset(visited, 0, sizeof(int)* Maxn);

	if (cost > 0)
	{
		file >> station_name;
		front = station_dic[station_name];
	}

	while (file >> station_name)
	{
		if (station_dic.find(station_name) == station_dic.end())  // 可能会读到，“换乘某某线”的字符串，过滤掉
		{
			continue;
		}

		int next = station_dic[station_name];
		int flag = 0;

		for (int i = 0; i < graph[front].size(); i++)  // 检查两个站点是否相邻
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
	for (int i = 1; i <= station_dic.size(); i++)  // 检查是否所有站点都遍历了，如果没有，将没遍历的站点写入order
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