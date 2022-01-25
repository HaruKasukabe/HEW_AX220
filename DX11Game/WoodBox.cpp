//===================================================
//		木箱[WoodBox.cpp]
//加藤匠
//=====================================================
#include "WoodBox.h"
#include "Texture.h"
#include "Shader.h"
#include "bsphere.h"
#include "sceneGame.h"
#include "map.h"

//*********************************************************
//マクロ定義
//*********************************************************
#define WOOD_BOX_MODEL_PATH			"data/model/WoodBox.fbx"
#define WOOD_BOX_NOW_TEXTURE_PATH	"data/texture/木箱.jpg"
#define WOOD_BOX_OLD_TEXTURE_PATH	"data/texture/木箱.jpg"

#define MAX_WOOD_BOX_TEXTURE		(2)

#define M_DIFFUSE			XMFLOAT4(1.0f,1.0f,1.0f,1.0f)
#define M_SPECULAR			XMFLOAT4(0.0f,0.0f,0.0f,1.0f)
#define M_POWER				(50.0f)
#define M_AMBIENT			XMFLOAT4(1.0f,1.0f,1.0f,1.0f)
#define M_EMISSIVE			XMFLOAT4(0.0f,0.0f,0.0f,1.0f)

#define WOOD_BOX_COLLISION_SIZE_X	4.0f
#define WOOD_BOX_COLLISION_SIZE_Y	10.0f
#define WOOD_BOX_GRAVITY				0.15f

#define BOY_HUND_LONG			10.0f

//*********************************************************
//構造体定義
//*********************************************************
const char* g_woodBoxTexture[MAX_WOOD_BOX_TEXTURE]
{
	WOOD_BOX_NOW_TEXTURE_PATH,
	WOOD_BOX_OLD_TEXTURE_PATH,
};

//*********************************************************
//グローバル変数
//*********************************************************
MESH g_woodBoxMesh;
static TAssimpMaterial g_woodBoxMaterial[MAX_WOOD_BOX_TEXTURE];

//=============================
//		ｺﾝｽﾄﾗｸﾀ
//=============================
WoodBox::WoodBox(){

	ID3D11Device* pDevice = GetDevice();
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	// 位置・回転・スケールの初期設定
	for (int i = 0; i < MAX_WOOD_BOX; ++i) {
		m_woodBox[i].m_pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_woodBox[i].m_oldPos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_woodBox[i].m_state = true;
		m_woodBox[i].m_use = false;
	}

	g_woodBoxMesh.pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_woodBoxMesh.rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_scl = XMFLOAT3(9.0f, 36.0f, 6.0f);

	// マテリアルの初期設定
	m_material.Diffuse = M_DIFFUSE;
	m_material.Ambient = M_AMBIENT;
	m_material.Specular = M_SPECULAR;
	m_material.Power = M_POWER;
	m_material.Emissive = M_EMISSIVE;
	g_woodBoxMesh.pMaterial = &m_material;

	// モデルデータの読み込み
	if (!m_model.Load(pDevice, pDeviceContext, WOOD_BOX_MODEL_PATH)) {
		MessageBoxA(GetMainWnd(), "モデルデータ読み込みエラー", "InitModel", MB_OK);
	}

	// テクスチャの読み込み
	HRESULT hr;
	for (int i = 0; i < MAX_WOOD_BOX_TEXTURE; i++)
	{
		hr = CreateTextureFromFile(pDevice, g_woodBoxTexture[i], &g_woodBoxMaterial[i].pTexture);
		if (FAILED(hr))
		{
			MessageBoxA(GetMainWnd(), "テクスチャ読み込みエラー", "木箱のテクスチャ", MB_OK);
		}
	}
	m_model.SetMaterial(&g_woodBoxMaterial[0]);


	XMStoreFloat4x4(&g_woodBoxMesh.mtxTexture, XMMatrixIdentity());

}

