//===================================================
//		箱[box.h]
//小楠裕子
//=====================================================
#pragma once
#include "main.h"
#include "mesh.h"
#include "AssimpModel.h"


//*****************************************************************************
// 構造体
//*****************************************************************************
struct TBox {
	bool		m_use;		// 使用しているかどうか
	XMFLOAT3	m_pos;		// 中心座標(モデル座標系)
	XMFLOAT3    m_oldPos;   // 未来でのボックスを反映させる前の座標(箱を持っているときに使用)
	bool		m_state;	//状態	true:壊されてない,false:壊されている
	XMFLOAT4X4	m_mtxWorld;	// ワールドマトリックス
	int			m_nTime;	// 今か過去か
	int			m_nCat;		// 箱の種類
	XMFLOAT3    m_scl;      // 箱のスケール
	XMFLOAT2    m_collision;// 当たり判定の大きさ
};

//*****マクロ定義*****
#define MAX_BOX		(100)

class Box {
public:
	Box();
	~Box();
	void Update();
	void Draw();
	void Draw(int num);

	int Create(XMFLOAT3 pos, int nCat);

	void Release(int num);

	bool Destroy(int num);

	TBox* GetBox();
	XMFLOAT3 GetPos(int num);
	void SetBoxPos(int num, XMFLOAT3 pos,int time);
	void SetOldBoxPos(int num);
	XMFLOAT2 GetSize();

	bool GetState(int num);

	void DrawOldNow(int nTime);
	int CreateOldNow(XMFLOAT3 pos, int nTime);

private:
	CAssimpModel	m_model;	//モデル
	XMFLOAT4X4		m_mtxWorld;		// ワールドマトリックス
	TBox			m_box[MAX_BOX];		//箱の情報
	XMFLOAT3		m_scl;		//オブジェクトのスケール

	MATERIAL m_material;
};
