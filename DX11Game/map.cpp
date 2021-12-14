//=============================================================================
//
// �}�b�v [map.cpp]
// ����T�q
//=============================================================================
#include "map.h"
#include "box.h"
#include "collision.h"
//*****�}�N����`*****
#define MAP_WIDTH	19
#define MAP_HEIGHT	5



OBJECT_INFO g_oldMap[MAP_HEIGHT][MAP_WIDTH] =
{
	{{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},},
	{{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},},
	{{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},},
	{{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},},
	{{0,0},{0,0},{1,0},{0,0},{0,0},{0,0},{0,0},{1,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{1,0},{0,0},{0,0},{0,0},{0,0},},
};

OBJECT_INFO g_nowMap[MAP_HEIGHT][MAP_WIDTH] =
{
	{{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{1,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},},
	{{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{1,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},},
	{{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{1,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},},
	{{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{1,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},},
	{{0,0},{0,0},{1,0},{0,0},{0,0},{0,0},{0,0},{1,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{1,0},{0,0},{0,0},{0,0},{0,0},},
};

static Box* g_pBox;

//=============================
//		������
//=============================
HRESULT InitMap() {
	g_pBox = new Box;
	for (int i = 0; i < MAP_HEIGHT; ++i) {
		for (int j = 0; j < MAP_WIDTH; ++j) {
			switch (g_oldMap[i][j].m_nCategory) {
			case 0:
				break;
			case 1:
				g_oldMap[i][j].m_nObject = g_pBox->Create(XMFLOAT3(-90.0f+ j * 10.0f,30 - i * 20.0f,0.0f));
				break;
			}
			switch (g_nowMap[i][j].m_nCategory) {
			case 0:
				break;
			case 1:
				g_nowMap[i][j].m_nObject = g_pBox->Create(XMFLOAT3(-90.0f + j * 10.0f, 30 - i * 20.0f, 0.0f));
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
			case 1:
				g_pBox->Release(g_oldMap[i][j].m_nObject);
				break;
			}
			switch (g_nowMap[i][j].m_nCategory) {
			case 0:
				break;
			case 1:
				g_pBox->Release(g_nowMap[i][j].m_nObject);
				break;
			}
		}
	}
	delete g_pBox;
}

//=============================
//		�X�V
//=============================
void UpdateMap() {
	g_pBox->Update();
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
			case 1:
				g_pBox->Draw(g_oldMap[i][j].m_nObject);
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
			case 1:
				g_pBox->Draw(g_nowMap[i][j].m_nObject);
				break;
			}
		}
	}
}

//=============================
//		�����蔻��ߋ�
//=============================
OBJECT_INFO CollisionOldMap(XMFLOAT2 pos, XMFLOAT2 size) {
	for (int i = 0; i < MAP_HEIGHT; ++i) {
		for (int j = 0; j < MAP_WIDTH; ++j) {
			switch (g_oldMap[i][j].m_nCategory) {
			case 0:
				break;
			case 1:
				//box�Ƃ̓����蔻��
				if (!g_pBox->GetState(g_oldMap[i][j].m_nObject))
				{
					break;
				}
				XMFLOAT2 BoxPos = XMFLOAT2(g_pBox->GetPos(g_oldMap[i][j].m_nObject).x, g_pBox->GetPos(g_oldMap[i][j].m_nObject).y);
				XMFLOAT2 BoxSize = g_pBox->GetSize();
				if (CollisionRect(pos, size, BoxPos, BoxSize)) 
				{
					return g_oldMap[i][j];
				}
				break;
			}
		}
	}

	return OBJECT_INFO{-1,-1};
}

//=============================
//		�����蔻�茻��
//=============================
OBJECT_INFO	CollisionNowMap(XMFLOAT2 pos, XMFLOAT2 size) {
	for (int i = 0; i < MAP_HEIGHT; ++i) {
		for (int j = 0; j < MAP_WIDTH; ++j) {
			switch (g_nowMap[i][j].m_nCategory) {
			case 0:
				break;
			case 1:
				//box�Ƃ̓����蔻��
				if (!g_pBox->GetState(g_nowMap[i][j].m_nObject))
				{
					break;
				}
				XMFLOAT2 BoxPos = XMFLOAT2(g_pBox->GetPos(g_nowMap[i][j].m_nObject).x, g_pBox->GetPos(g_nowMap[i][j].m_nObject).y);
				XMFLOAT2 BoxSize = g_pBox->GetSize();
				if (CollisionRect(pos, size, BoxPos, BoxSize))
				{
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