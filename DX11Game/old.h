//===================================================
//		�ߋ�[old.h]
//����T�q
//=====================================================
#pragma once
#include "main.h"
#include "meshfield.h"
#include "playerBoy.h"

class Old {
public:
	Old();
	~Old();
	void Update();
	void Draw();

	Player_Boy* GetPlayerBoy();
private:
	MeshField* m_pMeshField;		//̨����
	Player_Boy* m_pPlayerBoy;		//�j�̎q
};