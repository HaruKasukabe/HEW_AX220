//=============================================================================
//
// 木処理 [tree.h]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#pragma once
#include "main.h"
#include "mesh.h"
#include "AssimpModel.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MAX_TREE				(256)	// 木最大数

//*****************************************************************************
// 構造体定義
//*****************************************************************************
struct TTree {
	bool		m_use;		// 使用しているかどうか
	XMFLOAT3	m_pos;		// 位置
	XMFLOAT4X4  m_mtxWorld; // ワールドマトリックス
	XMFLOAT3    m_scl;      // 木のスケール
	XMFLOAT2    m_collision;// 当たり判定
	int         m_nTime;    // 過去か未来か
};

//*****************************************************************************
// クラス定義
//*****************************************************************************
class Tree{
public:
	Tree();
	~Tree();
	void Update();
	void Draw();
	void Draw(int num);
	TTree* GetTree();
	void DrawOldNow(int nTime);
	int CreateOldNow(XMFLOAT3 pos, int nTime);

private:
	CAssimpModel m_model;          // モデル
	CAssimpModel m_sModel;
	XMFLOAT4X4   m_mtxWorld;       // ワールドマトリックス
	TTree        m_tree[MAX_TREE]; // 木の情報

	MATERIAL    m_material;
	MATERIAL    m_seedMaterial;

};