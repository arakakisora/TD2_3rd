#include "PlayerManager.h"


void PlayerManager::Initialize(Field* field, Ball* ball) {
    std::vector<int> initPosZ = { 0, 2, 4 };
    for (int i = 0; i < initPosZ.size(); ++i) {
        auto player = std::make_unique<Player>();
        if (i == 0) {
            player->Initialize(initPosZ[i], ball);  // 最初のプレイヤーにボールを持たせる
        }
        else {
            player->Initialize(initPosZ[i]);
        }
        players_.push_back(std::move(player));
    }

    // プレイヤーの位置をフィールドにセット
    for (const auto& player : players_) {
        field->SetPlayerPos(player->GetPosX(), player->GetPosY(), player->GetPosZ());
    }
}

void PlayerManager::Update() {
    for (auto& player : players_) {
        player->Update();
    }
}

void PlayerManager::Draw() {
    for (const auto& player : players_) {
        player->Draw();
    }
}

void PlayerManager::Finalize() {
    for (auto& player : players_) {
        player->Finalize();
    }
}

void PlayerManager::UpdateTransform()
{
    for (auto& player : players_)
    {
        player->UpdateTransform();
    }
}
void PlayerManager::ImGuiDebug()
{
    for (const auto& player : players_)
    {
        player->ImGui();
    }
}

bool PlayerManager::IsAllPlayersDone() const {
    for (const auto& player : players_) {
        if (!player->GetHasMoved() && !player->IsPassing()) {
            return false;
        }
    }
    return true;
}

void PlayerManager::ResetTurn() {
    for (auto& player : players_) {
        player->ResetMoveFlag();
        player->ResetPassFlag();
    }
}

void PlayerManager::HandleMouseClick(const Vector3& mousePos, Field* field, EnemyManager* enemyManager) {
    for (auto& player : players_) {
        player->HandleMouseClick(mousePos, field, selectedPlayer_, enemyManager);
    }
}
