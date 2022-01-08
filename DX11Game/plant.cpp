//=============================================================================
//
// 植物 [plant.cpp]
// Author : 加藤　匠
//
//=============================================================================
#include "plant.h"
#include "collision.h"
#include "playerGirl.h"
#include "sceneGame.h"

// グローバル変数
Player_Girl* g_pPlayerGirl;

Plant::Plant()
{
	m_pTree = new Tree;
}
Plant::~Plant()
{
	delete m_pTree;
}
void Plant::Update()
{
	g_pPlayerGirl = GetNow()->GetPlayerGirl();
	if (g_pPlayerGirl->GetGirlPos().x >= m_treePos.x - 3.5f && g_pPlayerGirl->GetGirlPos().x <= m_treePos.x + 3.5f)
		g_pPlayerGirl->SetGirlPos(m_treePos);
	m_pTree->Update();
}

void Plant::OldDraw()
{
	m_pTree->DrawOldNow(1);
}

void Plant::NowDraw()
{
	m_pTree->DrawOldNow(0);
}

// 木の種を植える(男の子の行動)
void Plant::Plants(XMFLOAT3 pos)
{
	pos.x += 2.0f;
	pos.y += -8.0f;
	m_pTree->CreateOldNow(pos, 1);
}

// 木が生える(今での変化)
void Plant::Grow(XMFLOAT3 pos)
{
	pos.x += 2.0f;
	pos.y += -8.0f;
	m_pTree->CreateOldNow(pos, 0);
	
}