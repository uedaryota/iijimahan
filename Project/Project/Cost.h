#pragma once
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>
#include "Texture.h"
#include "Sprite.h"

class Cost
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// コストの取得
	/// </summary>
	int GetCost();

	/// <summary>
	/// コストの増加
	/// </summary>
	/// <param name="cost">コスト</param>
	void SetAddCost(int cost) { this->cost += cost; }

	/// <summary>
	/// コストの減少
	/// </summary>
	/// <param name ="cost">コスト</param>
	void SetSubCost(int cost) { this->cost -= cost; }

	/// <summary>
	/// コスト増加間隔のの設定
	/// </summary>
	/// <param name="costUpTime">コストの増加間隔</param>
	void SetCostUpTime(int costUpTime) { this->costUpTime = costUpTime; }

private:
	int maxcost = 60;//最大コスト
	Sprite* sprite = new Sprite();

public:
	int costUpTime = 30;//コスト増加間隔
	int time = 0;//時間測定用
	int cost = 0;//ガジェット設置用コスト
};

