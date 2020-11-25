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

private:
	Enemy* enemybox[999];
	int boxcount = 0;
};
