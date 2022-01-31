//========================================================
//
//爆発処理[explosion.h]
//
//========================================================
#pragma once

#include "main.h"

struct TBreak
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
class Break
{
public:
	Break();
	~Break();
	void UpdateBreak(void);
	void DrawBreak(void);

	int StartBreak(XMFLOAT3 pos, XMFLOAT2 size);
private:
	
};


//HRESULT InitBreak(void);
//void UninitBreak(void);

