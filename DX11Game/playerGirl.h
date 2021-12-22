//=============================================================================
//
// プレイヤー女の子 [playerGirl.h]
// 小楠裕子
//=============================================================================
#pragma once
#include "main.h"
#include "AssimpModel.h"

class Player_Girl {
public:
	Player_Girl();
	~Player_Girl();
	void Update();
	void Draw();

	XMFLOAT3 GetGirlPos();
	XMFLOAT3 GetGirlMove();
	void SetGirlPos(XMFLOAT3);

	bool CheckField();
private:
	CAssimpModel	m_model;	//モデル
	XMFLOAT3		m_pos;		// 現在の位置
	XMFLOAT3		m_rot;		// 現在の向き
	XMFLOAT3		m_rotDest;	// 目的の向き
	XMFLOAT3		m_move;		// 移動量
	bool			m_bLand;	// 着地・接地フラグ

	XMFLOAT4X4		m_mtxWorld;		// ワールドマトリックス

	int				m_nCube;		//立方体番号
};
