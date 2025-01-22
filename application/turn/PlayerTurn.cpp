#include "PlayerTurn.h"

#include "GamePlayScene.h"
#include "EnemyTurn.h"


PlayerTurn::PlayerTurn(GamePlayScene* _pScene) : BaseTurnState("Player Turn", _pScene)
{
	pPlayScene_->GetPlayer()->SetTurnEnd(false);
}

void PlayerTurn::Update()
{
	// プレイヤーの更新
	pPlayScene_->GetPlayer()->Update();

	// エネミーのトランスフォームの更新 （ここに入れないと敵のターンが来るまで描画が変になってまう）
	pPlayScene_->GetEnemy()->UpdateTransform();

	// プレイヤーの移動が終わったらエネミーターンに移行
	if (pPlayScene_->GetPlayer()->IsTurnEnd())
	{
		pPlayScene_->ChangeState(std::make_unique<EnemyTurn>(pPlayScene_));
	}
}
