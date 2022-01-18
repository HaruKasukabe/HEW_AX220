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
#include "Monument.h"
#include "playerBoy.h"
#include "mapData.h"

OBJECT_INFO g_nowMap[MAP_HEIGHT][MAP_WIDTH] =
{
	{{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},},
	{{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},},
	{{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},},
	{{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{3,0,1},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},},
	{{0,0},{0,0},{0,0},{3,0},{0,0},{0,0},{0,0},{1,0},{1,0},{1,0},{1,0},{1,0},{1,0},{1,0},{0,0},{0,0},{0,0},{0,0},{5,0},},
};

OBJECT_INFO g_oldMap[MAP_HEIGHT][MAP_WIDTH] =
{
	{{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},},
	{{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{3,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},},
	{{0,0},{0,0},{2,0},{3,0},{0,0},{1,0},{0,0},{2,0},{0,0},{0,0},{0,0},{0,0},{3,0},{0,0},{2,0},{0,0},{0,0},{0,0},{0,0},},
	{{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{3,0,1},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},},
	{{0,0},{0,0},{0,0},{3,0,1},{0,0},{0,0},{0,0},{1,0},{1,0},{1,0},{1,0},{1,0},{1,0},{1,0},{2,0},{0,0},{0,0},{0,0},{5,0},},
};

//*****�O���[�o���ϐ�*****
static Box* g_pBox;
static DWBox* g_pDWBox;
static HalfBox* g_pHalfBox;
static Player_Boy* g_pPlayerBoy;
static Monument* g_pMonument;

//=============================
//		������
//=============================
HRESULT InitMap() {
	// �Z���N�g�����X�e�[�W���ƂɃ}�b�v��ݒ�
	g_nowMap[MAP_HEIGHT][MAP_WIDTH] = *GetMapData(0).nowMap;
	g_oldMap[MAP_HEIGHT][MAP_WIDTH] = *GetMapData(0).pOldMap;

	g_pBox = new Box;
	g_pDWBox = new DWBox;
	g_pHalfBox = new HalfBox;
	g_pPlayerBoy = new Player_Boy;
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
				g_oldMap[i][j].m_nObject = g_pBox->Create(XMFLOAT3(-90.0f + j * 10.0f, -10 - i * 10.0f, 0.0f), g_oldMap[i][j].m_nCategory, 1);
				g_oldMap[i][j].m_bOnBox = false;
				break;
			case CARRY:
				g_oldMap[i][j].m_nObject = g_pBox->Create(XMFLOAT3(-90.0f + j * 10.0f, -10 - i * 10.0f, 0.0f), g_oldMap[i][j].m_nCategory, 1);
				g_oldMap[i][j].m_bOnBox = false;
				break;
			case GOAL:
				g_oldMap[i][j].m_nObject = g_pMonument->Create(XMFLOAT3(100.0f, -45.0f, 0.0f),0);
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
				g_nowMap[i][j].m_nObject = g_pBox->Create(XMFLOAT3(-90.0f + j * 10.0f, -10 - i * 10.0f, 0.0f), g_nowMap[i][j].m_nCategory, 0);
				g_nowMap[i][j].m_bOnBox = false;
			case CARRY:
				g_nowMap[i][j].m_nObject = g_pBox->Create(XMFLOAT3(-90.0f + j * 10.0f, -10 - i * 10.0f, 0.0f), g_nowMap[i][j].m_nCategory, 0);
				g_nowMap[i][j].m_bOnBox = false;
				break;
			case GOAL:
				g_nowMap[i][j].m_nObject = g_pMonument->Create(XMFLOAT3(100.0f, -45.0f, 0.0f), g_nowMap[i][j].m_nCategory);
				g_nowMap[i][j].m_bOnBox = false;
				break;
			}
		}
	}
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
				g_pBox->Release(g_oldMap[i][j].m_nObject);
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
				g_pBox->Release(g_nowMap[i][j].m_nObject);
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

	if (g_pHalfBox->GetObj().size() != 0)
	{
		std::vector<int>::iterator it;
		it = g_pHalfBox->GetObj().begin();
		while (it != g_pHalfBox->GetObj().end())
		{
			g_pHalfBox->Release(*it);
			it++;
		}
	}

	delete g_pBox;
	delete g_pDWBox;
	delete g_pHalfBox;
	delete g_pPlayerBoy;
	delete g_pMonument;
}

//=============================
//		�X�V
//=============================
void UpdateMap() {
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
				BoxPos = XMFLOAT2(g_pBox->GetPos(g_oldMap[i][j].m_nObject).x, g_pBox->GetPos(g_oldMap[i][j].m_nObject).y - 34.0f);
				if(!(CollisionOldMap(BoxPos, BoxSize).m_nCategory > 0))
					g_pBox->SetGravity(g_oldMap[i][j].m_nObject, 1);
				break;
			}
			switch (g_nowMap[i][j].m_nGravity) 
			{
			case 0:
				break;
			case 1:
				BoxPos = XMFLOAT2(g_pBox->GetPos(g_nowMap[i][j].m_nObject).x, g_pBox->GetPos(g_nowMap[i][j].m_nObject).y - 34.0f);
				if (!(CollisionNowMap(BoxPos, BoxSize).m_nCategory > 0))
					g_pBox->SetGravity(g_nowMap[i][j].m_nObject, 1);
				break;
			}
		}
	}
	g_pBox->Update();
	g_pDWBox->Update();
	g_pHalfBox->Update();
	g_pMonument->Update();
	static bool m_flg;
	m_flg = g_pPlayerBoy->GetHaveFlg();
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
				g_pBox->Draw(g_oldMap[i][j].m_nObject);
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

	if (g_pHalfBox->GetObj().size() != 0)
	{
		std::vector<int>::iterator it;
		it = g_pHalfBox->GetObj().begin();
		while (it != g_pHalfBox->GetObj().end())
		{
			g_pHalfBox->Draw(*it);
			it++;
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
				g_pBox->Draw(g_nowMap[i][j].m_nObject);
				break;
			case CARRY:
				g_pBox->Draw(g_nowMap[i][j].m_nObject);
				break;
			case GOAL:
				g_pMonument->Draw(g_nowMap[i][j].m_nObject);
				break;
			}
		}
	}

	//g_pHalfBox->Draw();
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

					collisionObj.push_back(g_oldMap[i][j]);
				}
				break;
			case BREAK:
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
			}
		}
	}
	return OBJECT_INFO{ -1,-1 };
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