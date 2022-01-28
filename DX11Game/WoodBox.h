//===================================================
//		木箱[WoodBox.h]
//加藤匠
//=====================================================
#pragma once
#include "main.h"
#include "mesh.h"
#include "AssimpModel.h"
#include "box.h"

//*****マクロ定義*****
#define MAX_WOOD_BOX		(100)

class WoodBox : public Box{
public:
	WoodBox();
	~WoodBox();
	void Update();
	void Draw();
	void Draw(int num);

	int Create(XMFLOAT3 pos, int nCat, int nTime);

	void Release(int num);

	bool Destroy(int num);

	TBox* GetWoodBox();
	XMFLOAT3 GetPos(int num);
	void SetWoodBoxPos(int num, XMFLOAT3 pos, XMFLOAT3 move, int time);
	void SetOldWoodBoxPos(int num);
	XMFLOAT2 GetSize();

	bool GetState(int num);

	void DrawOldNow(int nTime);
	int CreateOldNow(XMFLOAT3 pos, int nTime);

	void SetGravity(int, int);

private:
	CAssimpModel	m_model;	//モデル
	XMFLOAT4X4		m_mtxWorld;		// ワールドマトリックス
	TBox			m_woodBox[MAX_WOOD_BOX];		//箱の情報
	XMFLOAT3		m_scl;		//オブジェクトのスケール

	MATERIAL m_material;
};
