//===================================================
//		過去[old.cpp]
//小楠裕子
//=====================================================
#include "old.h"
#include "map.h"
#include "bsphere.h"
#include "shadow.h"


//=============================
//		ｺﾝｽﾄﾗｸﾀ
//=============================
Old::Old() {
	// 丸影初期化
	InitShadow();
	//ﾌｨｰﾙﾄﾞ初期化
	m_pMeshField = new MeshField(8, 1, 80.0f, 80.0f, 1.0f, 1.0f, XMFLOAT3(0.0f,-50.0f,0.0f));
	//男の子初期化
	m_pPlayerBoy = new Player_Boy;
	//境界球初期化
	InitBSphere();
}

//=============================
//		ﾃﾞｽﾄﾗｸﾀ
//=============================
Old::~Old(){
	//ﾌｨｰﾙﾄﾞ終了
	delete m_pMeshField;
	//男の子終了
	delete m_pPlayerBoy;
	//境界球終了
	UninitBSphere();
	// 丸影終了処理
	UninitShadow();
}

//=============================
//		更新
//=============================
void Old::Update(){
	//ﾌｨｰﾙﾄﾞ更新
	m_pMeshField->Update();
	//男の子更新
	m_pPlayerBoy->Update();
	//境界球更新
	UpdateBSphere();
}

//=============================
//		描画
//=============================
void Old::Draw(){
	// Zバッファ無効(Zチェック無&Z更新無)
	SetZBuffer(false);
	// 丸影描画
	DrawShadow();


	// Zバッファ有効(Zチェック有&Z更新有)
	SetZBuffer(true);
	//ﾌｨｰﾙﾄﾞ描画
	m_pMeshField->Draw();
	//男の子描画
	m_pPlayerBoy->Draw();
	//マップ描画
	DrawOldMap();
	//境界球描画
	DrawBSphere();
	// 丸影描画
	DrawShadow();
}


//=============================
//		男の子取得
//=============================
Player_Boy* Old::GetPlayerBoy() {
	return m_pPlayerBoy;
}

// プレイヤーの座標を獲得
XMFLOAT3 Old::GetBoyPos()
{
	return m_pPlayerBoy->GetBoyPos();
}