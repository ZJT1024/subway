// subway.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include"headfile.h"
#include "station.h"
#include "line.h"
#include "system.h"


using namespace std;


int main()
{
	System* subway = new System;
	string order = "\n";
	
	cout << subway->Find_the_route("魏公村", "良乡大学城", 1 , order) << endl << order;

    return 0;
}