//=============================
//		ﾃﾞｽﾄﾗｸﾀ
//=============================
WoodBox::~WoodBox() {
	// モデルの解放
	m_model.Release();
	ReleaseMesh(&g_woodBoxMesh);
}
//=============================
//		更新
//=============================
void WoodBox::Update() {
	XMMATRIX mtxWorld,mtxScl,mtxTranslate;
	// メッシュ更新
	UpdateMesh(&g_woodBoxMesh);

	for (int i = 0; i < MAX_WOOD_BOX; ++i) 
	{
		//未使用ならスキップ
		if (!m_woodBox[i].m_use) {
			continue;
		}
		// ワールドマトリックスの初期化
		mtxWorld = XMMatrixIdentity();

		mtxScl = XMMatrixScaling(m_scl.x, m_scl.y, m_scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// 移動を反映
		mtxTranslate = XMMatrixTranslation(m_woodBox[i].m_pos.x, m_woodBox[i].m_pos.y, m_woodBox[i].m_pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ワールドマトリックス設定
		XMStoreFloat4x4(&m_woodBox[i].m_mtxWorld, mtxWorld);


	}
	
}

//=============================
//		描画
//=============================
void WoodBox::Draw() {
	ID3D11DeviceContext* pDC = GetDeviceContext();

	for (int i = 0; i < MAX_WOOD_BOX; ++i)
	{
		//未使用ならスキップ
		if (!m_woodBox[i].m_use) {
			continue;
		}

		// 不透明部分を描画
		m_model.Draw(pDC, m_woodBox[i].m_mtxWorld, eOpacityOnly);
		DrawMesh(pDC, &g_woodBoxMesh);
		// 半透明部分を描画
		SetBlendState(BS_ALPHABLEND);	// アルファブレンド有効
		SetZWrite(false);				// Zバッファ更新しない
		m_model.Draw(pDC, m_woodBox[i].m_mtxWorld, eTransparentOnly);
		SetZWrite(true);				// Zバッファ更新する
		SetBlendState(BS_NONE);			// アルファブレンド無効
	}
}

//=============================
//		描画
//=============================
void WoodBox::Draw(int num) {
	ID3D11DeviceContext* pDC = GetDeviceContext();
		//未使用なら描画しない
		if (!m_woodBox[num].m_use) {
			return;
		}
		//破壊されていたら描画しない
		if (!m_woodBox[num].m_state) {
			return;
		}

		//時間に応じてテクスチャを変更
		if (m_woodBox[num].m_nTime == 0)
			m_model.SetMaterial(&g_woodBoxMaterial[0]);
		else
			m_model.SetMaterial(&g_woodBoxMaterial[1]);

		// 不透明部分を描画
		m_model.Draw(pDC, m_woodBox[num].m_mtxWorld, eOpacityOnly);

		// 半透明部分を描画
		SetBlendState(BS_ALPHABLEND);	// アルファブレンド有効
		SetZWrite(false);				// Zバッファ更新しない
		m_model.Draw(pDC, m_woodBox[num].m_mtxWorld, eTransparentOnly);
		DrawMesh(pDC, &g_woodBoxMesh);
		SetZWrite(true);				// Zバッファ更新する
		SetBlendState(BS_NONE);			// アルファブレンド無効
}

//=============================
//	箱生成 引数 : モデル座標、サイズ、ワールドマトリックス
//=============================
int WoodBox::Create(XMFLOAT3 pos, int nCat, int nTime) {
	TBox* pWoodBox = m_woodBox;
	for (int i = 0; i < MAX_WOOD_BOX; ++i, ++pWoodBox) {
		if (pWoodBox->m_use) continue;
		pWoodBox->m_pos = pos;
		pWoodBox->m_oldPos = pos;
		pWoodBox->m_state = true;
		pWoodBox->m_use = true;
		pWoodBox->m_nCat = nCat;
		pWoodBox->m_nTime = nTime;

		return i;
	}
	return -1;
}

//=============================
//	箱解放	引数 :キューブ番号
//=============================
void WoodBox::Release(int num) {
	if (num < 0 || num >= MAX_WOOD_BOX)
		return;
	m_woodBox[num].m_use = false;
}

//=============================
//	箱　破壊
//=============================
bool WoodBox::Destroy(int num) {
	if (num < 0 || num >= MAX_WOOD_BOX)
		return false;
	if (!m_woodBox[num].m_nCat == CARRY)
		return false;
	m_woodBox[num].m_state = false;
	return true;
}

//=============================
//	箱の情報　取得
//=============================
TBox* WoodBox::GetWoodBox()
{
	return m_woodBox;
}

//=============================
//	箱　座標取得
//=============================
XMFLOAT3 WoodBox::GetPos(int num) {
	return m_woodBox[num].m_pos;
}

//=============================
//	箱　座標設定
//=============================
void WoodBox::SetWoodBoxPos(int num, XMFLOAT3 pos, XMFLOAT3 move, int time) {
	XMFLOAT3 boyPos = GetOld()->GetBoyPos();
	if (!(m_woodBox[num].m_nCat == CARRY))
		return;

	// 過去用
	if (time == 0){
		if (move.x > 0.0f)
			m_woodBox[num].m_pos.x = boyPos.x + BOY_HUND_LONG;
		else if (move.x < 0.0f)
			m_woodBox[num].m_pos.x = boyPos.x - BOY_HUND_LONG;

		if (!(boyPos.y - m_woodBox[num].m_pos.y >= BOY_HUND_LONG || boyPos.y - m_woodBox[num].m_pos.y <= -BOY_HUND_LONG))
			m_woodBox[num].m_pos.y += move.y;
		if (m_woodBox[num].m_pos.y > pos.y + 20.0f)
			m_woodBox[num].m_pos.y = pos.y + 20.0f;
		if (!(boyPos.z - m_woodBox[num].m_pos.z >= BOY_HUND_LONG || boyPos.y - m_woodBox[num].m_pos.z <= -BOY_HUND_LONG))
			m_woodBox[num].m_pos.z += move.z;
	}
	// 未来用
	if (time == 1) {
		if (move.x > 0.0f)
			m_woodBox[num].m_oldPos.x = boyPos.x + BOY_HUND_LONG;
		else if (move.x < 0.0f)
			m_woodBox[num].m_oldPos.x = boyPos.x - BOY_HUND_LONG;

		if (!(boyPos.y - m_woodBox[num].m_oldPos.y >= BOY_HUND_LONG || boyPos.y - m_woodBox[num].m_oldPos.y <= -BOY_HUND_LONG))
			m_woodBox[num].m_oldPos.y += move.y;
		if (!(boyPos.z - m_woodBox[num].m_oldPos.z >= BOY_HUND_LONG || boyPos.y - m_woodBox[num].m_oldPos.z <= -BOY_HUND_LONG))
			m_woodBox[num].m_oldPos.z += move.z;
	}
#ifndef TAKEI_HARUTO
	PrintDebugProc("ﾎｿﾞﾝｻﾞﾋｮｳx:%2f,y:%2f,z:%2f\n", m_woodBox[num].m_pos.x, m_woodBox[num].m_pos.y, m_woodBox[num].m_pos.z);
	PrintDebugProc("ﾊﾝｴｲｻﾞﾋｮｳx:%2f,y:%2f,z:%2f\n", 
		m_woodBox[num].m_oldPos.x, m_woodBox[num].m_oldPos.y, m_woodBox[num].m_oldPos.z);
#endif
}

//=============================
//	箱　座標反映
//=============================
void WoodBox::SetOldWoodBoxPos(int num) {
	if (!m_woodBox[num].m_nCat == CARRY)	// 今だけNORMALにしてあります(本来ならMOVE)
		return;

	m_woodBox[num].m_pos.x = m_woodBox[num].m_oldPos.x;
	m_woodBox[num].m_pos.y = m_woodBox[num].m_oldPos.y;
	m_woodBox[num].m_pos.z = m_woodBox[num].m_oldPos.z;

}

//=============================
//	箱　サイズ取得
//=============================
XMFLOAT2 WoodBox::GetSize() {
	return XMFLOAT2(WOOD_BOX_COLLISION_SIZE_X, WOOD_BOX_COLLISION_SIZE_Y);
}

//=============================
//	箱　状態取得
//=============================
bool WoodBox::GetState(int num) {
	return m_woodBox[num].m_state;
}

//===============================================
//		今と過去を分けれる描画(0が今、1が過去)
//===============================================
void WoodBox::DrawOldNow(int nTime) {
	ID3D11DeviceContext* pDC = GetDeviceContext();

	for (int i = 0; i < MAX_WOOD_BOX; ++i)
	{
		if (!m_woodBox[i].m_use) {
			continue;
		}
		if (!m_woodBox[i].m_nTime == nTime)
		{
			continue;
		}
		// 不透明部分を描画
		m_model.Draw(pDC, m_woodBox[i].m_mtxWorld, eOpacityOnly);

		// 半透明部分を描画
		SetBlendState(BS_ALPHABLEND);	// アルファブレンド有効
		SetZWrite(false);				// Zバッファ更新しない
		m_model.Draw(pDC, m_woodBox[i].m_mtxWorld, eTransparentOnly);
		SetZWrite(true);				// Zバッファ更新する
		SetBlendState(BS_NONE);			// アルファブレンド無効
	}
}

//=======================================
//	キューブ生成(0が今、1が過去)
//=======================================
int WoodBox::CreateOldNow(XMFLOAT3 pos, int nTime) {
	TBox* pWoodBox = m_woodBox;
	for (int i = 0; i < MAX_WOOD_BOX; ++i, ++pWoodBox) {
		if (pWoodBox->m_use) continue;
		pWoodBox->m_pos = pos;
		pWoodBox->m_oldPos = pos;
		pWoodBox->m_state = true;
		pWoodBox->m_use = true;
		pWoodBox->m_nTime = nTime;
		return i;
	}
	return -1;
}

//=======================================
//	重力設定
//=======================================
void WoodBox::SetGravity(int nObject, int nPat)
{
	if(GetOld()->GetPlayerBoy()->GetBoyHand() != nObject)
		if(!(m_woodBox[nObject].m_pos.y <= -49.0f))
			m_woodBox[nObject].m_pos.y -= WOOD_BOX_GRAVITY;
}