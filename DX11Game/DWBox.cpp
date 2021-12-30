//===================================================
//		箱[box.cpp]
//小楠裕子
//=====================================================
#include "DWBox.h"
#include "Texture.h"
#include "Shader.h"
#include "bsphere.h"
#include "sceneGame.h"
#include "map.h"

//*********************************************************
//マクロ定義
//*********************************************************
#define DWBOX_MODEL_PATH	"data/model/box001.x"
#define DWBOX_TEXTURE_PATH "data/texture/glass.png"

#define M_DIFFUSE			XMFLOAT4(1.0f,1.0f,1.0f,1.0f)
#define M_SPECULAR			XMFLOAT4(0.0f,0.0f,0.0f,1.0f)
#define M_POWER				(50.0f)
#define M_AMBIENT			XMFLOAT4(1.0f,1.0f,1.0f,1.0f)
#define M_EMISSIVE			XMFLOAT4(0.0f,0.0f,0.0f,1.0f)

#define DWBOX_COLLISION_SIZE_X	4.0f // sclが1のときの設定
#define DWBOX_COLLISION_SIZE_Y	4.0f // sclが1のときの設定

//*********************************************************
//グローバル変数
//*********************************************************
MESH g_dwboxMesh;

//=============================
//		ｺﾝｽﾄﾗｸﾀ
//=============================
DWBox::DWBox() {

	ID3D11Device* pDevice = GetDevice();
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	// 位置・回転・スケールの初期設定
	for (int i = 0; i < MAX_BOX; ++i) {
		m_box[i].m_pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_box[i].m_oldPos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_box[i].m_state = true;
		m_box[i].m_use = false;
		m_box[i].m_scl = XMFLOAT3(1.0f, 1.0f, 1.0f);//デフォルト設定
		m_box[i].m_collision = XMFLOAT2(DWBOX_COLLISION_SIZE_X, DWBOX_COLLISION_SIZE_Y);
	}

	g_dwboxMesh.pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_dwboxMesh.rot = XMFLOAT3(0.0f, 0.0f, 0.0f);

	// マテリアルの初期設定
	m_material.Diffuse = M_DIFFUSE;
	m_material.Ambient = M_AMBIENT;
	m_material.Specular = M_SPECULAR;
	m_material.Power = M_POWER;
	m_material.Emissive = M_EMISSIVE;
	g_dwboxMesh.pMaterial = &m_material;

	// モデルデータの読み込み
	if (!m_model.Load(pDevice, pDeviceContext, DWBOX_MODEL_PATH)) {
		MessageBoxA(GetMainWnd(), "モデルデータ読み込みエラー", "InitModel", MB_OK);
	}
	// テクスチャの読み込み
	static TAssimpMaterial material;
	HRESULT hr = CreateTextureFromFile(pDevice, DWBOX_TEXTURE_PATH, &material.pTexture);
	if (FAILED(hr))
	{
		MessageBoxA(GetMainWnd(), "テクスチャ読み込みエラー", "草ァ！のテクスチャ", MB_OK);
	}
	m_model.SetMaterial(&material);


	XMStoreFloat4x4(&g_dwboxMesh.mtxTexture, XMMatrixIdentity());

}

