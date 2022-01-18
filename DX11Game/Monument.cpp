//=============================================================================
//
// ゴールの石碑[Monument.cpp]
// Author : 武井 遥都
//
//=============================================================================
#include "Monument.h"
#include "Texture.h"
#include "Shader.h"

//*********************************************************
//マクロ定義
//*********************************************************
#define BOX_MODEL_PATH	"data/model/sekihi.fbx"
#define BOX_TEXTURE_PATH "data/texture/sekihi_color.jpg"

#define M_DIFFUSE			XMFLOAT4(1.0f,1.0f,1.0f,1.0f)
#define M_SPECULAR			XMFLOAT4(0.0f,0.0f,0.0f,1.0f)
#define M_POWER				(50.0f)
#define M_AMBIENT			XMFLOAT4(1.0f,1.0f,1.0f,1.0f)
#define M_EMISSIVE			XMFLOAT4(0.0f,0.0f,0.0f,1.0f)

//*********************************************************
//グローバル変数
//*********************************************************
MESH g_monumentMesh;

//==========================
// コンストラクタ
//==========================
Monument::Monument(){

	ID3D11Device* pDevice = GetDevice();
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	for (int i = 0; i < MAX_MONUMENT; ++i) {
		m_monument[i].m_pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_monument[i].m_use = false;
		m_monument[i].m_scl = XMFLOAT3(1.5f, 2.0f, 1.0f);
		m_monument[i].m_nTime = -1;
	}

	g_monumentMesh.pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_monumentMesh.rot = XMFLOAT3(0.0f, 0.0f, 0.0f);

	// マテリアルの初期設定
	m_material.Diffuse = M_DIFFUSE;
	m_material.Ambient = M_AMBIENT;
	m_material.Specular = M_SPECULAR;
	m_material.Power = M_POWER;
	m_material.Emissive = M_EMISSIVE;
	g_monumentMesh.pMaterial = &m_material;

	// モデルデータの読み込み
	if (!m_model.Load(pDevice, pDeviceContext, BOX_MODEL_PATH)) {
		MessageBoxA(GetMainWnd(), "モデルデータ読み込みエラー", "モニュメントModel", MB_OK);
	}
	// テクスチャの読み込み
	static TAssimpMaterial material;
	HRESULT hr = CreateTextureFromFile(pDevice, BOX_TEXTURE_PATH, &material.pTexture);
	if (FAILED(hr))
	{
		MessageBoxA(GetMainWnd(), "テクスチャ読み込みエラー", "モニュメントのテクスチャ", MB_OK);
	}
	m_model.SetMaterial(&material);


	XMStoreFloat4x4(&g_monumentMesh.mtxTexture, XMMatrixIdentity());
}

//=============================
//		ﾃﾞｽﾄﾗｸﾀ
//=============================
Monument::~Monument() {
	// モデルの解放
	m_model.Release();
	ReleaseMesh(&g_monumentMesh);
}
//=============================
//		更新
//=============================
void Monument::Update() {
	XMMATRIX mtxWorld, mtxScl, mtxTranslate;
	// メッシュ更新
	UpdateMesh(&g_monumentMesh);

	for (int i = 0; i < MAX_MONUMENT; ++i)
	{
		//未使用ならスキップ
		if (!m_monument[i].m_use) {
			continue;
		}
		// ワールドマトリックスの初期化
		mtxWorld = XMMatrixIdentity();

		mtxScl = XMMatrixScaling(m_monument[i].m_scl.x, m_monument[i].m_scl.y, m_monument[i].m_scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// 移動を反映
		mtxTranslate = XMMatrixTranslation(m_monument[i].m_pos.x, m_monument[i].m_pos.y, m_monument[i].m_pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ワールドマトリックス設定
		XMStoreFloat4x4(&m_monument[i].m_mtxWorld, mtxWorld);


	}

}

//===============================================
//		描画(0が今、1が過去)
//===============================================
void Monument::Draw(int nTime) {
	ID3D11DeviceContext* pDC = GetDeviceContext();

	for (int i = 0; i < MAX_MONUMENT; ++i)
	{
		if (!m_monument[i].m_use) {
			continue;
		}
		if (!m_monument[i].m_nTime == nTime)
		{
			continue;
		}
		// 不透明部分を描画
		m_model.Draw(pDC, m_monument[i].m_mtxWorld, eOpacityOnly);

		// 半透明部分を描画
		SetBlendState(BS_ALPHABLEND);	// アルファブレンド有効
		SetZWrite(false);				// Zバッファ更新しない
		m_model.Draw(pDC, m_monument[i].m_mtxWorld, eTransparentOnly);
		SetZWrite(true);				// Zバッファ更新する
		SetBlendState(BS_NONE);			// アルファブレンド無効
	}
}

//=======================================
//	モニュメント生成(0が今、1が過去)
//=======================================
int Monument::Create(XMFLOAT3 pos, int nTime) {
	Sekihi* pMonument = m_monument;
	for (int i = 0; i < MAX_MONUMENT; ++i, ++pMonument) {
		if (pMonument->m_use) continue;
		pMonument->m_pos = pos;
		pMonument->m_use = true;
		pMonument->m_nTime = nTime;
		return i;
	}
	return -1;
}

//================================
//	ﾓﾆｭﾒﾝﾄ解放	引数 :キューブ番号
//================================
void Monument::Release(int num){
	if (num < 0 || num >= MAX_MONUMENT) {
		return;
	}
	m_monument[num].m_use = false;
}