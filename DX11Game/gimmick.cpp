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
	m_pTree = new Tree;
}
Gimmick::~Gimmick()
{
	delete m_pTree;
}
void Gimmick::Update(XMFLOAT3 pos)
{
	if (GetKeyTrigger(VK_P))
	{
		m_pTree->Plant(pos);
		m_pTree->Grow(pos);
	}
	m_pTree->Update();
}
void Gimmick::OldDraw()
{
	m_pTree->OldDraw();
}
void Gimmick::NowDraw()
{
	m_pTree->NowDraw();
}