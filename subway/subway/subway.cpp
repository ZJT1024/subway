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
	subway->Find_the_route("人民大学", "军事博物馆", 0);
    return 0;
}

