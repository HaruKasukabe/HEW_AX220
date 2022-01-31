//=============================================================================
//
// �}�b�v [map.cpp]
// ����T�q
//=============================================================================
#include "map.h"
#include "box.h"
#include "collision.h"
#include "DWBox.h"
#include "HalfBox.h"
#include "WoodBox.h"
#include "Monument.h"
#include "playerBoy.h"
#include "mapData.h"

OBJECT_INFO g_nowMap[MAP_HEIGHT][MAP_WIDTH] =
{
	//{{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},},
	//{{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},},
	//{{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{3,0,1},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},},
	//{{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},},
	//{{0,0},{0,0},{0,0},{3,0,1},{0,0},{0,0},{0,0},{1,0},{1,0},{1,0},{1,0},{1,0},{1,0},{1,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{1,0},{1,0},{1,0},{1,0},{1,0},{0,0},{2,0},{0,0},{2,0},{0,0},{0,0},{0,0},{0,0},{2,0},{5,0},},
};

OBJECT_INFO g_oldMap[MAP_HEIGHT][MAP_WIDTH] =
{
	//{{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},},
	//{{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},},
	//{{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{3,0,1},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},},
	//{{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},},
	//{{0,0},{0,0},{0,0},{3,0,1},{0,0},{0,0},{0,0},{1,0},{1,0},{1,0},{1,0},{1,0},{1,0},{1,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{1,0},{1,0},{1,0},{1,0},{1,0},{0,0},{2,0},{0,0},{2,0},{0,0},{0,0},{3,0,1},{0,0},{2,0},{5,0},},
};

//*****�O���[�o���ϐ�*****
static Box* g_pBox;
static DWBox* g_pDWBox;
static HalfBox* g_pHalfBox;
static WoodBox* g_pWoodBox;
static Player_Boy* g_pPlayerBoy;
static Monument* g_pMonument;
bool g_bBoxGravity;

//=============================
//		������
//=============================
HRESULT InitMap() {
	// �Z���N�g�����X�e�[�W���ƂɃ}�b�v��ݒ�
	for (int i = 0; i < MAP_HEIGHT; ++i) {
		for (int j = 0; j < MAP_WIDTH; ++j) {
			g_nowMap[i][j] = GetNowMapData(i, j);
			g_oldMap[i][j] = GetOldMapData(i, j);
		}
	}

	g_pBox = new Box;
	g_pDWBox = new DWBox;
	g_pHalfBox = new HalfBox;
	g_pWoodBox = new WoodBox;
	//g_pPlayerBoy = new Player_Boy;
	g_pMonument = new Monument;
	for (int i = 0; i < MAP_HEIGHT; ++i) {
		for (int j = 0; j < MAP_WIDTH; ++j) {
			switch (g_oldMap[i][j].m_nCategory) {
			case 0:
				g_oldMap[i][j].m_bOnBox = false;
				break;
			case NORMAL:
				g_oldMap[i][j].m_nObject = g_pDWBox->Create(XMFLOAT3(-90.0f+ j * 10.0f, -10 - i * 10.0f,0.0f), g_oldMap[i][j].m_nCategory, 1);
				g_oldMap[i][j].m_bOnBox = false;
				break;
			case BREAK:
				g_oldMap[i][j].m_nObject = g_pWoodBox->Create(XMFLOAT3(-90.0f + j * 10.0f, -10 - i * 10.0f, 0.0f), g_oldMap[i][j].m_nCategory, 1);
				g_oldMap[i][j].m_bOnBox = false;
				break;
			case CARRY:
				g_oldMap[i][j].m_nObject = g_pBox->Create(XMFLOAT3(-90.0f + j * 10.0f, -10 - i * 10.0f, 0.0f), g_oldMap[i][j].m_nCategory, 1);
				g_oldMap[i][j].m_bOnBox = false;
				break;
			case GOAL:
				g_oldMap[i][j].m_nObject = g_pMonument->Create(XMFLOAT3(-90.0f + j * 10.0f, -10 - i * 10.0f, 0.0f),0);
				g_oldMap[i][j].m_bOnBox = false;
				break;
			}
			switch (g_nowMap[i][j].m_nCategory) {
			case 0:
				g_nowMap[i][j].m_bOnBox = false;
				break;
			case NORMAL:
				g_nowMap[i][j].m_nObject = g_pDWBox->Create(XMFLOAT3(-90.0f + j * 10.0f, -10 - i * 10.0f, 0.0f), g_nowMap[i][j].m_nCategory, 0);
				g_nowMap[i][j].m_bOnBox = false;
				break;
			case BREAK:
				g_nowMap[i][j].m_nObject = g_pWoodBox->Create(XMFLOAT3(-90.0f + j * 10.0f, -10 - i * 10.0f, 0.0f), g_nowMap[i][j].m_nCategory, 0);
				g_nowMap[i][j].m_bOnBox = false;
			case CARRY:
				g_nowMap[i][j].m_nObject = g_pBox->Create(XMFLOAT3(-90.0f + j * 10.0f, -10 - i * 10.0f, 0.0f), g_nowMap[i][j].m_nCategory, 0);
				g_nowMap[i][j].m_bOnBox = false;
				break;
			case GOAL:
				g_nowMap[i][j].m_nObject = g_pMonument->Create(XMFLOAT3(-90.0f + j * 10.0f, -10 - i * 10.0f, 0.0f), g_nowMap[i][j].m_nCategory);
				g_nowMap[i][j].m_bOnBox = false;
				break;
			}
		}
	}

	// ���̏d�͏�����
	g_bBoxGravity = true;

	return true;
}

