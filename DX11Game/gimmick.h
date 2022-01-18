//=============================================================================
//
// ギミック [gimmick.h]
// Author : 加藤　匠
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
