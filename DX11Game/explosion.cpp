//========================================================
//
//爆破処理[explosion.cpp]
//
//========================================================
#include "explosion.h"
#include "Camera.h"
#include "Texture.h"
#include "mesh.h"
#include "Light.h"

//*********************************************************
//マクロ定義
//*********************************************************
#define TEXTURE_FILENAME	L"data/texture/dash.png"	//テクスチャファイル名
#define M_DIFFUSE			XMFLOAT4(1.0f,1.0f,1.0f,1.0f)
#define M_SPECULAR			XMFLOAT4(0.0f,0.0f,0.0f,1.0f)
#define M_POWER				(1.0f)
#define M_AMBIENT			XMFLOAT4(0.0f,0.0f,0.0f,1.0f)
#define M_EMISSIVE			XMFLOAT4(0.0f,0.0f,0.0f,1.0f)

#define MAX_EXPLOSION		100

#define EXPLOSION_FRAME_X	(4)			//横フレーム数
#define EXPLOSION_FRAME_Y	(5)			//縦フレーム数
#define EXPLOSION_ANIM_COUNT	1			//一コマ当たりの表示回数

//***********************************************************
//構造体定義
//***********************************************************
struct TExplosion {
	XMFLOAT3	pos;		//位置
	XMFLOAT2	size;		//サイズ
	bool		use;		//使用しているかどうか
	int			anim;		//アニメーション再生位置
	int			count;		//アニメーションフレーム数
};

//*************************************************************
//グローバル変数
//*************************************************************
static MESH		g_mesh;			//メッシュ情報
static MATERIAL	g_material;		//マテリアル
static TExplosion	g_explosion[MAX_EXPLOSION];		//爆発情報

//**************************************************************
//プロトタイプ宣言
//**************************************************************
static HRESULT MakeVertexExplosion(ID3D11Device* pDevice);

//==============================================================
//初期化
//==============================================================
HRESULT InitExplosion(void) {
	ID3D11Device* pDevice = GetDevice();
	HRESULT hr = S_OK;

	//頂点情報の作成
	hr = MakeVertexExplosion(pDevice);
	if (FAILED(hr)) {
		return hr;
	}

	//マテリアルの設定
	g_material.Diffuse = M_DIFFUSE;
	g_material.Ambient = M_AMBIENT;
	g_material.Specular = M_SPECULAR;
	g_material.Power = M_POWER;
	g_material.Emissive = M_EMISSIVE;
	g_mesh.pMaterial = &g_material;

	//テクスチャの読み込み
	hr = CreateTextureFromFile(pDevice, TEXTURE_FILENAME, &g_mesh.pTexture);
	if (FAILED(hr)) {
		return hr;
	}
	XMStoreFloat4x4(&g_mesh.mtxTexture, XMMatrixIdentity());

	//ワールドマトリックス初期化
	XMStoreFloat4x4(&g_mesh.mtxWorld, XMMatrixIdentity());

	//爆発情報初期化
	for (int i = 0; i < MAX_EXPLOSION; ++i) {
		g_explosion[i].pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_explosion[i].size = XMFLOAT2(10.0f,10.0f);
		g_explosion[i].use = false;
		g_explosion[i].anim = 0;
		g_explosion[i].count = EXPLOSION_ANIM_COUNT;
	}
	
	return hr;
}

//==============================================================
//終了処理
//==============================================================
void UninitExplosion(void) {
	ReleaseMesh(&g_mesh);
}

//==============================================================
//更新
//==============================================================
void UpdateExplosion(void) {
	TExplosion* pExplosion = g_explosion;
	for (int i = 0; i < MAX_EXPLOSION; ++i, ++pExplosion) {
		//未使用ならスキップ
		if (!pExplosion->use) {
			continue;
		}
		//カウンタ更新
		--pExplosion->count;
		if (pExplosion->count <= 0) {
			//アニメーション更新
			++pExplosion->anim;
			if (pExplosion->anim >= EXPLOSION_FRAME_X * EXPLOSION_FRAME_Y) {
				pExplosion->use = false;	//消滅
				continue;
			}
			//カウンタ初期化
			pExplosion->count = EXPLOSION_ANIM_COUNT;
		}
	}
}

