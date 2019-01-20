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
			int cost = test->Find_the_route("κ����", "�����ѧ", 0, order);

			Assert::AreEqual(cost, 2);

			// TODO: �ڴ�������Դ���
		}
		TEST_METHOD(boundary_test2_oneStation)
		{
			System* test = new System();
			string order = "";
			int cost = test->Find_the_route("κ����", "κ����", 0, order);

			Assert::AreEqual(cost, 1);

			// TODO: �ڴ�������Դ���
		}
		TEST_METHOD(data_correct_test)
		{
			System* test = new System();
			string order = "";
			int cost = test->Find_the_route("κ����", "����", 0, order);

			Assert::AreEqual(cost, -1);

			// TODO: �ڴ�������Դ���
		}
		TEST_METHOD(normal_test)
		{
			System* test = new System();
			string order = "";
			int cost = test->Find_the_route("κ����", "�����ѧ��", 0, order);

			Assert::AreEqual(cost, 21);

			// TODO: �ڴ�������Դ���
		}
		TEST_METHOD(long_distance_test)
		{
			System* test = new System();
			string order = "";
			int cost = test->Find_the_route("��ׯ", "ʯ��", 0, order);

			Assert::AreEqual(cost, 44);

			// TODO: �ڴ�������Դ���
		}
		TEST_METHOD(transfer_station)
		{
			System* test = new System();
			string order = "";
			int cost = test->Find_the_route("κ����", "��ɽ", 1, order);

			Assert::AreEqual(cost, 16);

			// TODO: �ڴ�������Դ���
		}
		TEST_METHOD(not_transfer_station)
		{
			System* test = new System();
			string order = "";
			int cost = test->Find_the_route("κ����", "��ɽ", 0, order);

			Assert::AreEqual(cost, 10);

			// TODO: �ڴ�������Դ���
		}
		TEST_METHOD(on_the_same_line_not_transfer)
		{
			System* test = new System();
			string order = "";
			int cost = test->Find_the_route("ƻ��԰", "�Ļݶ�", 0, order);

			Assert::AreEqual(cost, 23);

			// TODO: �ڴ�������Դ���
		}
		TEST_METHOD(on_the_same_line_transfer)
		{
			System* test = new System();
			string order = "";
			int cost = test->Find_the_route("ƻ��԰", "�Ļݶ�", 1, order);

			Assert::AreEqual(cost, 23);

			// TODO: �ڴ�������Դ���
		}
		TEST_METHOD(loop_line)
		{
			System* test = new System();
			string order = "";
			int cost = test->Find_the_route("��ֱ��", "��ֱ��", 0, order);

			Assert::AreEqual(cost, 6);

			// TODO: �ڴ�������Դ���
		}
		TEST_METHOD(one_way_traffic1)
		{
			System* test = new System();
			string order = "";
			int cost = test->Find_the_route("κ����", "2�ź�վ¥", 0, order);

			Assert::AreEqual(cost, 16);

			// TODO: �ڴ�������Դ���
		}
		TEST_METHOD(one_way_traffic2)
		{
			System* test = new System();
			string order = "";
			int cost = test->Find_the_route("2�ź�վ¥", "κ����", 0, order);

			Assert::AreEqual(cost, 15);

			// TODO: �ڴ�������Դ���
		}
	};
}