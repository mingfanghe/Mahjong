// mahjong.cpp : 定义控制台应用程序的入口点。
//简单的麻将算法,找出可吃、碰、杠和胡的牌
//麻将的表示:1-9:万
//            11-19:筒
//            21-29:条
//            31-34:东南西北
//            41-43:中白发

#include "stdafx.h"
#include<vector>
#include<iostream>
#include <algorithm>    // std::sort
#include <stdlib.h>  
#include<string.h>
#include <memory.h>
#include <fstream>
#include <sstream>
#include <cstdio>
#include <functional>

using namespace std;

bool ishu;//判胡标志
int HuMahjong[44];//计数存储数组,存每个牌的张数,最大数为44,数组长度为44
vector<int> vecPeng;
vector<int> vecGang;
vector<vector<int> > vecChi;
vector<vector<int> > HuType;
vector<vector<int> > OutHuType;
vector<int> HuTemp;

void Peng(int PengPai[])
{
	vecPeng.clear();
	for (int i = 1; i < 44; i++)
	{
		if ((PengPai[i] == 2) || (PengPai[i] == 3))
		{
			vecPeng.push_back(i);
		}
	}
}

void Gang(int GangPai[])
{
	vecGang.clear();
	for (int i = 1; i < 44; i++)
	{
		if (GangPai[i] == 3)
		{
			vecGang.push_back(i);
		}
	}
}

void Chi(int ChiPai[])
{
	vecChi.clear();
	int Rows = 0;
	for (int i = 0; i < 30; i++)
	{
		vector<int> temp;
		if ((ChiPai[i] > 0) && (ChiPai[i + 1] > 0))//两连取两边
		{
			if (((i - 1) % 10 != 0) && (ChiPai[i - 1] != 4))//取前
			{
				temp.push_back(i - 1);
				//temp.push_back(i - 1);
				temp.push_back(i);
				temp.push_back(i + 1);
				vecChi.push_back(temp);
				temp.clear();
			}
			if (((i + 2) % 10 != 0) && (ChiPai[i + 2] != 4))//取后
			{
				temp.push_back(i + 2);
				temp.push_back(i);
				temp.push_back(i + 1);
				//temp.push_back(i + 2);
				vecChi.push_back(temp);
				temp.clear();
			}
		}
		if ((ChiPai[i] > 0) && (ChiPai[i + 2] > 0) && ((i + 1) % 10 != 0) && (ChiPai[i + 1] != 4)) //取中
		{
			temp.push_back(i + 1);
			temp.push_back(i);
			//temp.push_back(i + 1);
			temp.push_back(i + 2);
			vecChi.push_back(temp);
			temp.clear();
		}
	}
}

bool empty()//判断数组是否删空,即每张牌数都为0
{
	int sum = 0;
	for (int i = 1; i < 44; ++i) sum += HuMahjong[i];
	return !sum;
}

void backtrack()
{
	if (empty())
	{
		ishu = true;
		HuType.push_back(HuTemp);
		//return;
	}
	else
	{
		int i = 1;
		while (!HuMahjong[i]) ++i;
		if (HuMahjong[i + 1] && HuMahjong[i + 2] && i < 29) //判断刻子
		{
			--HuMahjong[i];
			--HuMahjong[i + 1];
			--HuMahjong[i + 2];
			HuTemp.push_back(i);
			HuTemp.push_back(i + 1);
			HuTemp.push_back(i + 2);
			backtrack();
			++HuMahjong[i];
			++HuMahjong[i + 1];
			++HuMahjong[i + 2];
			HuTemp.pop_back();
			HuTemp.pop_back();
			HuTemp.pop_back();
		}
		if (HuMahjong[i] >= 3)
		{
			HuMahjong[i] -= 3;
			HuTemp.push_back(i);
			HuTemp.push_back(i);
			HuTemp.push_back(i);
			backtrack();
			HuMahjong[i] += 3;
			HuTemp.pop_back();
			HuTemp.pop_back();
			HuTemp.pop_back();
		}
	}
}

bool IsHu(int Pai)  //14张牌分组判断
{
	ishu = false;//置“胡”标志为否
	for (int i = 0; i < 44; i++)//判断有没有出现5对的情况
	{
		if (HuMahjong[i] == 5) return false;
	}
	for (int i = 0; i < 44; i++)
	{
		if (HuMahjong[i] >= 2)
		{
			HuMahjong[i] -= 2;//去将牌
			HuTemp.clear();
			HuTemp.push_back(Pai);
			HuTemp.push_back(i);
			HuTemp.push_back(i);
			backtrack();//入递归
			HuMahjong[i] += 2;//还原
		}
	}
	return false;
}

void Hu()
{
	HuType.clear();
	int AllPai[34] = { 1,2,3,4,5,6,7,8,9,11,12,13,14,15,16,17,18,19,21,22,23,24,25,26,27,28,29,31,32,33,34,41,42,43 };
	for (int i = 0; i < 34; i++)
	{
		++HuMahjong[AllPai[i]];
		IsHu(AllPai[i]);
		--HuMahjong[AllPai[i]];
	}
}


