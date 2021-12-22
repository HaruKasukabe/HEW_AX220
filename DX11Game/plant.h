//=============================================================================
//
// êAï® [plant.h]
// Author : â¡ì°Å@è†
//
//=============================================================================
#pragma once

#include "box.h"

class Tree
{
public:
	Tree();
	~Tree();

	void Update();

	void OldDraw();
	void NowDraw();

	void Plant(XMFLOAT3);		// êAÇ¶ÇÈ
	void Grow(XMFLOAT3);		// ê∂Ç¶ÇÈ

private:
	Box* m_pBox;
	XMFLOAT3 m_treePos;
};