//=============================
//		�I��
//=============================
void UninitMap() {
	for (int i = 0; i < MAP_HEIGHT; ++i) {
		for (int j = 0; j < MAP_WIDTH; ++j) {
			switch (g_oldMap[i][j].m_nCategory) {
			case 0:
				break;
			case NORMAL:
				g_pDWBox->Release(g_oldMap[i][j].m_nObject);
				break;
			case BREAK:
				g_pWoodBox->Release(g_oldMap[i][j].m_nObject);
				break;
			case CARRY:
				g_pBox->Release(g_oldMap[i][j].m_nObject);
				break;
			case GOAL:
				g_pMonument->Release(g_oldMap[i][j].m_nObject);
				break;
			default:
				g_pBox->Release(g_oldMap[i][j].m_nObject);
				g_pDWBox->Release(g_oldMap[i][j].m_nObject);
				break;
			}
			switch (g_nowMap[i][j].m_nCategory) {
			case 0:
				break;
			case NORMAL:
				g_pDWBox->Release(g_nowMap[i][j].m_nObject);
				break;
			case BREAK:
				g_pWoodBox->Release(g_nowMap[i][j].m_nObject);
				break;
			case CARRY:
				g_pBox->Release(g_nowMap[i][j].m_nObject);
				break;
			case GOAL:
				g_pMonument->Release(g_nowMap[i][j].m_nObject);
				break;
			default:
				g_pBox->Release(g_nowMap[i][j].m_nObject);
				g_pDWBox->Release(g_nowMap[i][j].m_nObject);
				break;
			}
		}
	}

	delete g_pBox;
	delete g_pDWBox;
	delete g_pHalfBox;
	//delete g_pPlayerBoy;
	delete g_pMonument;
}

