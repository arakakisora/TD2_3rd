#pragma once
#include <vector>
#include <memory>
#include "Player.h"
#include "Ball.h"

class PlayerManager {
public:
    // 初期化
    void Initialize(Ball* ball);

    //終了
	void Finalize();

    // 更新
    void Update(Field* field, EnemyManager* enemyManager);

    // 描画
    void Draw();

    // 選択されたプレイヤーを取得
    Player* GetSelectedPlayer() { return selectedPlayer_; }

    // プレイヤーリストを取得
    const std::vector<std::unique_ptr<Player>>& GetPlayers() const { return players_; }

    void ImGui();

	void UpdateTransform();

    bool HasAnyPlayerMovedOrPassed();
    void ResetAllMoveAndPassFlags();

    Ball* GetBall() { return ball_; }

	// ゴールしたか判定
	bool IsGoal() const { return isGoal_; }

private:
    std::vector<std::unique_ptr<Player>> players_;
    Player* selectedPlayer_ = nullptr;
	Ball* ball_ = nullptr;
    bool isGoal_ = false;
};
