// mahjong.cpp : �������̨Ӧ�ó������ڵ㡣
//�򵥵��齫�㷨,�ҳ��ɳԡ������ܺͺ�����
//�齫�ı�ʾ:1-9:��
//            11-19:Ͳ
//            21-29:��
//            31-34:��������
//            41-43:�а׷�

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

bool ishu;//�к���־
int HuMahjong[44];//�����洢����,��ÿ���Ƶ�����,�����Ϊ44,���鳤��Ϊ44
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
		if ((ChiPai[i] > 0) && (ChiPai[i + 1] > 0))//����ȡ����
		{
			if (((i - 1) % 10 != 0) && (ChiPai[i - 1] != 4))//ȡǰ
			{
				temp.push_back(i - 1);
				//temp.push_back(i - 1);
				temp.push_back(i);
				temp.push_back(i + 1);
				vecChi.push_back(temp);
				temp.clear();
			}
			if (((i + 2) % 10 != 0) && (ChiPai[i + 2] != 4))//ȡ��
			{
				temp.push_back(i + 2);
				temp.push_back(i);
				temp.push_back(i + 1);
				//temp.push_back(i + 2);
				vecChi.push_back(temp);
				temp.clear();
			}
		}
		if ((ChiPai[i] > 0) && (ChiPai[i + 2] > 0) && ((i + 1) % 10 != 0) && (ChiPai[i + 1] != 4)) //ȡ��
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

bool empty()//�ж������Ƿ�ɾ��,��ÿ��������Ϊ0
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
		if (HuMahjong[i + 1] && HuMahjong[i + 2] && i < 29) //�жϿ���
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

bool IsHu(int Pai)  //14���Ʒ����ж�
{
	ishu = false;//�á�������־Ϊ��
	for (int i = 0; i < 44; i++)//�ж���û�г���5�Ե����
	{
		if (HuMahjong[i] == 5) return false;
	}
	for (int i = 0; i < 44; i++)
	{
		if (HuMahjong[i] >= 2)
		{
			HuMahjong[i] -= 2;//ȥ����
			HuTemp.clear();
			HuTemp.push_back(Pai);
			HuTemp.push_back(i);
			HuTemp.push_back(i);
			backtrack();//��ݹ�
			HuMahjong[i] += 2;//��ԭ
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
		//�������Ƽ�0
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
		while (p < tempsort.size())//�жϿ��Ӻ�˳����ɺ�����,��ÿ��
		{
			tempsort1 = tempsort;
			for (int it = 2; it >= 0; it--)
			{
				if (tempsort1[p][it] == HuType[i][0])//�����пɺ�����
				{
					tempsort1[p][it] = 0;//��0
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
		OutHuType.erase(unique(OutHuType.begin(), OutHuType.end()), OutHuType.end());//ȥ��
		sort(OutHuType.begin(), OutHuType.end());
	}
}

int main()
{
	//�±�Ϊ�Ƶ����ֱ�ʾ
	vector<string> Mahjong = { " ","һ��", "����", "����", "����", "����", "����", "����", "����", "����",
		" ","һ��", "����", "����", "����", "����", "����", "����", "����", "����",
		" ","һͲ", "��Ͳ", "��Ͳ", "��Ͳ", "��Ͳ", "��Ͳ", "��Ͳ", "��Ͳ", "��Ͳ",
		" ","����", "����", "�Ϸ�", "����",    " ",   " ",    " ",    " ",    " ",
		" ",  "����", "�װ�", "����" };
	fstream TestFile("./Test.log", ios::in);
	if (!TestFile.is_open()) cout << "�ļ��򿪳ɹ�";
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
			HandsPaiNums += 3; //������Ŀ4��ʼ
			vector<int> InputPai;
			LogFile << "����Ϊ:";
			for (int i = 0; i < HandsPaiNums; i++)
			{
				InputPai.push_back(CardArray[i]);//ȡϴ���Ƶ�ǰHandsPaiNums��
				LogFile << Mahjong[CardArray[i]] << " "; //д���ļ�
			}
			LogFile << endl;

			memset(HuMahjong, 0, sizeof(HuMahjong));   //��ʼ����0
			for (int i = 0; i < HandsPaiNums; i++)  //���Ƽ���,�±�Ϊ�ƵĴ�С,���Ԫ��Ϊ����
			{
				++HuMahjong[InputPai[i]];
			}

			Peng(HuMahjong);
			LogFile << "����������:";
			for (int i = 0; i < vecPeng.size(); i++)
			{
				LogFile << Mahjong[vecPeng[i]] << " ";
			}
			LogFile << endl;

			Gang(HuMahjong);
			LogFile << "���Ըܵ���:";
			for (int i = 0; i < vecGang.size(); i++)
			{
				LogFile << Mahjong[vecGang[i]] << " ";
			}
			LogFile << endl;

			Chi(HuMahjong);
			sort(vecChi.begin(), vecChi.end());
			LogFile << "���ԳԵ���:" << endl;
			if (vecChi.size() > 0)
			{
				LogFile << Mahjong[vecChi[0][0]] << ":" << Mahjong[vecChi[0][1]] << " " << Mahjong[vecChi[0][2]];//����Եĵ�һ��
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
			LogFile << "���Ժ�����:" << endl;
			if (HuType.size())
			{
				//OutHuType.clear();
				HuTypeOut();
				//���Ƶĵ�һ��
				LogFile << Mahjong[OutHuType[0][0]] << ":";
				for (int j = 1; j < OutHuType[0].size(); j++)
				{
					if ((j % 3 == 1) && (j > 1)) LogFile << ",";
					if (OutHuType[0][j] != 0)
					{
						LogFile << Mahjong[OutHuType[0][j]] << " ";
					}
				}
				//��2�����һ��
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

			//����log
			if (OutHuType.size())
			{
				LogFileHu << "����Ϊ:";
				for (int i = 0; i < HandsPaiNums; i++)
				{
					LogFileHu << Mahjong[InputPai[i]] << " "; //д���ļ�
				}
				LogFileHu << endl;

				LogFileHu << "����������:";
				for (int i = 0; i < vecPeng.size(); i++)
				{
					LogFileHu << Mahjong[vecPeng[i]] << " ";
				}
				LogFileHu << endl;

				LogFileHu << "���Ըܵ���:";
				for (int i = 0; i < vecGang.size(); i++)
				{
					LogFileHu << Mahjong[vecGang[i]] << " ";
				}
				LogFileHu << endl;

				LogFileHu << "���ԳԵ���:" << endl;
				if (vecChi.size() > 0)
				{
					LogFileHu << Mahjong[vecChi[0][0]] << ":" << Mahjong[vecChi[0][1]] << " " << Mahjong[vecChi[0][2]];//����Եĵ�һ��
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



				LogFileHu << "���Ժ�����:" << endl;
				//���Ƶĵ�һ��
				LogFileHu << Mahjong[OutHuType[0][0]] << ":";
				for (int j = 1; j < OutHuType[0].size(); j++)
				{
					if ((j % 3 == 1) && (j > 1)) LogFileHu << ",";
					if (OutHuType[0][j] != 0)
					{
						LogFileHu << Mahjong[OutHuType[0][j]] << " ";
					}
				}
				//��2�����һ��
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