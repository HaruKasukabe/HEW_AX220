//=============================================================================
//
// プレイヤー男の子 [playerBoy.h]
// 小楠裕子
//=============================================================================
#pragma once
#include "main.h"
#include "AssimpModel.h"

class Player_Boy {
public:
	Player_Boy();
	~Player_Boy();
	void Update();
	void Draw();
	XMFLOAT3 GetBoyPos();
	XMFLOAT3 GetBoyMove();
	bool CheckField();
private:
	CAssimpModel	m_model;	//モデル
	XMFLOAT3		m_pos;		// 現在の位置
	XMFLOAT3		m_rot;		// 現在の向き
	XMFLOAT3		m_rotDest;	// 目的の向き
	XMFLOAT3		m_move;		// 移動量
	int				m_dir;		//
	bool			m_bJump;	// ジャンプ中フラグ
	bool			m_bLand;	// 着地・接地フラグ

	XMFLOAT4X4		m_mtxWorld;		// ワールドマトリックス

	int				m_nSphere;		//境界球番号

	int				m_nHund;	// 持ち物
};
