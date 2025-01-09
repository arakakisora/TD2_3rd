#pragma once

#include <Framework.h>

#include "Object3D.h"

// 定数としてマップサイズを定義
constexpr int WIDTH = 7;  // x軸方向のサイズ
constexpr int HEIGHT = 5; // y軸方向のサイズ
constexpr int DEPTH = 1;  // z軸方向のサイズ

class Field
{
public:
	
	// 初期化
	void Initialize(std::vector<Object3D*> object3D);
	
	// 終了
	void Finalize();
	
	// 更新
	void Update();
	
	// 描画
	void Draw();

	// ImGui
	void ImGui();

public: // セッター

	// ブロックの種類
	void SetBlockType(int x, int y, int z, uint32_t type) { pBlocks_[z][y][x].type = type; }

public: // ゲッター

    uint32_t GetBlockType(int x, int y, int z) const 
    {
        if (z >= 0 && z < DEPTH && y >= 0 && y < HEIGHT && x >= 0 && x < WIDTH) 
        {
            return pBlocks_[z][y][x].type;
        }
        return -1; // 無効なブロックを返す
    }

private: // 構造体	

	struct Block
	{
		Object3D object3D;
		uint32_t type;
	};

private:

	// ブロックの配列
	std::array<std::array<std::array<Block, WIDTH>, HEIGHT>, DEPTH> pBlocks_;

};