//=============================
//		�X�V
//=============================
void UpdateMap() {
	g_bBoxGravity = true;
	XMFLOAT2 BoxPos;
	XMFLOAT2 BoxSize = XMFLOAT2(g_pBox->GetSize());
	// ���ɏd�͂�������
	for (int i = 0; i < MAP_HEIGHT; ++i) {
		for (int j = 0; j < MAP_WIDTH; ++j) {
			switch (g_oldMap[i][j].m_nGravity)
			{
			case 0:
				break;
			case 1:
				BoxPos = XMFLOAT2(g_pBox->GetPos(g_oldMap[i][j].m_nObject).x, g_pBox->GetPos(g_oldMap[i][j].m_nObject).y);
				std::vector<OBJECT_INFO> collision = BoxCollisionOldMap(BoxPos, BoxSize, g_oldMap[i][j].m_nObject);
				std::vector<OBJECT_INFO>::iterator it = collision.begin();
				while (it != collision.end())
				{
					if (it->m_nCategory > 0)
					{
						if (it->m_bOnBox == true)
							g_bBoxGravity = false;
					}
					it++;
				}
				if (g_bBoxGravity == true)
					g_pBox->SetGravity(g_oldMap[i][j].m_nObject, 1);
				g_bBoxGravity = true;
				break;
			}
			switch (g_nowMap[i][j].m_nGravity) 
			{
			case 0:
				break;
			case 1:
				BoxPos = XMFLOAT2(g_pBox->GetPos(g_nowMap[i][j].m_nObject).x, g_pBox->GetPos(g_nowMap[i][j].m_nObject).y);
				std::vector<OBJECT_INFO> collision = BoxCollisionNowMap(BoxPos, BoxSize, g_oldMap[i][j].m_nObject);
				std::vector<OBJECT_INFO>::iterator it = collision.begin();
				while (it != collision.end())
				{
					if (it->m_nCategory > 0)
					{
						if (it->m_bOnBox == true)
							g_bBoxGravity = false;
					}
					it++;
				}
				if (g_bBoxGravity == true)
					g_pBox->SetGravity(g_nowMap[i][j].m_nObject, 1);
				g_bBoxGravity = true;
				break;
			}
		}
	}
	g_pBox->Update();
	g_pDWBox->Update();
	g_pHalfBox->Update();
	g_pWoodBox->Update();
	g_pMonument->Update();
	static bool m_flg;
	//m_flg = g_pPlayerBoy->GetHaveFlg();
	if (m_flg) {
		for (int i = 0; i < MAP_HEIGHT; ++i) {
			for (int j = 0; j < MAP_WIDTH; ++j) {
				switch (g_nowMap[i][j].m_nObject) {
				case 0:
					break;
				case NORMAL:
					break;
				case BREAK:
					break;
				case CARRY:

					break;
				}
			}

		}
	}
}

//=============================
//		�`��
//=============================
void DrawOldMap() {
	for (int i = 0; i < MAP_HEIGHT; ++i) {
		for (int j = 0; j < MAP_WIDTH; ++j) {
			switch (g_oldMap[i][j].m_nCategory) {
			case 0:
				break;
			case NORMAL:
				g_pDWBox->Draw(g_oldMap[i][j].m_nObject);
				break;
			case BREAK:
				g_pWoodBox->Draw(g_oldMap[i][j].m_nObject);
				break;
			case CARRY:
				g_pBox->Draw(g_oldMap[i][j].m_nObject);
				break;
			case GOAL:
				g_pMonument->Draw(g_oldMap[i][j].m_nObject);
				break;
			}
		}
	}
}

//=============================
//		�`��
//=============================
void DrawNowMap() {
	for (int i = 0; i < MAP_HEIGHT; ++i) {
		for (int j = 0; j < MAP_WIDTH; ++j) {
			switch (g_nowMap[i][j].m_nCategory) {
			case 0:
				break;
			case NORMAL:
				g_pDWBox->Draw(g_nowMap[i][j].m_nObject);
				break;
			case BREAK:
				g_pWoodBox->Draw(g_nowMap[i][j].m_nObject);
				break;
			case CARRY:
				g_pBox->Draw(g_nowMap[i][j].m_nObject);
				break;
			case GOAL:
				g_pMonument->Draw(g_nowMap[i][j].m_nObject);
				break;
			case HALF:
				g_pHalfBox->Draw(g_nowMap[i][j].m_nObject);
				break;
			}
		}
	}
}

