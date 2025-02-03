#include "PlayerManeger.h"



PlayerManeger::PlayerManeger()
{
}

PlayerManeger::~PlayerManeger()
{
}

void PlayerManeger::Initialize(Ball* ball)
{
	ball_ = ball;
	std::vector<int> initPosZ = { 0, 2, 4 };
	for (int i = 0; i < initPosZ.size(); ++i) {
		auto player = std::make_unique<Player>();
		if (i == 0)
		{
			//１番目にボールを持たせる
			player->Initialize(initPosZ[i], ball_);
		}
		else
		{
			player->Initialize(initPosZ[i]);
		}
		pPlayer_.push_back(std::move(player));
	}
}

void PlayerManeger::Finalize()
{
	for (auto& player : pPlayer_)
	{
		player->Finalize();
	}
	ball_->Finalize();
}

void PlayerManeger::Update()
{
	for (const auto& player : pPlayer_) {
		player->Update();
	}
	ball_->Update();
}

void PlayerManeger::Draw()
{
	for (const auto& player : pPlayer_) {
		player->Draw();
	}
	ball_->Draw();
}
