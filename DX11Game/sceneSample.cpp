//===================================================
//		サンプルシーン[sceneSample.cpp]
//小楠裕子
//2021/11/30	ファイル作成
//=====================================================
#include "sceneSample.h"
#include "fade.h"
#include "meshfield.h"
#include "model.h"
#include "shadow.h"
#include "bg.h"
#include "enemy.h"
#include "tree.h"
#include "bullet.h"
#include "explosion.h"
#include "bsphere.h"
#include "fire.h"
#include "smoke.h"
#include "ui.h"
#include "object_3D.h"
#include "cube.h"
#include "Sound.h"


//*****グローバル変数*****
static BG* g_pBG;		//背景
static UI* g_pUI;		//UI
static Object_3D* g_pObject3D;
static Cube*		g_pCube;
static MeshField* g_pMeshField;		//ﾌｨｰﾙﾄﾞ

//=============================
//		初期化
//=============================
HRESULT InitSceneSample() {
	HRESULT hr = MB_OK;

	//立方体初期化
	g_pCube = new Cube;

	// 境界球初期化
	hr = InitBSphere();
	if (FAILED(hr))
		return hr;

	// 丸影初期化
	hr = InitShadow();
	if (FAILED(hr))
		return hr;

	// モデル表示初期化
	g_pObject3D = new Object_3D;

	// 敵機初期化
	hr = InitEnemy();
	if (FAILED(hr))
		return hr;

	// フィールド初期化
	/*hr = InitMeshField(8, 8, 80.0f, 80.0f);
	if (FAILED(hr))
		return hr;*/
	g_pMeshField = new MeshField(8, 8, 80.0f, 80.0f, 1.0f, 1.0f, XMFLOAT3(0.0f, 0.0f, 0.0f));

	// 背景初期化
	g_pBG = new BG;

	//UI初期化
	g_pUI = new UI;

	// 木の初期化
	
	//ビルボード弾初期化
	hr = InitBullet();
	if (FAILED(hr)) {
		return hr;
	}

	//爆発処理初期化
	/*hr = InitExplosion();
	if (FAILED(hr)) {
		return hr;
	}*/

	//炎エフェクト初期化
	hr = InitFire();
	if (FAILED(hr)) {
		return hr;
	}

	//煙エフェクト初期化
	hr = InitSmoke();
	if (FAILED(hr)) {
		return hr;
	}

	CSound::Init();
	CSound::Play(BGM_000);
	return hr;
}

//=============================
//		終了
//=============================
void UninitSceneSample() {
	CSound::Stop(BGM_000);
	CSound::Fin();
	//立方体終了
	delete g_pCube;
	//煙エフェクト終了処理
	UninitSmoke();

	//炎エフェクト終了処理
	UninitFire();

	//爆発処理終了処理
	//UninitExplosion();

	//ビルボード弾終了処理
	UninitBullet();

	// 木の終了処理

	// 背景終了処理
	delete g_pBG;

	//UI終了処理
	delete g_pUI;

	// フィールド終了処理
	//UninitMeshField();
	delete g_pMeshField;

	// 敵機終了処理
	UninitEnemy();

	// モデル表示終了処理
	delete g_pObject3D;

	// 丸影終了処理
	UninitShadow();

	// 境界球終了処理
	UninitBSphere();
}

//=============================
//		更新
//=============================
void UpdateSceneSample() {
	// モデル更新
	g_pObject3D->Update();

	//ビルボード弾更新
	UpdateBullet();

	// 敵機更新
	UpdateEnemy();

	// 背景更新
	g_pBG->Update(5);

	//UI更新
	g_pUI->Update();

	// フィールド更新
	//UpdateMeshField();
	g_pMeshField->Update();

	// 木の更新

	// 丸影更新
	UpdateShadow();

	// 境界球更新
	UpdateBSphere();

	//爆発処理更新
	//UpdateExplosion();

	//炎エフェクト更新
	UpdateFire();

	//煙エフェクト更新
	UpdateSmoke();

	//立方体更新
	g_pCube->Update();

	CSound::Update();
}

//=============================
//		描画
//=============================
void DrawSceneSample() {
	// Zバッファ無効(Zチェック無&Z更新無)
	SetZBuffer(false);

	// 背景描画
	g_pBG->Draw();


	// Zバッファ有効(Zチェック有&Z更新有)
	SetZBuffer(true);

	// フィールド描画
	//DrawMeshField();
	g_pMeshField->Draw();

	//立方体描画
	g_pCube->Draw();

	// モデル描画
	g_pObject3D->Draw();

	// 敵機描画
	DrawEnemy();

	// 丸影描画
	DrawShadow();

	//ビルボード弾描画
	DrawBullet();

	// 木の描画

	// 境界球描画
	DrawBSphere();

	//爆発処理描画
	//DrawExplosion();

	//炎エフェクト描画
	DrawFire();

	//煙エフェクト描画
	DrawSmoke();

	// Zバッファ無効(Zチェック無&Z更新無)
	SetZBuffer(false);

	//UI描画
	g_pUI->Draw();
}

Cube* GetCube() {
	return g_pCube;
}