//=============================
//		�ړ��p�̓����蔻��ߋ�
//=============================
std::vector<OBJECT_INFO> WalkCollisionOldMap(XMFLOAT2 pos, XMFLOAT2 size) {
	std::vector<OBJECT_INFO> collisionObj;
	XMFLOAT2 BoxPos;
	XMFLOAT2 BoxSize;
	for (int i = 0; i < MAP_HEIGHT; ++i) {
		for (int j = 0; j < MAP_WIDTH; ++j) {
			switch (g_oldMap[i][j].m_nCategory) {
			case 0:
				break;
			case NORMAL:
				//DWbox�Ƃ̓����蔻��
				if (!g_pDWBox->GetState(g_oldMap[i][j].m_nObject))
				{
					break;
				}
				BoxPos = XMFLOAT2(g_pDWBox->GetPos(g_oldMap[i][j].m_nObject).x, g_pDWBox->GetPos(g_oldMap[i][j].m_nObject).y);
				BoxSize = g_pDWBox->GetSize(g_oldMap[i][j].m_nObject);
				if (CollisionRect(pos, size, BoxPos, BoxSize))
				{
					if (pos.y >= BoxPos.y + BoxSize.y)
						g_oldMap[i][j].m_bOnBox = true;
					else
						g_oldMap[i][j].m_bOnBox = false;

					collisionObj.push_back(g_oldMap[i][j]);
				}
				break;
			case BREAK:
				//box�Ƃ̓����蔻��
				if (!g_pWoodBox->GetState(g_oldMap[i][j].m_nObject))
				{
					break;
				}
				BoxPos = XMFLOAT2(g_pWoodBox->GetPos(g_oldMap[i][j].m_nObject).x, g_pWoodBox->GetPos(g_oldMap[i][j].m_nObject).y);
				BoxSize = g_pWoodBox->GetSize();
				if (CollisionRect(pos, size, BoxPos, BoxSize))
				{
					if (pos.y >= BoxPos.y + BoxSize.y)
						g_oldMap[i][j].m_bOnBox = true;
					else
						g_oldMap[i][j].m_bOnBox = false;

					collisionObj.push_back(g_oldMap[i][j]);
				}
				break;
			case CARRY:
				//box�Ƃ̓����蔻��
				if (!g_pBox->GetState(g_oldMap[i][j].m_nObject))
				{
					break;
				}
				BoxPos = XMFLOAT2(g_pBox->GetPos(g_oldMap[i][j].m_nObject).x, g_pBox->GetPos(g_oldMap[i][j].m_nObject).y);
				BoxSize = g_pBox->GetSize();
				if (CollisionRect(pos, size, BoxPos, BoxSize))
				{
					if (pos.y >= BoxPos.y + BoxSize.y)
						g_oldMap[i][j].m_bOnBox = true;
					else
						g_oldMap[i][j].m_bOnBox = false;

					collisionObj.push_back(g_oldMap[i][j]);
				}
				break;
			}
		}
	}

	return collisionObj;
}

