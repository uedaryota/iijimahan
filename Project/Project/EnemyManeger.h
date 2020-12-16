#include"Enemy.h"
#include"EnemyAI.h"
#include"Poly.h"

class EnemyManeger
{
public:
	void Initialize();//初期化
	void Add(Enemy* enemy);//マネージャーに登録
	void Create();//マネージャーで作成
	void Destory();//削除
	void Draw();//描画
	void Update();//更新処理

    ///<summary>
	///第1第2ポイントまとめての移動目標の全体設定
	///<param name="anc1">第1移動地点</param>
	///<param name="anc2">第2移動地点</param>
	///</summary>
	void AncerSet(XMFLOAT3 anc1,XMFLOAT3 anc2);

	///<summary>
	///第1ポイントの全体設定
	///<param name="anc1">第1移動地点</param>
	///</summary>
	void FirstAncerSet(XMFLOAT3 anc1);

	///<summary>
	///第2ポイントの全体設定
	///<param name="anc2">第2移動地点</param>
	///</summary>
	void SecondAncerSet(XMFLOAT3 anc2);

private:
	Enemy* enemybox[999];
	int boxcount = 0;
};
