//===================================================
//		����[now.h]
//����T�q
//=====================================================
#pragma once
#include "main.h"
#include "meshfield.h"
#include "playerGirl.h"

class Now {
public:
	Now();
	~Now();
	void Update();
	void Draw();

	Player_Girl* GetPlayerGirl();
private:
	MeshField* m_pMeshField;		//̨����
	Player_Girl* m_pPlayerGirl;		//���̎q
};