#pragma once
#include <vector>
#include <memory>
#include "Player.h"
#include "Field.h"
#include "application/characters/enemy/EnemyManager.h"

class PlayerManager {
public:
    void Initialize(Field* field, Ball* ball);
    void Update();
    void Draw();
    void Finalize();

    void UpdateTransform();
    

    void ImGuiDebug();
    



    // プレイヤーのターン終了判定
    bool IsAllPlayersDone() const;
    void ResetTurn();

    // クリック処理
    void HandleMouseClick(const Vector3& mousePos, Field* field, EnemyManager* enemyManager);

	//getter
    const std::vector<std::unique_ptr<Player>>& GetPlayers() const { return players_; }




private:
    std::vector<std::unique_ptr<Player>> players_;
    Player* selectedPlayer_ = nullptr;
};

