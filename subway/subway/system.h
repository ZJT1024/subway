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

	vector<pair<int, int >> graph[Maxn];  // 记录每个站点可达的站点信息

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


	map<int, bool> station_book;  // 遍历时记录结点是否被访问过  pair记录站点id，bool记录是否被访问过
	queue<int> station_que;  //  记录未访问过的结点
	map<int, string> decode_station;  // 将站点id对应站点名字

public :
	System();
	int Find_the_route(const string& start_station , const string& end_station , int transform , string& order);
	int Traversal(const string& now_station , string& order);
};
