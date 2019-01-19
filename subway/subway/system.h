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

	vector<pair<int, int >> graph[Maxn];  // ��¼ÿ��վ��ɴ��վ����Ϣ

	static int distance[Maxn];
	// ����ʹ�������ȶ��ж����·�㷨�����Ż�������Ҫ���رȽϷ���
	// ���ȽϷ��ŵĳ�ֵҪ��distance����Ϊ��̬��Ա����

	struct Cmp
	{
		bool operator()(pair<int, int> x, pair<int, int>y)
		{
			return distance[x.first] > distance[y.first];
			// �ù������ȶ��ж����·�㷨�����Ż�
		}
	};


	map<int, bool> station_book;  // ����ʱ��¼����Ƿ񱻷��ʹ�  pair��¼վ��id��bool��¼�Ƿ񱻷��ʹ�
	queue<int> station_que;  //  ��¼δ���ʹ��Ľ��
	map<int, string> decode_station;  // ��վ��id��Ӧվ������

public :
	System();
	int Find_the_route(const string& start_station , const string& end_station , int transform , string& order);
	int Traversal(const string& now_station , string& order);
};