//=============================
//		�ړ��p�̓����蔻�茻��
//=============================
std::vector<OBJECT_INFO> WalkCollisionNowMap(XMFLOAT2 pos, XMFLOAT2 size) {
	std::vector<OBJECT_INFO> collisionObj;
	XMFLOAT2 BoxPos;
	XMFLOAT2 BoxSize;
	for (int i = 0; i < MAP_HEIGHT; ++i) {
		for (int j = 0; j < MAP_WIDTH; ++j) {
			switch (g_nowMap[i][j].m_nCategory) {
			case 0:
				break;
			case NORMAL:
				//DWbox�Ƃ̓����蔻��
				if (!g_pBox->GetState(g_nowMap[i][j].m_nObject))
				{
					break;
				}
				BoxPos = XMFLOAT2(g_pDWBox->GetPos(g_nowMap[i][j].m_nObject).x, g_pDWBox->GetPos(g_nowMap[i][j].m_nObject).y);
				BoxSize = g_pDWBox->GetSize(g_nowMap[i][j].m_nObject);
				if (CollisionRect(pos, size, BoxPos, BoxSize))
				{
					if (pos.y >= BoxPos.y + BoxSize.y)
						g_nowMap[i][j].m_bOnBox = true;
					else
						g_nowMap[i][j].m_bOnBox = false;

					collisionObj.push_back(g_nowMap[i][j]);
				}
				break;
			case BREAK:
				//box�Ƃ̓����蔻��
				if (!g_pWoodBox->GetState(g_nowMap[i][j].m_nObject))
				{
					break;
				}
				BoxPos = XMFLOAT2(g_pWoodBox->GetPos(g_nowMap[i][j].m_nObject).x, g_pWoodBox->GetPos(g_nowMap[i][j].m_nObject).y);
				BoxSize = g_pWoodBox->GetSize();
				if (CollisionRect(pos, size, BoxPos, BoxSize))
				{
					if (pos.y >= BoxPos.y + BoxSize.y)
						g_nowMap[i][j].m_bOnBox = true;
					else
						g_nowMap[i][j].m_bOnBox = false;

					collisionObj.push_back(g_nowMap[i][j]);
				}
				break;
			case CARRY:
				//box�Ƃ̓����蔻��
				if (!g_pBox->GetState(g_nowMap[i][j].m_nObject))
				{
					break;
				}
				BoxPos = XMFLOAT2(g_pBox->GetPos(g_nowMap[i][j].m_nObject).x, g_pBox->GetPos(g_nowMap[i][j].m_nObject).y);
				BoxSize = g_pBox->GetSize();
				if (CollisionRect(pos, size, BoxPos, BoxSize))
				{
					if (pos.y >= BoxPos.y + BoxSize.y)
						g_nowMap[i][j].m_bOnBox = true;
					else
						g_nowMap[i][j].m_bOnBox = false;

					collisionObj.push_back(g_nowMap[i][j]);
				}
				break;
			}
		}
	}

	return collisionObj;
}

//=============================
//		�����蔻��ߋ�
//=============================
OBJECT_INFO CollisionOldMap(XMFLOAT2 pos, XMFLOAT2 size) {
	XMFLOAT2 BoxPos;
	XMFLOAT2 BoxSize;
	for (int i = 0; i < MAP_HEIGHT; ++i) {
		for (int j = 0; j < MAP_WIDTH; ++j) {
			switch (g_oldMap[i][j].m_nCategory) {
			case 0:
				break;
			case NORMAL:
				//DWbox�Ƃ̓����蔻��
				if (!g_pBox->GetState(g_oldMap[i][j].m_nObject))
				{
					break;
				}
				BoxPos = XMFLOAT2(g_pDWBox->GetPos(g_oldMap[i][j].m_nObject).x, g_pDWBox->GetPos(g_oldMap[i][j].m_nObject).y);
				BoxSize = g_pDWBox->GetSize(g_oldMap[i][j].m_nObject);
				if (CollisionRect(pos, size, BoxPos, BoxSize))
				{
					if (pos.y >= BoxPos.y + BoxSize.y)
						g_oldMap[i][j].m_bOnBox = true;
					else
						g_oldMap[i][j].m_bOnBox = false;

					return g_oldMap[i][j];
				}
				break;
			case BREAK:
				//box�Ƃ̓����蔻��
				if (!g_pWoodBox->GetState(g_oldMap[i][j].m_nObject))
				{
					break;
				}
				BoxPos = XMFLOAT2(g_pWoodBox->GetPos(g_oldMap[i][j].m_nObject).x, g_pWoodBox->GetPos(g_oldMap[i][j].m_nObject).y);
				BoxSize = g_pWoodBox->GetSize();
				if (CollisionRect(pos, size, BoxPos, BoxSize))
				{
					if (pos.y >= BoxPos.y + BoxSize.y)
						g_oldMap[i][j].m_bOnBox = true;
					else
						g_oldMap[i][j].m_bOnBox = false;

					return g_oldMap[i][j];
				}
				break;
			case CARRY:
				//box�Ƃ̓����蔻��
				if (!g_pBox->GetState(g_oldMap[i][j].m_nObject))
				{
					break;
				}
				BoxPos = XMFLOAT2(g_pBox->GetPos(g_oldMap[i][j].m_nObject).x, g_pBox->GetPos(g_oldMap[i][j].m_nObject).y);
				BoxSize = g_pBox->GetSize();
				if (CollisionRect(pos, size, BoxPos, BoxSize))
				{
					if (pos.y >= BoxPos.y + BoxSize.y)
						g_oldMap[i][j].m_bOnBox = true;
					else
						g_oldMap[i][j].m_bOnBox = false;

					return g_oldMap[i][j];
				}
				break;
			}
		}
	}

	return OBJECT_INFO{ -1,-1 };
}

