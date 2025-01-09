#include "Player.h"

Player::Player()
{
}

Player::~Player()
{
}

void Player::Initialize()
{
	ModelManager::GetInstans()->LoadModel("Player.obj");


}

void Player::Update()
{
}

void Player::Draw()
{
}

void Player::Finalize()
{
}

void Player::MoveBeforeCheck()
{
}
