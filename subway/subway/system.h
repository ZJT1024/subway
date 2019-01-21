#pragma once
#include "stdafx.h"
#include "headfile.h"
#include"station.h"
#include "line.h"

class System
{
private:
	int station_num;  // �õ�������վ������
	Station  station_set[Maxn];  //  ��վ����ɵļ���
	map<string, int > station_dic;  //  վ���ֵ䣬��վ�����ֺ�վ��id��Ӧ����

	int line_num;  // �õ������е���·����
	Line line_set[Maxm];  //  ����·��ɵļ���
	map<string, int> line_dic;  //  ��·�ֵ䣬��·���ֺ���·id��Ӧ����

	vector<pair<int, int >> graph[Maxn];  // ��¼ÿ��վ��ɴ��վ����Ϣ��pair.firstΪ�ɴ��վ��id��pair.secondΪ�ÿɴ�վ��������

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

	int station_book[Maxn];  //  ����¼������ȫ����
	queue<int> station_que;  //  ��¼δ���ʹ��Ľ��
	map<int, string> decode_station;  // ��վ��id��Ӧվ������

public:
	System();
	~System();
	int Find_the_route(const string& start_station, const string& end_station, int transform, string& order);  //  Ѱ������վ��֮������·
	int Traversal(const string& now_station, string& order);  //  ȫ����
	int Print_line(const string&, string& order);  //  ��ӡĳ����·�ϵ�վ������
	void Check(const string& filename, string& order);  //  ���filename�д�ŵı��������Ƿ���ȷ
};