//=============================
//		�����蔻�茻��
//=============================
OBJECT_INFO	CollisionNowMap(XMFLOAT2 pos, XMFLOAT2 size) {
	XMFLOAT2 BoxPos;
	XMFLOAT2 BoxSize;
	for (int i = 0; i < MAP_HEIGHT; ++i) {
		for (int j = 0; j < MAP_WIDTH; ++j) {
			switch (g_nowMap[i][j].m_nCategory) {
			case 0:
				break;
			case NORMAL:
				//DWbox�Ƃ̓����蔻��
				if (!g_pBox->GetState(g_oldMap[i][j].m_nObject))
				{
					break;
				}
				BoxPos = XMFLOAT2(g_pDWBox->GetPos(g_oldMap[i][j].m_nObject).x, g_pDWBox->GetPos(g_oldMap[i][j].m_nObject).y);
				BoxSize = g_pDWBox->GetSize(g_oldMap[i][j].m_nObject);
				if (CollisionRect(pos, size, BoxPos, BoxSize))
				{
					if (pos.y >= BoxPos.y + BoxSize.y)
						g_oldMap[i][j].m_bOnBox = true;
					else
						g_oldMap[i][j].m_bOnBox = false;

					return g_oldMap[i][j];
				}
				break;
			case BREAK:
				//Woodbox�Ƃ̓����蔻��
				if (!g_pWoodBox->GetState(g_nowMap[i][j].m_nObject))
				{
					break;
				}
				BoxPos = XMFLOAT2(g_pWoodBox->GetPos(g_nowMap[i][j].m_nObject).x, g_pWoodBox->GetPos(g_nowMap[i][j].m_nObject).y);
				BoxSize = g_pWoodBox->GetSize();
				if (CollisionRect(pos, size, BoxPos, BoxSize))
				{
					if (pos.y >= BoxPos.y + BoxSize.y)
						g_nowMap[i][j].m_bOnBox = true;
					else
						g_nowMap[i][j].m_bOnBox = false;

					return g_nowMap[i][j];
				}
				break;
			case CARRY:
				//box�Ƃ̓����蔻��
				if (!g_pBox->GetState(g_nowMap[i][j].m_nObject))
				{
					break;
				}
				BoxPos = XMFLOAT2(g_pBox->GetPos(g_nowMap[i][j].m_nObject).x, g_pBox->GetPos(g_nowMap[i][j].m_nObject).y);
				BoxSize = g_pBox->GetSize();
				if (CollisionRect(pos, size, BoxPos, BoxSize))
				{
					if (pos.y >= BoxPos.y + BoxSize.y)
						g_nowMap[i][j].m_bOnBox = true;
					else
						g_nowMap[i][j].m_bOnBox = false;

					return g_nowMap[i][j];
				}
				break;
			case HALF:
				//HalfBox�Ƃ̓����蔻��
				if (!g_pHalfBox->GetState(g_nowMap[i][j].m_nObject))
				{
					break;
				}
				BoxPos = XMFLOAT2(g_pHalfBox->GetPos(g_nowMap[i][j].m_nObject).x, g_pHalfBox->GetPos(g_nowMap[i][j].m_nObject).y);
				BoxSize = g_pHalfBox->GetSize(g_nowMap[i][j].m_nObject);
				if (CollisionRect(pos, size, BoxPos, BoxSize))
				{
					if (pos.y >= BoxPos.y + BoxSize.y)
						g_nowMap[i][j].m_bOnBox = true;
					else
						g_nowMap[i][j].m_bOnBox = false;

					return g_nowMap[i][j];
				}
				break;
			}
		}
	}
	return OBJECT_INFO{ -1,-1 };
}

