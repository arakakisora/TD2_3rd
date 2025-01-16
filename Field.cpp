#include "Field.h"

void Field::Initialize(std::vector<Object3D*> object3D)
{
	// ブロックの初期化
	for (int z = 0; z < DEPTH; z++)
	{
		for (int y = 0; y < HEIGHT; y++)
		{
			for (int x = 0; x < WIDTH; x++)
			{
				pBlocks_[z][y][x].object3D = *object3D[x + y * WIDTH + z * WIDTH * HEIGHT];
				pBlocks_[z][y][x].object3D.SetTranslate(Vector3(float(x) - 3, float(y) - 2, float(z)));
				pBlocks_[z][y][x].object3D.SetScale(Vector3(0.3f, 0.3f, 0.3f));
				pBlocks_[z][y][x].type = 0;
			
				// 攻めゴールの位置Typeを2にしてる
				if (x == 6)
				{
					pBlocks_[z][y][x].type = 2;
				}
				// 守りゴールの位置Typeを3にしてる
				if (x == 0)
				{
					pBlocks_[z][y][x].type = 3;
				}

				// ボールの位置Typeを1にしてる
				pBlocks_[0][0][1].type = 1;
				
			}
		}
	}
}

void Field::Finalize()
{
}

void Field::Update()
{
	for (int z = 0; z < DEPTH; z++)
	{
		for (int y = 0; y < HEIGHT; y++)
		{
			for (int x = 0; x < WIDTH; x++)
			{
				pBlocks_[z][y][x].object3D.Update();

				// ゴール位置(とりあえずでっかくしてる)
				if (pBlocks_[z][y][x].type == 2)
				{
					pBlocks_[z][y][x].object3D.SetScale(Vector3(0.8f, 0.8f, 0.8f));
				}

				// ゴール判定  決めゴール位置にボール持ちがいたらゴール
				if (pBlocks_[z][y][6].type == 5)
				{
					isGoal_ = true;
				}

				ballPos_ = GetBlockPosition(1);
				// ボールの位置を整数に変換
				ballPos_.x = (int)ballPos_.x;
				ballPos_.y = (int)ballPos_.y;
				ballPos_.z = (int)ballPos_.z;
				// 自機の位置にボールが来たらボール持ちに変更
				if()

				// プレイヤーのボールが取られたらゲームオーバー


				// -----------------テスト-----------------
				if (pBlocks_[z][y][x].type == 1)
				{
					pBlocks_[z][y][x].object3D.SetScale(Vector3(0.15f, 0.15f, 0.15f));
				}
				// 大きさ戻す
				if (pBlocks_[z][y][x].type == 0)
				{
					pBlocks_[z][y][x].object3D.SetScale(Vector3(0.3f, 0.3f, 0.3f));
				}

				
			}
		}
	}
}

void Field::Draw()
{
	// ブロックの描画
	for (int z = 0; z < DEPTH; z++)
	{
		for (int y = 0; y < HEIGHT; y++)
		{
			for (int x = 0; x < WIDTH; x++)
			{
				pBlocks_[z][y][x].object3D.Draw();
			}
		}
	}
}

void Field::ImGui()
{
	ImGui::Text("type 1 : %.0f %.0f %.0f", GetBlockPosition(1).x, GetBlockPosition(1).y, GetBlockPosition(1).z);

	for (int z = 0; z < DEPTH; z++)
	{
		for (int y = 0; y < HEIGHT; y++)
		{
			for (int x = 0; x < WIDTH; x++)
			{
				ImGui::Text("Block Position %.2f %.2f %.2f", 
					pBlocks_[z][y][x].object3D.GetTransform().translate.x,
					pBlocks_[z][y][x].object3D.GetTransform().translate.y,
					pBlocks_[z][y][x].object3D.GetTransform().translate.z);
			}
		}
	}
}
