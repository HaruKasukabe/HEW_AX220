//=============================================================================
//
// 木処理 [tree.cpp]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#include "tree.h"
#include "Texture.h"
#include "Shader.h"
#include "bsphere.h"
#include "sceneGame.h"
#include "map.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MODEL_TREE      "data/model/Tree.fbx"
#define	TEXTURE_TREE	"data/texture/tree_color.jpg"	// 読み込むテクスチャファイル名
#define MODEL_SEED      "data/model/seed.fbx"
#define TEXTURE_SEED    "data/texture/seed_color.jpg"

#define M_DIFFUSE			XMFLOAT4(1.0f,1.0f,1.0f,1.0f)
#define M_SPECULAR			XMFLOAT4(0.0f,0.0f,0.0f,1.0f)
#define M_POWER				(50.0f)
#define M_AMBIENT			XMFLOAT4(1.0f,1.0f,1.0f,1.0f)
#define M_EMISSIVE			XMFLOAT4(0.0f,0.0f,0.0f,1.0f)

#define TREE_COLLISION_SIZE_X 4.0f
#define TREE_COLLISION_SIZE_Y 4.0f

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static MESH				g_treeMesh;				// メッシュ情報
static MESH             g_seedMesh;

//=============================================================================
// コンストラクタ
//=============================================================================
Tree::Tree()
{
	ID3D11Device* pDevice = GetDevice();
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	// 木の情報初期化
	for (int i = 0; i < MAX_TREE; ++i){
		m_tree[i].m_pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_tree[i].m_use = false;
		m_tree[i].m_scl = XMFLOAT3(0.5f, 1.0f, 0.5f);
		m_tree[i].m_collision = XMFLOAT2(TREE_COLLISION_SIZE_X * m_tree[i].m_scl.x, TREE_COLLISION_SIZE_Y * m_tree[i].m_scl.y);
		m_tree[i].m_nTime = -1;
	}

	g_treeMesh.pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_treeMesh.rot = XMFLOAT3(0.0f, 0.0f, 0.0f);

	g_seedMesh.pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_seedMesh.rot = XMFLOAT3(0.0f, 0.0f, 0.0f);

	// マテリアルの初期設定
	m_material.Diffuse = M_DIFFUSE;
	m_material.Ambient = M_AMBIENT;
	m_material.Specular = M_SPECULAR;
	m_material.Power = M_POWER;
	m_material.Emissive = M_EMISSIVE;
	g_treeMesh.pMaterial = &m_material;

	m_seedMaterial.Diffuse = M_DIFFUSE;
	m_seedMaterial.Ambient = M_AMBIENT;
	m_seedMaterial.Specular = M_SPECULAR;
	m_seedMaterial.Power = M_POWER;
	m_seedMaterial.Emissive = M_EMISSIVE;
	g_treeMesh.pMaterial = &m_seedMaterial;

	// モデルデータの読み込み
	if (!m_model.Load(pDevice, pDeviceContext, MODEL_TREE)) {
		MessageBoxA(GetMainWnd(), "モデルデータ読み込みエラー", "木のモデル", MB_OK);
	}

	// テクスチャの読み込み
	static TAssimpMaterial material;
	HRESULT hr = CreateTextureFromFile(pDevice, TEXTURE_TREE, &material.pTexture);
	if (FAILED(hr)) {
		MessageBoxA(GetMainWnd(), "テクスチャ読み込みエラー", "木のテクスチャ", MB_OK);
	}
	m_model.SetMaterial(&material);

	XMStoreFloat4x4(&g_treeMesh.mtxTexture, XMMatrixIdentity());

	// モデルデータの読み込み
	if (!m_sModel.Load(pDevice, pDeviceContext, MODEL_SEED)) {
		MessageBoxA(GetMainWnd(), "モデルデータ読み込みエラー", "種のモデル", MB_OK);
	}

	// テクスチャの読み込み
	static TAssimpMaterial material_Seed;
	hr = CreateTextureFromFile(pDevice, TEXTURE_SEED, &material_Seed.pTexture);
	if (FAILED(hr)) {
		MessageBoxA(GetMainWnd(), "テクスチャ読み込みエラー", "木のテクスチャ", MB_OK);
	}
	m_sModel.SetMaterial(&material_Seed);

}

//=============================================================================
// デストラクタ
//=============================================================================
Tree::~Tree()
{
	// モデルの開放
	m_model.Release();
	ReleaseMesh(&g_treeMesh);
	m_sModel.Release();
	ReleaseMesh(&g_seedMesh);
}

