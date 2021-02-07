#pragma once
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include <iostream>
#include <fstream>
#include <sstream> 

enum Tag
{
	EnemyTag = 0,
};

struct DATA
{
	std::string Name;
	DirectX::XMFLOAT3 FirstPos;
	DirectX::XMFLOAT3 SecondPos;
	DirectX::XMFLOAT3 SpawnSpot;
	double HP;
	double SPEED;
	Tag Tag;
	double Wave1;
	double Wave2;
	double Wave3;
	double Wave4;
	double Wave2Attack;
	double Wave2Hp;
	double Wave2Speed;
	double Wave3Attack;
	double Wave3Hp;
	double Wave3Speed;
	double Wave4Attack;
	double Wave4Hp;
	double Wave4Speed;
	double BossAttack;
	double BossHp;
	double BossSpeed;
};

using namespace std;

vector<DATA> LoadData(const char* filePath);

vector<DATA>LoadData(const char*filePath)
{
	vector<DATA>dateTable;

	ifstream fileStream(filePath);
	if (!fileStream)
	{
		cout << "ファイルオープンに失敗しました" << endl;
		return dateTable;
	}
	//現在の行数
	int i = 0;

	while (!fileStream.eof())
	{
		i++;
		string bufferString;
		fileStream >> bufferString;
		//1行目は列名でデータではないためスキップ
		if (i <= 1) continue;
		if (i >= 3)break;
		//データを項目ごとに抽出
		istringstream stringStream(bufferString);
		vector<string> datas;
		string tmp;
		//カンマ区切りでデータを切り分け
		while (getline(stringStream, tmp, ','))
		{
			datas.push_back(tmp);
		}
		//切り分けたデータを構造体に格納
		DATA data;
		data.Name = datas[0];
		data.FirstPos.x = strtof(datas[1].c_str(), NULL);
		data.FirstPos.y = 0.0f;
		data.FirstPos.z = strtof(datas[2].c_str(), NULL);
		data.SecondPos.x = strtof(datas[3].c_str(), NULL);
		data.SecondPos.y = strtof(datas[4].c_str(), NULL);
		data.SecondPos.z = strtof(datas[5].c_str(), NULL);
		data.HP = strtod(datas[6].c_str(), NULL);
		data.SPEED = strtod(datas[7].c_str(), NULL);
		auto tag = datas[8];
		data.SpawnSpot.x = strtof(datas[9].c_str(), NULL);
		data.SpawnSpot.z = strtof(datas[10].c_str(), NULL);
		data.SpawnSpot.y = 0.0f;
		data.Wave1 = strtof(datas[11].c_str(), NULL);
		data.Wave2 = strtof(datas[12].c_str(), NULL);
		data.Wave3 = strtof(datas[13].c_str(), NULL);
		data.Wave2Attack = strtof(datas[14].c_str(), NULL);
		data.Wave2Hp = strtof(datas[15].c_str(), NULL);
		data.Wave2Speed = strtof(datas[16].c_str(), NULL);
		data.Wave3Attack = strtof(datas[17].c_str(), NULL);
		data.Wave3Hp = strtof(datas[18].c_str(), NULL);
		data.Wave3Speed = strtof(datas[19].c_str(), NULL);
		data.Wave4 = strtof(datas[20].c_str(), NULL);
		data.Wave4Attack = strtof(datas[21].c_str(), NULL);
		data.Wave4Hp = strtof(datas[22].c_str(), NULL);
		data.Wave4Speed = strtof(datas[23].c_str(), NULL);
		data.BossAttack = strtof(datas[24].c_str(), NULL);
		data.BossHp = strtof(datas[25].c_str(), NULL);
		data.BossSpeed = strtof(datas[26].c_str(), NULL);
		if (tag == "Enemy")
		{
			data.Tag = EnemyTag;
		}
		dateTable.push_back(data);
	}
	fileStream.close();
	return dateTable;
};