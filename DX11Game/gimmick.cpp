//=============================================================================
//
// ギミック [gimmick.cpp]
// Author : 加藤　匠
//
//=============================================================================
#include "gimmick.h"
#include "input.h"

Gimmick::Gimmick()
{
	m_pPlant = new Plant;
}
Gimmick::~Gimmick()
{
	delete m_pPlant;
}
void Gimmick::Update(XMFLOAT3 pos)
{
	if (GetKeyTrigger(VK_P))
	{
		m_pPlant->Plants(pos);
		m_pPlant->Grow(pos);
	}
	m_pPlant->Update();
}
void Gimmick::OldDraw()
{
	m_pPlant->OldDraw();
}
void Gimmick::NowDraw()
{
	m_pPlant->NowDraw();
}