//=============================================================================
// 更新処理
//=============================================================================
void Tree::Update()
{
	XMMATRIX mtxWorld, mtxScl, mtxTranslate;
	// メッシュ更新
	UpdateMesh(&g_treeMesh);
	UpdateMesh(&g_seedMesh);

	for (int i = 0; i < MAX_TREE; ++i) {
		// 未使用ならスキップ
		if (!m_tree[i].m_use) {
			continue;
		}
		// ワールドマトリックスの初期化
		mtxWorld = XMMatrixIdentity();

		mtxScl = XMMatrixScaling(m_tree[i].m_scl.x, m_tree[i].m_scl.y, m_tree[i].m_scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// 移動を反映
		mtxTranslate = XMMatrixTranslation(m_tree[i].m_pos.x, m_tree[i].m_pos.y, m_tree[i].m_pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ワールドマトリックス設定
		XMStoreFloat4x4(&m_tree[i].m_mtxWorld, mtxWorld);
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void Tree::Draw()
{
	ID3D11DeviceContext* pDC = GetDeviceContext();

	for (int i = 0; i < MAX_TREE; ++i) {
		// 未使用ならスキップ
		if (!m_tree[i].m_use) {
			continue;
		}

		// 不透明な部分を作成
		m_model.Draw(pDC, m_mtxWorld, eOpacityOnly);
		DrawMesh(pDC, &g_treeMesh);
		// 半透明な部分を作成
		SetBlendState(BS_ALPHABLEND); // αブレンド有効
		SetZWrite(false);             // Zバッファ更新しない
		m_model.Draw(pDC, m_tree[i].m_mtxWorld, eTransparentOnly);
		SetZWrite(true);              // Zバッファ更新
		SetBlendState(BS_NONE);       // αブレンド無効
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void Tree::Draw(int num)
{
	ID3D11DeviceContext* pDC = GetDeviceContext();
	// 未使用ならスキップ
	if (!m_tree[num].m_use) {
		return;
	}

	// 不透明な部分を作成
	m_model.Draw(pDC, m_mtxWorld, eOpacityOnly);
	DrawMesh(pDC, &g_treeMesh);
	// 半透明な部分を作成
	SetBlendState(BS_ALPHABLEND); // αブレンド有効
	SetZWrite(false);             // Zバッファ更新しない
	m_model.Draw(pDC, m_tree[num].m_mtxWorld, eTransparentOnly);
	SetZWrite(true);              // Zバッファ更新
	SetBlendState(BS_NONE);       // αブレンド無効
}

// ========================
// 木  情報取得
// ========================
TTree* Tree::GetTree()
{
	return m_tree;
}

// ========================
// 木 過去未来わけ描画
// ========================
void Tree::DrawOldNow(int n_Time)
{
	ID3D11DeviceContext* pDC = GetDeviceContext();

	for (int i = 0; i < MAX_TREE; ++i) 
	{
		if (!m_tree[i].m_use) {
			continue;
		}
		if (m_tree[i].m_nTime != n_Time) {
			continue;
		}
		if (m_tree[i].m_nTime == 0) {
			// 不透明部分を描画
			m_model.Draw(pDC, m_tree[i].m_mtxWorld, eOpacityOnly);

			// 半透明部分を描画
			SetBlendState(BS_ALPHABLEND);	// アルファブレンド有効
			SetZWrite(false);				// Zバッファ更新しない
			m_model.Draw(pDC, m_tree[i].m_mtxWorld, eTransparentOnly);
			SetZWrite(true);				// Zバッファ更新する
			SetBlendState(BS_NONE);			// アルファブレンド無効
		}
		if (m_tree[i].m_nTime == 1) {
			// 不透明部分を描画
			m_sModel.Draw(pDC, m_tree[i].m_mtxWorld, eOpacityOnly);

			// 半透明部分を描画
			SetBlendState(BS_ALPHABLEND);	// アルファブレンド有効
			SetZWrite(false);				// Zバッファ更新しない
			m_sModel.Draw(pDC, m_tree[i].m_mtxWorld, eTransparentOnly);
			SetZWrite(true);				// Zバッファ更新する
			SetBlendState(BS_NONE);			// アルファブレンド無効
		}
	}
}

// ========================
// 木  生成(0が今、1が過去)
// ========================
int Tree::CreateOldNow(XMFLOAT3 pos, int n_Time)
{
	TTree* pTree = m_tree;
	for (int i = 0; i < MAX_TREE; ++i, ++pTree)
	{
		if (pTree->m_use) continue;
		pTree->m_pos = pos;
		pTree->m_use = true;
		pTree->m_nTime = n_Time;
		if (n_Time == 1)
		{
			pTree->m_scl = XMFLOAT3(10.0f,10.0f,10.0f);
		}
		return i;
	}
	return -1;
}