//=============================================================================
//
// �}�b�v [map.h]
// ����T�q
//=============================================================================
#pragma once
#include "main.h"
#include "box.h"
//*****�\���̒�`*****
typedef struct {
	int m_nCategory;	//�I�u�W�F�N�g�̎��
	int m_nObject;		//�I�u�W�F�N�g�Ǘ��ԍ�
}OBJECT_INFO;

HRESULT InitMap();
void UninitMap();
void UpdateMap();
void DrawOldMap();
void DrawNowMap();

OBJECT_INFO CollisionOldMap(XMFLOAT2 pos, XMFLOAT2 size);
OBJECT_INFO	CollisionNowMap(XMFLOAT2 pos, XMFLOAT2 size);

Box* GetBox();
