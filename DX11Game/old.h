//===================================================
//		‰ß‹[old.h]
//¬“í—Tq
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
	XMFLOAT3 GetBoyPos();
private:
	MeshField* m_pMeshField;		//Ì¨°ÙÄŞ
	Player_Boy* m_pPlayerBoy;		//’j‚Ìq
};