//==============================================================
//描画
//==============================================================
void DrawExplosion(void) {
	ID3D11DeviceContext* pDC = GetDeviceContext();

	CLight::Get()->SetDisable();		//光源無効
	SetBlendState(BS_ALPHABLEND);		//αブレンディング有効
	XMFLOAT4X4& mView = CCamera::Get()->GetViewMatrix();
	TExplosion* pExplosion = g_explosion;
	for (int i = 0; i < MAX_EXPLOSION; ++i, ++pExplosion) {
		//未使用ならスキップ
		if (!pExplosion->use) {
			continue;
		}
		//テクスチャマトリックス更新
		XMMATRIX mtxTex = XMMatrixScaling(1.0f / EXPLOSION_FRAME_X, 1.0f / EXPLOSION_FRAME_Y, 1.0f);
		int u = pExplosion->anim % EXPLOSION_FRAME_X;
		int v = pExplosion->anim / EXPLOSION_FRAME_X;
		mtxTex = XMMatrixMultiply(mtxTex, XMMatrixTranslation((float)u / EXPLOSION_FRAME_X, (float)v / EXPLOSION_FRAME_Y, 0.0f));
		XMStoreFloat4x4(&g_mesh.mtxTexture, mtxTex);
		//ビュー行列の回転成分の転置行列を設定(+拡縮)
		g_mesh.mtxWorld._11 = mView._11 * pExplosion->size.x;
		g_mesh.mtxWorld._12 = mView._21 * pExplosion->size.x;
		g_mesh.mtxWorld._13 = mView._31 * pExplosion->size.x;
		g_mesh.mtxWorld._14 = 0.0f;
		g_mesh.mtxWorld._21 = mView._12 * pExplosion->size.y;
		g_mesh.mtxWorld._22 = mView._22 * pExplosion->size.y;
		g_mesh.mtxWorld._23 = mView._32 * pExplosion->size.y;
		g_mesh.mtxWorld._24 = 0.0f;
		g_mesh.mtxWorld._31 = mView._13;
		g_mesh.mtxWorld._32 = mView._23;
		g_mesh.mtxWorld._33 = mView._33;
		g_mesh.mtxWorld._34 = 0.0f;
		//位置を反映
		g_mesh.mtxWorld._41 = pExplosion->pos.x;
		g_mesh.mtxWorld._42 = pExplosion->pos.y;
		g_mesh.mtxWorld._43 = pExplosion->pos.z;
		g_mesh.mtxWorld._44 = 1.0f;
		//描画
		DrawMesh(pDC, &g_mesh);
	}
	SetBlendState(BS_NONE);		//αブレンディング無効
	CLight::Get()->SetEnable();	//光源有効
}

//==============================================================
//爆発開始
//==============================================================
int StartExplosion(XMFLOAT3 pos, XMFLOAT2 size) {
	TExplosion* pExplosion = g_explosion;
	for (int i = 0; i < MAX_EXPLOSION; ++i, ++pExplosion) {
		//使用中ならスキップ
		if (pExplosion->use) {
			continue;
		}
		pExplosion->pos = pos;
		pExplosion->size = size;
		pExplosion->anim = 0;
		pExplosion->count = EXPLOSION_ANIM_COUNT;
		pExplosion->use = true;
		return i;		//爆発した(0～爆発番号）
	}
	return -1;			//爆発しなかった
}

//==============================================================
//頂点情報の作成
//==============================================================
HRESULT MakeVertexExplosion(ID3D11Device* pDevice) {
	//一時的な頂点配列を生成
	g_mesh.nNumVertex = 4;
	VERTEX_3D* pVertexWk = new VERTEX_3D[g_mesh.nNumVertex];

	//頂点配列の中身を埋める
	pVertexWk[0].vtx = XMFLOAT3(-0.5f,0.5f, 0.0f);
	pVertexWk[1].vtx = XMFLOAT3(0.5f, 0.5f, 0.0f);
	pVertexWk[2].vtx = XMFLOAT3(-0.5f, -0.5f, 0.0f);
	pVertexWk[3].vtx = XMFLOAT3(0.5f, -0.5f, 0.0f);
	pVertexWk[0].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	pVertexWk[1].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	pVertexWk[2].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	pVertexWk[3].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	pVertexWk[0].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);
	pVertexWk[1].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);
	pVertexWk[2].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);
	pVertexWk[3].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);
	pVertexWk[0].tex = XMFLOAT2(0.0f, 0.0f);
	pVertexWk[1].tex = XMFLOAT2(1.0f, 0.0f);
	pVertexWk[2].tex = XMFLOAT2(0.0f, 1.0f);
	pVertexWk[3].tex = XMFLOAT2(1.0f, 1.0f);

	//一時的なインデックス配列を生成
	g_mesh.nNumIndex = 4;
	int* pIndexWk = new int[4];

	//インデックス配列の中身を埋める
	pIndexWk[0] = 0;
	pIndexWk[1] = 1;
	pIndexWk[2] = 2;
	pIndexWk[3] = 3;

	//頂点バッファ・インデックスバッファ生成
	HRESULT hr = MakeMeshVertex(pDevice, &g_mesh, pVertexWk, pIndexWk);

	//一時的な頂点配列・インデックス配列を解放
	delete[] pIndexWk;
	delete[] pVertexWk;
	return hr;
}