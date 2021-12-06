//===================================================
//		ゲームシーン[sceneGame.cpp]
//小楠裕子
//=====================================================
#include "sceneGame.h"
#include "fade.h"
#include "Sound.h"
#include "old.h"
#include "now.h"
#include "input.h"
//#include "box.h"
#include "map.h"

//*****グローバル変数*****
static Old* g_pOld;		//過去
static Now* g_pNow;		//現在
//static Box* g_pBox;		//箱

const float FRAME_BUFFER_W = SCREEN_WIDTH;   //フレームバッファの幅。
const float FRAME_BUFFER_H = SCREEN_HEIGHT;   //フレームバッファの高さ。
ID3D11DeviceContext* d3dDeviceContext;   //D3D11デバイスコンテキスト、初期化済みとする
D3D11_VIEWPORT viewPorts[2];   //分割ビューポート、これをモデルの描画前に設定する
D3D11_VIEWPORT viewPortsReset;   //分割ビューポート、これをモデルの描画前に設定する

//=============================
//		初期化
//=============================
HRESULT InitSceneGame() {
	HRESULT hr = MB_OK;
	//過去初期化
	g_pOld = new Old;
	//現在初期化
	g_pNow = new Now;

	//箱初期化
	//g_pBox = new Box;
	//g_pBox->Create(XMFLOAT3(0.0f, -50.0f, 0.0f));
	//g_pBox->Create(XMFLOAT3(0.0f, -30.0f, 0.0f));
	//g_pBox->Create(XMFLOAT3(0.0f, -10.0f, 0.0f));
	//g_pBox->Create(XMFLOAT3(0.0f, 10.0f, 0.0f));
	//g_pBox->Create(XMFLOAT3(0.0f, 30.0f, 0.0f));

	//これが上画面
	viewPorts[0].Width = FRAME_BUFFER_W;   //画面の横サイズ
	viewPorts[0].Height = FRAME_BUFFER_H / 2;   //画面の縦サイズ
	viewPorts[0].TopLeftX = 0;   //画面左上のx座標
	viewPorts[0].TopLeftY = 0;   //画面左上のy座標
	viewPorts[0].MinDepth = 0.0f;   //深度値の最小値
	viewPorts[0].MaxDepth = 1.0f;   //深度値の最大値

	//これが下画面
	viewPorts[1].Width = FRAME_BUFFER_W;   //画面の横サイズ
	viewPorts[1].Height = FRAME_BUFFER_H / 2;   //画面の縦サイズ
	viewPorts[1].TopLeftX = 0;   //画面左上のx座標
	viewPorts[1].TopLeftY = FRAME_BUFFER_H / 2;   //画面左上のy座標
	viewPorts[1].MinDepth = 0.0f;   //深度値の最小値
	viewPorts[1].MaxDepth = 1.0f;   //深度値の最大値

	viewPortsReset.Width = FRAME_BUFFER_W;   //画面の横サイズ
	viewPortsReset.Height = FRAME_BUFFER_H;   //画面の縦サイズ

	//マップ初期化
	InitMap();

	CSound::Init();
	CSound::Play(BGM_000);
	return hr;
}

//=============================
//		終了
//=============================
void UninitSceneGame() {
	//過去終了
	delete g_pOld;
	//現在終了
	delete g_pNow;

	//マップ終了
	UninitMap();

	CSound::Stop(BGM_000);
	CSound::Fin();
	
}

//=============================
//		更新
//=============================
void UpdateSceneGame() {
	//過去更新
	g_pOld->Update();

	//現在更新
	g_pNow->Update();

	//箱更新
	//g_pBox->Update();
	//マップ更新
	UpdateMap();


	if (GetKeyPress(VK_F1)) {
		StartFadeOut(SCENE_TITLE);
	}



	CSound::Update();
}

//=============================
//		描画
//=============================
void DrawSceneGame() {
	d3dDeviceContext = GetDeviceContext();

		//ビューポートを設定　上画面
		d3dDeviceContext->RSSetViewports(1, &viewPorts[0]);
		//今描画
		g_pNow->Draw();

		//ビューポートを設定　下画面
		d3dDeviceContext->RSSetViewports(1, &viewPorts[1]);
		//過去描画
		g_pOld->Draw();


		//g_pBox->Draw();

	//ビューポートの設定を元に戻す
	d3dDeviceContext->RSSetViewports(1, &viewPortsReset);
}

//=============================
//		過去取得
//=============================
Old* GetOld() {
	return g_pOld;
}