//=============================
//		���p�̓����蔻��ߋ�
//=============================
std::vector<OBJECT_INFO> BoxCollisionOldMap(XMFLOAT2 pos, XMFLOAT2 size, int nObject) {
	std::vector<OBJECT_INFO> collisionObj;
	XMFLOAT2 BoxPos;
	XMFLOAT2 BoxSize;
	for (int i = 0; i < MAP_HEIGHT; ++i) {
		for (int j = 0; j < MAP_WIDTH; ++j) {
			if (g_oldMap[i][j].m_nObject == nObject)
				continue;
			switch (g_oldMap[i][j].m_nCategory) {
			case 0:
				break;
			case NORMAL:
				//DWbox�Ƃ̓����蔻��
				if (!g_pDWBox->GetState(g_oldMap[i][j].m_nObject))
				{
					break;
				}
				BoxPos = XMFLOAT2(g_pDWBox->GetPos(g_oldMap[i][j].m_nObject).x, g_pDWBox->GetPos(g_oldMap[i][j].m_nObject).y);
				BoxSize = g_pDWBox->GetSize(g_oldMap[i][j].m_nObject);
				if (CollisionRect(pos, size, BoxPos, BoxSize))
				{
					if (pos.y >= BoxPos.y + BoxSize.y)
						g_oldMap[i][j].m_bOnBox = true;
					else
						g_oldMap[i][j].m_bOnBox = false;

					collisionObj.push_back(g_oldMap[i][j]);
				}
				break;
			case BREAK:
				//woodbox�Ƃ̓����蔻��
				if (!g_pWoodBox->GetState(g_oldMap[i][j].m_nObject))
				{
					break;
				}
				BoxPos = XMFLOAT2(g_pWoodBox->GetPos(g_oldMap[i][j].m_nObject).x, g_pWoodBox->GetPos(g_oldMap[i][j].m_nObject).y);
				BoxSize = g_pWoodBox->GetSize();
				if (CollisionRect(pos, size, BoxPos, BoxSize))
				{
					if (pos.y >= BoxPos.y + BoxSize.y)
						g_oldMap[i][j].m_bOnBox = true;
					else
						g_oldMap[i][j].m_bOnBox = false;

					collisionObj.push_back(g_oldMap[i][j]);
				}
				break;
			case CARRY:
				//box�Ƃ̓����蔻��
				if (!g_pBox->GetState(g_oldMap[i][j].m_nObject))
				{
					break;
				}
				BoxPos = XMFLOAT2(g_pBox->GetPos(g_oldMap[i][j].m_nObject).x, g_pBox->GetPos(g_oldMap[i][j].m_nObject).y);
				BoxSize = g_pBox->GetSize();
				if (CollisionRect(pos, size, BoxPos, BoxSize))
				{
					if (pos.y >= BoxPos.y + BoxSize.y)
						g_oldMap[i][j].m_bOnBox = true;
					else
						g_oldMap[i][j].m_bOnBox = false;

					collisionObj.push_back(g_oldMap[i][j]);
				}
				break;
			}
		}
	}

	return collisionObj;
}

