#include "stdafx.h"
#include "CppUnitTest.h"
#include"../subway/headfile.h"
#include"../subway/line.h"
#include"../subway/line.cpp"
#include"../subway/station.h"
#include"../subway/station.cpp"
#include"../subway/system.h"
#include"../subway/system.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace UnitTest1
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(boundary_test1_twoStations)
		{
			System* test = new System();
			string order = "";
			int cost = test->Find_the_route("魏公村", "人民大学", 0, order);

			Assert::AreEqual(cost, 2);

			// TODO: 在此输入测试代码
		}
		TEST_METHOD(boundary_test2_oneStation)
		{
			System* test = new System();
			string order = "";
			int cost = test->Find_the_route("魏公村", "魏公村", 0, order);

			Assert::AreEqual(cost, 1);

			// TODO: 在此输入测试代码
		}
		TEST_METHOD(data_correct_test)
		{
			System* test = new System();
			string order = "";
			int cost = test->Find_the_route("魏公村", "北京", 0, order);

			Assert::AreEqual(cost, -1);

			// TODO: 在此输入测试代码
		}
		TEST_METHOD(normal_test)
		{
			System* test = new System();
			string order = "";
			int cost = test->Find_the_route("魏公村", "良乡大学城", 0, order);

			Assert::AreEqual(cost, 21);

			// TODO: 在此输入测试代码
		}
		TEST_METHOD(long_distance_test)
		{
			System* test = new System();
			string order = "";
			int cost = test->Find_the_route("苏庄", "石门", 0, order);

			Assert::AreEqual(cost, 44);

			// TODO: 在此输入测试代码
		}
		TEST_METHOD(transfer_station)
		{
			System* test = new System();
			string order = "";
			int cost = test->Find_the_route("魏公村", "香山", 1, order);

			Assert::AreEqual(cost, 16);

			// TODO: 在此输入测试代码
		}
		TEST_METHOD(not_transfer_station)
		{
			System* test = new System();
			string order = "";
			int cost = test->Find_the_route("魏公村", "香山", 0, order);

			Assert::AreEqual(cost, 10);

			// TODO: 在此输入测试代码
		}
		TEST_METHOD(on_the_same_line_not_transfer)
		{
			System* test = new System();
			string order = "";
			int cost = test->Find_the_route("苹果园", "四惠东", 0, order);

			Assert::AreEqual(cost, 23);

			// TODO: 在此输入测试代码
		}
		TEST_METHOD(on_the_same_line_transfer)
		{
			System* test = new System();
			string order = "";
			int cost = test->Find_the_route("苹果园", "四惠东", 1, order);

			Assert::AreEqual(cost, 23);

			// TODO: 在此输入测试代码
		}
		TEST_METHOD(loop_line)
		{
			System* test = new System();
			string order = "";
			int cost = test->Find_the_route("西直门", "东直门", 0, order);

			Assert::AreEqual(cost, 6);

			// TODO: 在此输入测试代码
		}
		TEST_METHOD(one_way_traffic1)
		{
			System* test = new System();
			string order = "";
			int cost = test->Find_the_route("魏公村", "2号航站楼", 0, order);

			Assert::AreEqual(cost, 16);

			// TODO: 在此输入测试代码
		}
		TEST_METHOD(one_way_traffic2)
		{
			System* test = new System();
			string order = "";
			int cost = test->Find_the_route("2号航站楼", "魏公村", 0, order);

			Assert::AreEqual(cost, 15);

			// TODO: 在此输入测试代码
		}
	};
}