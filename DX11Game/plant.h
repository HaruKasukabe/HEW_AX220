//=============================================================================
//
// �A�� [plant.h]
// Author : �����@��
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

	void Plant(XMFLOAT3);		// �A����
	void Grow(XMFLOAT3);		// ������

private:
	Box* m_pBox;
};
