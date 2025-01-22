#include "EnemyTurn.h"

#include "GamePlayScene.h"
#include "PlayerTurn.h"

EnemyTurn::EnemyTurn(GamePlayScene* _pScene) : BaseTurnState("Enemy Turn",_pScene)
{
	pPlayScene_->GetEnemy()->SetTurnEnd(false);
}

void EnemyTurn::Update()
{
	// エネミーの更新
	pPlayScene_->GetEnemy()->Update();
	// エネミーの移動が終わったらプレイヤーターンに移行
	if (pPlayScene_->GetEnemy()->IsTurnEnd())
	{
		pPlayScene_->ChangeState(std::make_unique<PlayerTurn>(pPlayScene_));
	}
}
