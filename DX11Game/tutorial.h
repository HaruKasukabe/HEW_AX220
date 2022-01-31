// チュートリアル描画 [tutorial.h]
#pragma once
#include "main.h"

#define MAX_TUTORIAL_TEXTURE	(3)

class Tutorial {
public:
	Tutorial();
	~Tutorial();
	void Update(XMFLOAT3);
	void Draw();
private:
	XMFLOAT2	m_pos;
	XMFLOAT2	m_size;
	ID3D11ShaderResourceView* m_pTexture[MAX_TUTORIAL_TEXTURE];
};