void HuTypeOut()
{
	OutHuType.clear();
	vector<int> Temp;
	vector<vector<int> > tempsort;
	vector<int> Temp21;
	for (int i = 0; i < HuType.size(); i++)
	{
		Temp.clear();
		tempsort.clear();
		//两个将牌加0
		Temp.push_back(HuType[i][1]);
		Temp.push_back(HuType[i][2]);
		Temp.push_back(0);
		tempsort.push_back(Temp);
		for (int j = 3; j < HuType[0].size(); j += 3)
		{
			Temp.clear();
			Temp.push_back(HuType[i][j]);
			Temp.push_back(HuType[i][j + 1]);
			Temp.push_back(HuType[i][j + 2]);
			tempsort.push_back(Temp);
			Temp.clear();
		}

		int p = 0;
		vector<vector<int> > tempsort1(tempsort.size(), vector<int>(3));
		while (p < tempsort.size())//判断刻子和顺子里可胡的牌,即每行
		{
			tempsort1 = tempsort;
			for (int it = 2; it >= 0; it--)
			{
				if (tempsort1[p][it] == HuType[i][0])//该行有可胡的牌
				{
					tempsort1[p][it] = 0;//置0
					swap(tempsort1[p][it], tempsort1[p][2]);
					if (p != 0)
					{
						sort(tempsort1[p].begin(), tempsort1[p].end() - 1);
					}
					sort(tempsort1.begin(), tempsort1.end());
					Temp21.push_back(HuType[i][0]);
					for (int k = 0; k < tempsort1.size(); k++)
					{
						Temp21.push_back(tempsort1[k][0]);
						Temp21.push_back(tempsort1[k][1]);
						Temp21.push_back(tempsort1[k][2]);
					}
					OutHuType.push_back(Temp21);
					tempsort1.clear();
					break;
				}
			}
			Temp21.clear();
			p++;
		}
		OutHuType.erase(unique(OutHuType.begin(), OutHuType.end()), OutHuType.end());//去重
		sort(OutHuType.begin(), OutHuType.end());
	}
}

