#pragma once
#include "Model.h"
#include "ModelManager.h"
#include "Input.h"
#include "Object3D.h"
#include "Object3DCommon.h"
#include "Field.h"


class PlayerManager;
class EnemyManager;
class Ball;
class Player
{
public:
	// コンストラクタ
	Player();
	// デストラクタ
	~Player();
	// 初期化
	void Initialize(int posZ, Ball* ball = nullptr);
	// 更新
	void Update(PlayerManager* playerManager);
	// 描画
	void Draw();
	// 終了
	void Finalize();
	// 移動
	void Move(int WIDTH, int DEPTH, PlayerManager* playerManager);
	//3Dオブジェクトの更新
	void UpdateTransform();


	// ImGui 描画
	void ImGui();
	// ドリブル
	void Dribble(PlayerManager* playerManager);
	// パス
	void Pass();
  
	bool HasPerformedAction() const { return isMoved || ispsMoved; }
	void ResetActionFlags() { isMoved = false; ispsMoved = false; }

  

	void HandleMouseClick(const Vector3& mousePos, Field* field, Player*& selectedPlayer,EnemyManager* enemyManager, PlayerManager* playerManager);
	
	bool CanMoveTo(int x, int z);
	bool IsValidPassPosition(const Vector3& mousePos, Field* field);
	bool CheckObjectClick(Object3D* object, const Vector3& mousePos);

	
	

	
	void PlayerPass(const Vector3& mousePos, Field* field, Player*& selectedPlayer, PlayerManager* playerManager);
	void playerDribble(const Vector3& mousePos, Field* field, Player*& selectedPlayer, EnemyManager* enemyManager);
	//void playerPass(Player*& selectedPlayer);


public: //アクセッサ
	Vector3 GetPosition() { return playerData.position; }
	void SetPosition(Vector3 pos) { playerData.position = pos; }
	Vector3 GetRotate() { return playerData.rotate; }
	void SetRotate(Vector3 rot) { playerData.rotate = rot; }
	Vector3 GetScale() { return playerData.scale; }
	void SetScale(Vector3 scale) { playerData.scale = scale; }
	

	// ボール所持
	bool HasBall() { return ball; }
	void SetBall(Ball* ball);



public: // ゲッター  追加

	bool GetHasMoved() { return isMoved; }
	void ResetMoveFlag() { isMoved = false; }

	bool IsPassing() const { return  ispsMoved; }
	void ResetPassFlag() { ispsMoved = false; }

	


	// プレイヤーの位置(マス) 追加
	int GetPosX() { return posX; }
	int GetPosY() { return posY; }
	int GetPosZ() { return posZ; }

	// プレイヤーの前の位置(マス)  追加
	int GetPrePosX() { return prePosX; }
	int GetPrePosY() { return prePosY; }
	int GetPrePosZ() { return prePosZ; }


	//セッター

	//プレイヤーの座標セッター
	void SetPlayerPos(int x, int z) {
		posX = static_cast<int>(x);
		posZ = static_cast<int>(z);
	}


private:

	Model* model;
	Object3D* object3D_;
	Object3D* passObject3D_;
	Object3D* dribbleObject3D_;
	bool isMoved = false; // 1ターン内での移動を制限
	bool ispsMoved = false; // 1ターン内での移動を制限

	bool isPassing = false;    // パスモードかどうか
	bool isDribbling = false;  // ドリブルモードかどうか
	bool isPassDribbleVisible = false;  // パス・ドリブルオブジェクトの可視フラグ



	struct PlayerData
	{
		Vector3 position;
		Vector3 rotate;
		Vector3 scale;
	};

	Ball* ball = nullptr;
	bool hasBall = false;
	PlayerData playerData;
	
	int posX = 0;
	int posY = 0;
	int posZ;

	// プレイヤーの前の位置　追加
	int prePosX = 0;
	int prePosY = 0;
	int prePosZ = 0;
};

