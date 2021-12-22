//=============================================================================
//
//  ÉSÅ[Éã[Goal.h]
// ïêà‰óyìs
//=============================================================================
#pragma once
#include "main.h"

class Goal {
public:
	Goal();
	~Goal();
	void Update(float);
	void Draw();
private:
	XMFLOAT2	m_pos;
	XMFLOAT2	m_size;
	ID3D11ShaderResourceView* m_pTexture;
};