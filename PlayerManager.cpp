#include "PlayerManager.h"
#include <CameraManager.h>

void PlayerManager::Initialize(Ball* ball) {
    std::vector<int> initPosZ = { 0, 2, 4 };

    for (size_t i = 0; i < initPosZ.size(); ++i) {
        auto player = std::make_unique<Player>();
        if (i == 0) {
            player->Initialize(initPosZ[i], ball);
        }
        else {
            player->Initialize(initPosZ[i]);
        }
        players_.push_back(std::move(player));
    }
}

void PlayerManager::Finalize()
{
	for (auto& player : players_) {
		player->Finalize();
	}
}

void PlayerManager::Update(Field* field, EnemyManager* enemyManager) {
    for (auto& player : players_) {
        player->Update();
        field->SetPlayerPos(player->GetPosX(), player->GetPosY(), player->GetPosZ());
    }

    // マウスクリックでプレイヤーを選択
    Vector3 mousePos = Input::GetInstans()->GetMouseWorldPosition(
        CameraManager::GetInstans()->GetActiveCamera()->GetTransform().translate.y);

    if (Input::GetInstans()->TriggerMouse(0)) {
        for (auto& player : players_) {
            player->HandleMouseClick(mousePos, field, selectedPlayer_, enemyManager);
        }
    }
}

void PlayerManager::Draw() {
    for (const auto& player : players_) {
        player->Draw();
    }
}

void PlayerManager::ImGui() {
    for (const auto& player : players_) {
        player->ImGui();
    }
}

void PlayerManager::UpdateTransform()
{
	for (const auto& player : players_) {
		player->UpdateTransform();
	}
}

bool PlayerManager::HasAnyPlayerMovedOrPassed() {
    for (const auto& player : players_) {
        if (player->GetHasMoved() || player->IsPassing()) {
            return true;
        }
    }
    return false;
}

void PlayerManager::ResetAllMoveAndPassFlags() {
    for (auto& player : players_) {
        player->ResetMoveFlag();
        player->ResetPassFlag();
    }
}

