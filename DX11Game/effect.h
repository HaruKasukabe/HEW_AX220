//========================================================
//
//爆発処理[explosion.h]
//
//========================================================
#pragma once

#include "main.h"

struct TEffect
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
class Effect
{
public:
	Effect();
	~Effect();
	void UpdateEffect(void);
	void DrawEffect(void);

	int StartEffect(XMFLOAT3 pos, XMFLOAT2 size);
private:

};


//HRESULT InitEffect(void);
//void UninitEffect(void);

