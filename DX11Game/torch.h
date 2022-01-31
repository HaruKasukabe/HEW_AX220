//========================================================
//
//爆発処理[explosion.h]
//
//========================================================
#pragma once

#include "main.h"

struct TTorch
{
	XMFLOAT3	pos;		//位置
	XMFLOAT2	size;		//サイズ
	bool		use;		//使用しているかどうか
	int			anim;		//アニメーション再生位置
	int			count;		//アニメーションフレーム数

};

//********************************************************
//プロトタイプ宣言
//********************************************************
class Torch
{
public:
	Torch();
	~Torch();
	void UpdateTorch(void);
	void DrawTorch(void);

	int StartTorch(XMFLOAT3 pos, XMFLOAT2 size);
private:

};


//HRESULT InitTorch(void);
//void UninitTorch(void);

