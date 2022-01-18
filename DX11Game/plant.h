//=============================================================================
//
// �A�� [plant.h]
// Author : �����@��
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

	void Plants(XMFLOAT3);		// �A����
	void Grow(XMFLOAT3);		// ������

private:
	Tree* m_pTree;
	XMFLOAT3 m_treePos;
};
