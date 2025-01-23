#include "EnemyManager.h"

void EnemyManager::Initialize(Object3DCommon* objectCommon, const std::string& filename, int enemyCount)
{
	//敵の数分確保
	enemies_.resize(enemyCount);
	//敵の初期化
	for (auto& enemy : enemies_)
	{
		enemy = std::make_unique<Enemy>();
		enemy->Initialize(objectCommon, filename);
	}
}