//=============================
//		ﾃﾞｽﾄﾗｸﾀ
//=============================
DWBox::~DWBox() {
	// モデルの解放
	m_model.Release();
	ReleaseMesh(&g_dwboxMesh);
}
//=============================
//		更新
//=============================
void DWBox::Update() {
	XMMATRIX mtxWorld, mtxScl, mtxTranslate;
	// メッシュ更新
	UpdateMesh(&g_dwboxMesh);

	for (int i = 0; i < MAX_BOX; ++i)
	{
		//未使用ならスキップ
		if (!m_box[i].m_use) {
			continue;
		}
		// ワールドマトリックスの初期化
		mtxWorld = XMMatrixIdentity();

		mtxScl = XMMatrixScaling(m_box[i].m_scl.x, m_box[i].m_scl.y, m_box[i].m_scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// 移動を反映
		mtxTranslate = XMMatrixTranslation(m_box[i].m_pos.x, m_box[i].m_pos.y, m_box[i].m_pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ワールドマトリックス設定
		XMStoreFloat4x4(&m_box[i].m_mtxWorld, mtxWorld);


	}

}

//=============================
//		描画
//=============================
void DWBox::Draw() {
	ID3D11DeviceContext* pDC = GetDeviceContext();

	for (int i = 0; i < MAX_BOX; ++i)
	{
		//未使用ならスキップ
		if (!m_box[i].m_use) {
			continue;
		}

		// 不透明部分を描画
		m_model.Draw(pDC, m_box[i].m_mtxWorld, eOpacityOnly);
		DrawMesh(pDC, &g_dwboxMesh);
		// 半透明部分を描画
		SetBlendState(BS_ALPHABLEND);	// アルファブレンド有効
		SetZWrite(false);				// Zバッファ更新しない
		m_model.Draw(pDC, m_box[i].m_mtxWorld, eTransparentOnly);
		SetZWrite(true);				// Zバッファ更新する
		SetBlendState(BS_NONE);			// アルファブレンド無効
	}
}

//=============================
//		描画
//=============================
void DWBox::Draw(int num) {
	ID3D11DeviceContext* pDC = GetDeviceContext();
	//未使用なら描画しない
	if (!m_box[num].m_use) {
		return;
	}
	//破壊されていたら描画しない
	if (!m_box[num].m_state) {
		return;
	}
	// 不透明部分を描画
	m_model.Draw(pDC, m_box[num].m_mtxWorld, eOpacityOnly);

	// 半透明部分を描画
	SetBlendState(BS_ALPHABLEND);	// アルファブレンド有効
	SetZWrite(false);				// Zバッファ更新しない
	m_model.Draw(pDC, m_box[num].m_mtxWorld, eTransparentOnly);
	DrawMesh(pDC, &g_dwboxMesh);
	SetZWrite(true);				// Zバッファ更新する
	SetBlendState(BS_NONE);			// アルファブレンド無効
}

//=============================
//	箱生成 引数 : モデル座標、サイズ、ワールドマトリックス
//=============================
int DWBox::Create(XMFLOAT3 pos, int nCat) {
	TBox* pDWBox = m_box;
	for (int i = 0; i < MAX_BOX; ++i, ++pDWBox) {
		if (pDWBox->m_use) continue;
		pDWBox->m_pos = pos;
		pDWBox->m_oldPos = pos;
		pDWBox->m_state = true;
		pDWBox->m_use = true;
		pDWBox->m_nCat = nCat;

		return i;
	}
	return -1;
}

//=============================
//	箱解放	引数 :キューブ番号
//=============================
void DWBox::Release(int num) {
	if (num < 0 || num >= MAX_BOX)
		return;
	m_box[num].m_use = false;
}

//=============================
//	箱の情報　取得
//=============================
TBox* DWBox::GetBox()
{
	return m_box;
}

//=============================
//	箱　座標取得
//=============================
XMFLOAT3 DWBox::GetPos(int num) {
	return m_box[num].m_pos;
}

////=============================
////	箱　座標設定
////=============================
//void DWBox::SetDWBoxPos(int num, XMFLOAT3 pos, int time) {
//	XMFLOAT3 boyPos = GetOld()->GetBoyPos();
//	if (!m_box[num].m_nCat == CARRY)
//		return;
//
//	// 過去用
//	if (time == 0) {
//		if (pos.x > 0.0f)
//			m_box[num].m_pos.x = boyPos.x + BOY_HUND_LONG;
//		else if (pos.x < 0.0f)
//			m_box[num].m_pos.x = boyPos.x - BOY_HUND_LONG;
//
//		if (!(boyPos.y - m_box[num].m_pos.y >= BOY_HUND_LONG || boyPos.y - m_box[num].m_pos.y <= -BOY_HUND_LONG))
//			m_box[num].m_pos.y += pos.y;
//		if (!(boyPos.z - m_box[num].m_pos.z >= BOY_HUND_LONG || boyPos.y - m_box[num].m_pos.z <= -BOY_HUND_LONG))
//			m_box[num].m_pos.z += pos.z;
//	}
//	// 未来用
//	if (time == 1) {
//		if (pos.x > 0.0f)
//			m_box[num].m_oldPos.x = boyPos.x + BOY_HUND_LONG;
//		else if (pos.x < 0.0f)
//			m_box[num].m_oldPos.x = boyPos.x - BOY_HUND_LONG;
//
//		if (!(boyPos.y - m_box[num].m_oldPos.y >= BOY_HUND_LONG || boyPos.y - m_box[num].m_oldPos.y <= -BOY_HUND_LONG))
//			m_box[num].m_oldPos.y += pos.y;
//		if (!(boyPos.z - m_box[num].m_oldPos.z >= BOY_HUND_LONG || boyPos.y - m_box[num].m_oldPos.z <= -BOY_HUND_LONG))
//			m_box[num].m_oldPos.z += pos.z;
//	}
//#ifndef TAKEI_HARUTO
//	PrintDebugProc("ﾎｿﾞﾝｻﾞﾋｮｳx:%2f,y:%2f,z:%2f\n", m_box[num].m_pos.x, m_box[num].m_pos.y, m_box[num].m_pos.z);
//	PrintDebugProc("ﾊﾝｴｲｻﾞﾋｮｳx:%2f,y:%2f,z:%2f\n",
//		m_box[num].m_oldPos.x, m_box[num].m_oldPos.y, m_box[num].m_oldPos.z);
//#endif
//}

//=============================
//	箱　サイズ取得
//=============================
XMFLOAT2 DWBox::GetSize(int num) {
	return m_box[num].m_collision;
}
//=============================
//	箱　状態取得
//=============================
bool DWBox::GetState(int num) {
	return m_box[num].m_state;
}

//===============================================
//		今と過去を分けれる描画(0が今、1が過去)
//===============================================
void DWBox::DrawOldNow(int nTime) {
	ID3D11DeviceContext* pDC = GetDeviceContext();

	for (int i = 0; i < MAX_BOX; ++i)
	{
		if (!m_box[i].m_use) {
			continue;
		}
		if (!m_box[i].m_nTime == nTime)
		{
			continue;
		}
		// 不透明部分を描画
		m_model.Draw(pDC, m_box[i].m_mtxWorld, eOpacityOnly);

		// 半透明部分を描画
		SetBlendState(BS_ALPHABLEND);	// アルファブレンド有効
		SetZWrite(false);				// Zバッファ更新しない
		m_model.Draw(pDC, m_box[i].m_mtxWorld, eTransparentOnly);
		SetZWrite(true);				// Zバッファ更新する
		SetBlendState(BS_NONE);			// アルファブレンド無効
	}
}

//=======================================
//	キューブ生成(0が今、1が過去)
//=======================================
int DWBox::CreateOldNow(XMFLOAT3 pos, int nTime/*, XMFLOAT3 scl*/) {
	TBox* pDWBox = m_box;
	for (int i = 0; i < MAX_BOX; ++i, ++pDWBox) {
		if (pDWBox->m_use) continue;
		pDWBox->m_pos = pos;
		pDWBox->m_oldPos = pos;
		pDWBox->m_state = true;
		pDWBox->m_use = true;
		pDWBox->m_nTime = nTime;
		// pDWBox->m_scl = scl; スケールの設定
		pDWBox->m_collision = XMFLOAT2(DWBOX_COLLISION_SIZE_X*pDWBox->m_scl.x, DWBOX_COLLISION_SIZE_Y*pDWBox->m_scl.y);
		return i;
	}
	return -1;
}