//=============================================================================
//
// �}�b�v [map.h]
// ����T�q
//=============================================================================
#pragma once
#include "main.h"
#include "box.h"
#include "DWBox.h"
#include "HalfBox.h"
#include "Monument.h"
#include <vector>

//*****�\���̒�`*****
typedef struct {
	int m_nCategory;	//�I�u�W�F�N�g�̎��
	int	m_nObject;		//�I�u�W�F�N�g�Ǘ��ԍ�
	int m_nGravity;		//���ɏd�͂��������Ă邩�ǂ���(1:�L, 0:����)
	bool m_bOnBox;		//��̔���p
}OBJECT_INFO;

//*****�񋓌^*****
// �I�u�W�F�N�g�̎��
enum MAP_OBJECT 
{
	NONE,		// �Ȃɂ��Ȃ�
	NORMAL,		// ��
	BREAK,		// �󂹂锠
	CARRY,		// �^�ׂ锠
	PUSH,		// �����锠
	GOAL,		// �S�[���̐Δ�
};		

//*****�}�N����`*****
#define MAP_WIDTH	35
#define MAP_HEIGHT	5

#define MAP_END	(270.0f)

HRESULT InitMap();
void UninitMap();
void UpdateMap();
void DrawOldMap();
void DrawNowMap();

// ���܂ł̓����蔻��
OBJECT_INFO CollisionOldMap(XMFLOAT2 pos, XMFLOAT2 size);
OBJECT_INFO	CollisionNowMap(XMFLOAT2 pos, XMFLOAT2 size);

// �ړ��p�̓����蔻��
std::vector<OBJECT_INFO> WalkCollisionOldMap(XMFLOAT2 pos, XMFLOAT2 size);
std::vector<OBJECT_INFO> WalkCollisionNowMap(XMFLOAT2 pos, XMFLOAT2 size);

// ���p�̓����蔻��
std::vector<OBJECT_INFO> BoxCollisionOldMap(XMFLOAT2 pos, XMFLOAT2 size, int nObject);
std::vector<OBJECT_INFO> BoxCollisionNowMap(XMFLOAT2 pos, XMFLOAT2 size, int nObject);

Box* GetBox();
DWBox* GetDWBox();
HalfBox* GetHalfBox();
Monument* GetMonument();
OBJECT_INFO* GetMap(int);
