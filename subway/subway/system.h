#pragma once
#include "stdafx.h"
#include "headfile.h"
#include"station.h"
#include "line.h"

class System
{
private:
	int station_num;  // 该地铁网中站点总数
	Station  station_set[Maxn];  //  由站点组成的集合
	map<string, int > station_dic;  //  站点字典，将站点名字和站点id对应起来

	int line_num;  // 该地铁网中的线路总数
	Line line_set[Maxm];  //  由线路组成的集合
	map<string, int> line_dic;  //  线路字典，线路名字和线路id对应起来

	vector<pair<int, int >> graph[Maxn];  // 记录每个站点可达的站点信息，pair.first为可达的站点id，pair.second为该可达站点所在线

	static int distance[Maxn];
	// 由于使用了优先队列对最短路算法进行优化所以需要重载比较符号
	// 而比较符号的充值要求distance必须为静态成员变量

	struct Cmp
	{
		bool operator()(pair<int, int> x, pair<int, int>y)
		{
			return distance[x.first] > distance[y.first];
			// 用过用优先队列对最短路算法进行优化
		}
	};

	int station_book[Maxn];  //  打表记录，用于全遍历
	queue<int> station_que;  //  记录未访问过的结点
	map<int, string> decode_station;  // 将站点id对应站点名字

public:
	System();
	~System();
	int Find_the_route(const string& start_station, const string& end_station, int transform, string& order);  //  寻找两个站点之间的最短路
	int Traversal(const string& now_station, string& order);  //  全遍历
	int Print_line(const string&, string& order);  //  打印某条线路上的站点序列
	void Check(const string& filename, string& order);  //  检查filename中存放的遍历数据是否正确
};
