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
				pBlocks_[z][y][x].object3D.SetTranslate(Vector3(float(x), float(y), float(z)));
				pBlocks_[z][y][x].object3D.SetScale(Vector3(0.3f, 0.3f, 0.3f));
				pBlocks_[z][y][x].type = 0;
			
				// ゴールの位置Typeを1にしてる(仮置き)
				if (x == 6)
				{
					pBlocks_[z][y][x].type = 1;
				}

				// プレイヤーの位置Typeを2にしてる(仮置き)
				pBlocks_[0][0][1].type = 2;
				
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
				if (pBlocks_[z][y][x].type == 1)
				{
					pBlocks_[z][y][x].object3D.SetScale(Vector3(0.8f, 0.8f, 0.8f));
				}

				// ゴール判定  右端のtypeが1以外ならゴール
				if (pBlocks_[z][y][6].type != 1)
				{
					isGoal_ = true;
				}


				// -----------------テスト-----------------
				if (pBlocks_[z][y][x].type == 2)
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
	ImGui::Text("type 2 : %.0f %.0f %.0f", GetBlockPosition(2).x, GetBlockPosition(2).y, GetBlockPosition(2).z);

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
