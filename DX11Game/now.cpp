//===================================================
//		現在[now.cpp]
//小楠裕子
//=====================================================
#include "now.h"
#include "map.h"

//=============================
//		ｺﾝｽﾄﾗｸﾀ
//=============================
Now::Now() {
	//ﾌｨｰﾙﾄﾞ初期化
	m_pMeshField = new MeshField(8, 1, 80.0f, 80.0f, 1.0f, 1.0f, XMFLOAT3(0.0f,-50.0f,0.0f));
	//女の子初期化
	m_pPlayerGirl = new Player_Girl;
}

//=============================
//		ﾃﾞｽﾄﾗｸﾀ
//=============================
Now::~Now(){
	//ﾌｨｰﾙﾄﾞ終了
	delete m_pMeshField;
	//女の子終了
	delete m_pPlayerGirl;
}

//=============================
//		更新
//=============================
void Now::Update(){
	//ﾌｨｰﾙﾄﾞ更新
	m_pMeshField->Update();
	//女の子更新
	m_pPlayerGirl->Update();
}

//=============================
//		描画
//=============================
void Now::Draw(){
	// Zバッファ無効(Zチェック無&Z更新無)
	SetZBuffer(false);



	// Zバッファ有効(Zチェック有&Z更新有)
	SetZBuffer(true);
	//ﾌｨｰﾙﾄﾞ描画
	m_pMeshField->Draw();
	//女の子描画
	m_pPlayerGirl->Draw();
	//マップ描画
	DrawNowMap();
}

//=============================
//		男の子取得
//=============================
Player_Girl* Now::GetPlayerGirl() {
	return m_pPlayerGirl;
}