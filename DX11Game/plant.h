//=============================================================================
//
// êAï® [plant.h]
// Author : â¡ì°Å@è†
//
//=============================================================================
#pragma once

#include "Tree.h"

class Plant
{
public:
	Plant();
	~Plant();

	void Update();

	void OldDraw();
	void NowDraw();

	void Plants(XMFLOAT3);		// êAÇ¶ÇÈ
	void Grow(XMFLOAT3);		// ê∂Ç¶ÇÈ

private:
	Tree* m_pTree;
	XMFLOAT3 m_treePos;
};
