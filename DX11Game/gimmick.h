//=============================================================================
//
// �M�~�b�N [gimmick.h]
// Author : �����@��
//
//=============================================================================
#pragma once

#include "main.h"
#include "plant.h"

class Gimmick
{
public:
	Gimmick();
	~Gimmick();

	void Update(XMFLOAT3);

	void OldDraw();
	void NowDraw();
private:
	Plant* m_pPlant;
};
