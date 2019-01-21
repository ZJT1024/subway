#pragma once
#include"stdafx.h"
#include "headfile.h"

class Line
{
private:
	int num;  // ��¼���������ж��ٸ�վ��

public:
	string name;  // ��·����
	vector<int> station_order;  // վ���id��ɵ�����

	Line();
	Line(int, string);
	Line(const Line&);
	Line& operator = (const Line&);
	int Get_num();  // ���ظ���·��վ�������
};