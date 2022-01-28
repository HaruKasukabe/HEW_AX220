//=============================================================================
//
// プレイヤー女の子 [playerGirl.h]
// 小楠裕子
//=============================================================================
#pragma once
#include "main.h"
#include "AssimpModel.h"
#include "map.h"

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

	void SetAnim(int);
	void CollisionSide(std::vector<OBJECT_INFO>::iterator);
private:
	CAssimpModel	m_model;	//モデル
	CAssimpModel	m_modelSub[3];	//モデル(アニメーション用)
	XMFLOAT3		m_pos;		// 現在の位置
	XMFLOAT3		m_rot;		// 現在の向き
	XMFLOAT3		m_rotDest;	// 目的の向き
	XMFLOAT3		m_move;		// 移動量
	XMFLOAT3		m_scl;		//オブジェクトのスケール
	bool			m_bLand;	// 着地・接地フラグ
	bool			m_bJump;	// ジャンプ可能

	int				m_nShadow;	// 丸影番号

	XMFLOAT4X4		m_mtxWorld;		// ワールドマトリックス

	int				m_nCube;		//立方体番号

	int				m_nAnim;		// アニメーション
	int				m_nAnimNow;		// 現在のアニメーション
	int				m_nAnimTime;	// アニメーションの時間
};
