//===================================================
//	動かない床[DWBox.h]
//  武井遥都
//=====================================================
#pragma once
#include "main.h"
#include "mesh.h"
#include "AssimpModel.h"
#include "box.h"

//*****マクロ定義*****
#define MAX_DWBOX		(100)

class DWBox  : public  Box{
public:
	DWBox();
	~DWBox();
	void Update();
	void Draw();
	void Draw(int num);

	int Create(XMFLOAT3 pos, int nCat, int nTime);

	void Release(int num);

	TBox* GetBox();
	XMFLOAT3 GetPos(int num);
	XMFLOAT2 GetSize(int num);

	bool GetState(int num);
	bool Collision(XMFLOAT2 pos, XMFLOAT2 size);

	void DrawOldNow(int nTime);
	int CreateOldNow(XMFLOAT3 pos, int nTime/*,XMFLOAT3 scl*/);

private:
	CAssimpModel	m_model;	//モデル
	XMFLOAT4X4		m_mtxWorld;		// ワールドマトリックス
	TBox			m_box[MAX_DWBOX];		//動かない床の情報

	MATERIAL m_material;
};