int main()
{
	//下标为牌的数字表示
	vector<string> Mahjong = { " ","一条", "二条", "三条", "四条", "五条", "六条", "七条", "八条", "九条",
		" ","一万", "二万", "三万", "四万", "五万", "六万", "七万", "八万", "九万",
		" ","一筒", "二筒", "三筒", "四筒", "五筒", "六筒", "七筒", "八筒", "九筒",
		" ","东风", "西风", "南风", "北风",    " ",   " ",    " ",    " ",    " ",
		" ",  "红中", "白板", "发财" };
	fstream TestFile("./Test.log", ios::in);
	if (!TestFile.is_open()) cout << "文件打开成功";
	fstream LogFile("./TestData.txt", ios::out);
	fstream LogFileHu("./TestHuData.txt", ios::out);
	string line;
	int val;
	vector<int> CardArray;
	while (getline(TestFile, line))
	{
		CardArray.clear();
		istringstream inStream(line);
		while (inStream >> val)
		{
			CardArray.push_back(val);
		}
		int HandsPaiNums = 1;
		while (HandsPaiNums < 13)
		{
			HandsPaiNums += 3; //手牌数目4开始
			vector<int> InputPai;
			LogFile << "手牌为:";
			for (int i = 0; i < HandsPaiNums; i++)
			{
				InputPai.push_back(CardArray[i]);//取洗好牌的前HandsPaiNums张
				LogFile << Mahjong[CardArray[i]] << " "; //写入文件
			}
			LogFile << endl;

			memset(HuMahjong, 0, sizeof(HuMahjong));   //初始化置0
			for (int i = 0; i < HandsPaiNums; i++)  //对牌计数,下标为牌的大小,存放元素为张数
			{
				++HuMahjong[InputPai[i]];
			}

			Peng(HuMahjong);
			LogFile << "可以碰的牌:";
			for (int i = 0; i < vecPeng.size(); i++)
			{
				LogFile << Mahjong[vecPeng[i]] << " ";
			}
			LogFile << endl;

			Gang(HuMahjong);
			LogFile << "可以杠的牌:";
			for (int i = 0; i < vecGang.size(); i++)
			{
				LogFile << Mahjong[vecGang[i]] << " ";
			}
			LogFile << endl;

			Chi(HuMahjong);
			sort(vecChi.begin(), vecChi.end());
			LogFile << "可以吃的牌:" << endl;
			if (vecChi.size() > 0)
			{
				LogFile << Mahjong[vecChi[0][0]] << ":" << Mahjong[vecChi[0][1]] << " " << Mahjong[vecChi[0][2]];//输出吃的第一行
				for (int i = 1; i < vecChi.size(); i++)
				{
					if (vecChi[i][0] == vecChi[i - 1][0])
					{
						LogFile << "," << Mahjong[vecChi[i][1]] << " " << Mahjong[vecChi[i][2]];
					}
					else
					{
						LogFile << endl;
						LogFile << Mahjong[vecChi[i][0]] << ":" << Mahjong[vecChi[i][1]] << " " << Mahjong[vecChi[i][2]];
					}
				}
				LogFile << endl;
			}


			Hu();
			LogFile << "可以胡的牌:" << endl;
			if (HuType.size())
			{
				//OutHuType.clear();
				HuTypeOut();
				//胡牌的第一行
				LogFile << Mahjong[OutHuType[0][0]] << ":";
				for (int j = 1; j < OutHuType[0].size(); j++)
				{
					if ((j % 3 == 1) && (j > 1)) LogFile << ",";
					if (OutHuType[0][j] != 0)
					{
						LogFile << Mahjong[OutHuType[0][j]] << " ";
					}
				}
				//第2到最后一行
				for (int i = 1; i < OutHuType.size(); i++)
				{
					if (OutHuType[i][0] == OutHuType[i - 1][0])
					{
						LogFile << "  ";
						for (int j = 1; j < OutHuType[0].size(); j++)
						{
							if ((j % 3 == 1) && (j > 1)) LogFile << ",";
							if (OutHuType[i][j] != 0)
							{
								LogFile << Mahjong[OutHuType[i][j]] << " ";
							}
						}
					}
					else
					{
						LogFile << endl;
						LogFile << Mahjong[OutHuType[i][0]] << ":";
						for (int j = 1; j < OutHuType[0].size(); j++)
						{

							if ((j % 3 == 1) && (j > 1)) LogFile << ",";
							if (OutHuType[i][j] != 0)
							{
								LogFile << Mahjong[OutHuType[i][j]] << " ";
							}
						}
					}
				}
				LogFile << endl << endl;
			}

			LogFile << endl;

			//胡牌log
			if (OutHuType.size())
			{
				LogFileHu << "手牌为:";
				for (int i = 0; i < HandsPaiNums; i++)
				{
					LogFileHu << Mahjong[InputPai[i]] << " "; //写入文件
				}
				LogFileHu << endl;

				LogFileHu << "可以碰的牌:";
				for (int i = 0; i < vecPeng.size(); i++)
				{
					LogFileHu << Mahjong[vecPeng[i]] << " ";
				}
				LogFileHu << endl;

				LogFileHu << "可以杠的牌:";
				for (int i = 0; i < vecGang.size(); i++)
				{
					LogFileHu << Mahjong[vecGang[i]] << " ";
				}
				LogFileHu << endl;

				LogFileHu << "可以吃的牌:" << endl;
				if (vecChi.size() > 0)
				{
					LogFileHu << Mahjong[vecChi[0][0]] << ":" << Mahjong[vecChi[0][1]] << " " << Mahjong[vecChi[0][2]];//输出吃的第一行
					for (int i = 1; i < vecChi.size(); i++)
					{
						if (vecChi[i][0] == vecChi[i - 1][0])
						{
							LogFileHu << "," << Mahjong[vecChi[i][1]] << " " << Mahjong[vecChi[i][2]];
						}
						else
						{
							LogFileHu << endl;
							LogFileHu << Mahjong[vecChi[i][0]] << ":" << Mahjong[vecChi[i][1]] << " " << Mahjong[vecChi[i][2]];
						}
					}
					LogFileHu << endl;
				}



				LogFileHu << "可以胡的牌:" << endl;
				//胡牌的第一行
				LogFileHu << Mahjong[OutHuType[0][0]] << ":";
				for (int j = 1; j < OutHuType[0].size(); j++)
				{
					if ((j % 3 == 1) && (j > 1)) LogFileHu << ",";
					if (OutHuType[0][j] != 0)
					{
						LogFileHu << Mahjong[OutHuType[0][j]] << " ";
					}
				}
				//第2到最后一行
				for (int i = 1; i < OutHuType.size(); i++)
				{
					if (OutHuType[i][0] == OutHuType[i - 1][0])
					{
						LogFileHu << "  ";
						for (int j = 1; j < OutHuType[0].size(); j++)
						{
							if ((j % 3 == 1) && (j > 1)) LogFileHu << ",";
							if (OutHuType[i][j] != 0)
							{
								LogFileHu << Mahjong[OutHuType[i][j]] << " ";
							}
						}
					}
					else
					{
						LogFileHu << endl;
						LogFileHu << Mahjong[OutHuType[i][0]] << ":";
						for (int j = 1; j < OutHuType[0].size(); j++)
						{

							if ((j % 3 == 1) && (j > 1)) LogFileHu << ",";
							if (OutHuType[i][j] != 0)
							{
								LogFileHu << Mahjong[OutHuType[i][j]] << " ";
							}
						}
					}
				}
				LogFileHu << endl << endl;
			}
			OutHuType.clear();
		}
	}
	LogFile.close();
	LogFileHu.close();
	system("pause");
}