//=============================
//		���p�̓����蔻�茻��
//=============================
std::vector<OBJECT_INFO> BoxCollisionNowMap(XMFLOAT2 pos, XMFLOAT2 size, int nObject) {
	std::vector<OBJECT_INFO> collisionObj;
	XMFLOAT2 BoxPos;
	XMFLOAT2 BoxSize;
	for (int i = 0; i < MAP_HEIGHT; ++i) {
		for (int j = 0; j < MAP_WIDTH; ++j) {
			if (g_nowMap[i][j].m_nObject == nObject)
				continue;
			switch (g_nowMap[i][j].m_nCategory) {
			case 0:
				break;
			case NORMAL:
				//DWbox�Ƃ̓����蔻��
				if (!g_pBox->GetState(g_nowMap[i][j].m_nObject))
				{
					break;
				}
				BoxPos = XMFLOAT2(g_pDWBox->GetPos(g_nowMap[i][j].m_nObject).x, g_pDWBox->GetPos(g_nowMap[i][j].m_nObject).y);
				BoxSize = g_pDWBox->GetSize(g_nowMap[i][j].m_nObject);
				if (CollisionRect(pos, size, BoxPos, BoxSize))
				{
					if (pos.y >= BoxPos.y + BoxSize.y)
						g_nowMap[i][j].m_bOnBox = true;
					else
						g_nowMap[i][j].m_bOnBox = false;

					collisionObj.push_back(g_nowMap[i][j]);
				}
				break;
			case BREAK:
				//box�Ƃ̓����蔻��
				if (!g_pWoodBox->GetState(g_nowMap[i][j].m_nObject))
				{
					break;
				}
				BoxPos = XMFLOAT2(g_pWoodBox->GetPos(g_nowMap[i][j].m_nObject).x, g_pWoodBox->GetPos(g_nowMap[i][j].m_nObject).y);
				BoxSize = g_pWoodBox->GetSize();
				if (CollisionRect(pos, size, BoxPos, BoxSize))
				{
					if (pos.y >= BoxPos.y + BoxSize.y)
						g_nowMap[i][j].m_bOnBox = true;
					else
						g_nowMap[i][j].m_bOnBox = false;

					collisionObj.push_back(g_nowMap[i][j]);
				}
				break;
			case CARRY:
				//box�Ƃ̓����蔻��
				if (!g_pBox->GetState(g_nowMap[i][j].m_nObject))
				{
					break;
				}
				BoxPos = XMFLOAT2(g_pBox->GetPos(g_nowMap[i][j].m_nObject).x, g_pBox->GetPos(g_nowMap[i][j].m_nObject).y);
				BoxSize = g_pBox->GetSize();
				if (CollisionRect(pos, size, BoxPos, BoxSize))
				{
					if (pos.y >= BoxPos.y + BoxSize.y)
						g_nowMap[i][j].m_bOnBox = true;
					else
						g_nowMap[i][j].m_bOnBox = false;

					collisionObj.push_back(g_nowMap[i][j]);
				}
				break;
			}
		}
	}

	return collisionObj;
}

//==========================
//		���@�擾
//==========================
Box* GetBox() {
	return g_pBox;
}

//==========================
//	�����Ȃ����@�擾
//==========================
DWBox* GetDWBox() {
	return g_pDWBox;
}

//==========================
//	�n�[�t�u���b�N�@�擾
//==========================
HalfBox* GetHalfBox() {
	return g_pHalfBox;
}

//==========================
//	�ؔ��@�擾
//==========================
WoodBox* GetWoodBox() {
	return g_pWoodBox;
}

//==========================
//		�}�b�v�@�擾
//==========================
OBJECT_INFO* GetMap(int nTime)
{
	switch (nTime)
	{
	case 0:
		return *g_nowMap;
		break;
	case 1:
		return *g_oldMap;
		break;
	}
}

//==========================
//		�I�u�W�F�N�g�@�ݒ�
//==========================
void SetObject(int nObject, int nSetObject)
{
	for (int i = 0; i < MAP_HEIGHT; ++i) {
		for (int j = 0; j < MAP_WIDTH; ++j) {
			if(g_nowMap[i][j].m_nObject == nObject && g_nowMap[i][j].m_nCategory == CARRY)
			{
				g_nowMap[i][j].m_nObject = nSetObject;
				g_nowMap[i][j].m_nCategory = HALF;
			}
		}
	}
}

//==========================
//		�I�u�W�F�N�g�@�擾
//==========================
OBJECT_INFO GetObjectInfo(int nObject, int nCat)
{
	for (int i = 0; i < MAP_HEIGHT; ++i) {
		for (int j = 0; j < MAP_WIDTH; ++j) {
			if (g_nowMap[i][j].m_nObject == nObject && g_nowMap[i][j].m_nCategory == nCat)
				return g_nowMap[i][j];
		}
	}

	return { -1, -1 };
}