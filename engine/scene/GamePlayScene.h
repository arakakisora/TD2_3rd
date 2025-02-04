#pragma once

#include "Camera.h"	
#include "Model.h"
#include "Sprite.h"
#include "Object3D.h"
#include "Audio.h"
#include "BaseScene.h"
#include "Player.h"
#include "SceneManager.h"
#include "vector"
#include "Field.h"
#include "application/characters/enemy/Enemy.h"
#include "application/characters/enemy/EnemyManager.h"
#include "Ball.h"
#include "PlayerManager.h" // 追加

class GamePlayScene : public BaseScene
{
public:
    /// <summary>
    /// シーンの初期化
    /// </summary>
    void Initialize() override;
    /// <summary>
    /// シーンの終了処理
    /// </summary>
    void Finalize() override;
    /// <summary>
    /// シーンの更新
    /// </summary>
    void Update() override;
    /// <summary>
    /// シーンの描画
    /// </summary>
    void Draw() override;

    // 切り替え演出
    void Fade();

private:
    // ターンの状態
    enum class TurnState
    {
        NONE,
        PLAYER,
        ENEMY,
    };
    TurnState turnState_ = TurnState::PLAYER;

    // **プレイヤーマネージャー**
    PlayerManager playerManager_; // 追加

    // カメラのポインタ
    Camera* pCamera_ = nullptr;
    Vector3 cameraPos_ = Vector3(3.0f, -14.285f, 8.334f);
    Vector3 cameraRot_ = Vector3(-2.015f, 0.0f, 0.0f);

    // 3Dオブジェクト
    std::vector<Object3D*> pFieldObject_ = {};

    // フィールド
    std::unique_ptr<Field> pField_ = nullptr;

    // エネミーマネージャー
    std::unique_ptr<EnemyManager> enemyManager_ = nullptr;

    // テスト用
    std::vector<Vector3> prePos_ = {};

    // プレイヤーの選択状態
    Player* selectedPlayer_ = nullptr; // (後で `PlayerManager` に統合)

    // ボール
    Ball* ball = nullptr;

    // マウスの位置
    Vector3 mousePos = Vector3(0, 0, 0);
    Object3D* MouseObject = nullptr;

    // スプライト
    Sprite* whiteSprite_ = nullptr;
    Sprite* blackSprite_ = nullptr;

    bool isSceneStart_ = true;
    bool isClearFadeStart_ = false;
    bool isGameOverFadeStart_ = false;

    // 透明度
    float blackAlpha_ = 0.0f;
    float whiteAlpha_ = 0.0f;
};
