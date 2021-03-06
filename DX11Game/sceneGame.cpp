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
#include "pause.h"
#include "Sound.h"
#include "bg.h"
#include "Goal.h"
#include "gimmick.h"
#include "shadow.h"
#include "effect.h"
#include "break.h"
#include "torch.h"


//*****定数定義*****
#define OLD_SCROLL_SPEED	(1.0f)
#define NOW_SCROLL_SPEED	(1.0f)


//*****グローバル変数*****
static Old* g_pOld;			//過去
static Now* g_pNow;			//現在
//static Box* g_pBox;		//箱
static BG* g_pBG;			//背景
static Goal* g_pGoal;		//ゴール
static Gimmick* g_pGimmick;	//ギミック
static Effect* g_pEffect;   //エフェクト
static Break* g_pBreak;
static Torch* g_pTorct;

const float FRAME_BUFFER_W = SCREEN_WIDTH;   //フレームバッファの幅。
const float FRAME_BUFFER_H = SCREEN_HEIGHT;   //フレームバッファの高さ。
ID3D11DeviceContext* d3dDeviceContext;   //D3D11デバイスコンテキスト、初期化済みとする
D3D11_VIEWPORT viewPorts[2];   //分割ビューポート、これをモデルの描画前に設定する
D3D11_VIEWPORT viewPortsReset;   //分割ビューポート、これをモデルの描画前に設定する

float g_fBoyOldPosX;	// 男の子の過去座標
float g_fGirlOldPosX;	// 女の子の過去座標

bool PauseFlg = false;

XMFLOAT3 g_girlCameraTarget;
XMFLOAT3 g_boyCameraTarget;

//=============================
//		初期化
//=============================
HRESULT InitSceneGame() {
	HRESULT hr = MB_OK;
	//カメラ初期化
	CCamera::Get()->Init();
	CCamera::Set(OLD_CAMERA);
	CCamera::Get()->Init();
	//過去初期化
	g_pOld = new Old;
	//現在初期化
	g_pNow = new Now;
	// 背景初期化
	g_pBG = new BG;
	//ゴール初期化
	g_pGoal = new Goal;
	//ギミック初期化
	g_pGimmick = new Gimmick;
	//エフェクト初期化
	g_pEffect = new Effect;
	//破壊初期化
	g_pBreak = new Break;
	//松明初期化
	g_pTorct = new Torch;


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
	InitPause();

	//サウンド初期化
	CSound::Init();
	CSound::Play(BGM_001);

	g_girlCameraTarget = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_boyCameraTarget = XMFLOAT3(0.0f, 0.0f, 0.0f);

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
	//背景終了処理
	delete g_pBG;
	//ゴール終了処理
	delete g_pGoal;
	//ギミック終了処理
	delete g_pGimmick;
	//エフェクト終了処理
	delete g_pEffect;
	//破壊終了処理
	delete g_pBreak;
	//松明終了処理
	delete g_pTorct;

	// 丸影終了処理
	UninitShadow();

	//マップ終了
	UninitMap();


	UninitPause();

	//ゴール終了
	//UninitGoal();

	//サウンド終了
	CSound::Stop(BGM_001);
	CSound::Fin();

}

//=============================
//		更新
//=============================
void UpdateSceneGame() {

	PauseFlg = UpdatePause();


	if (!PauseFlg)
	{

		//箱更新
		//g_pBox->Update();
		//マップ更新
		//UpdateMap();

		//ゴール更新
		g_pGoal->Update(g_pNow->GetPlayerGirl()->GetGirlPos().x);

		//過去更新
		g_pOld->Update();

		//現在更新
		g_pNow->Update();

		// 丸影更新
		UpdateShadow();

		//ギミック更新
		g_pGimmick->Update(g_pOld->GetBoyPos());

		//エフェクト更新
		g_pEffect->UpdateEffect();

		//破壊更新
		g_pBreak->UpdateBreak();

		//松明更新
		g_pTorct->UpdateTorch();

		//マップ更新
		UpdateMap();

		// 画面をスクロール
		if (g_pNow->GetPlayerGirl()->GetGirlPos().x > -100.0f)
		{
			if (g_fGirlOldPosX != g_pNow->GetPlayerGirl()->GetGirlPos().x)
			{
				//今の背景更新
				g_pBG->Update(0);

				g_girlCameraTarget.x += g_pNow->GetPlayerGirl()->GetGirlMove().x * NOW_SCROLL_SPEED;

				CCamera::Set(NOW_CAMERA);
				XMFLOAT3 cameraPos = CCamera::Get()->GetPos();
				CCamera::Get()->SetPos(cameraPos.x += g_pNow->GetPlayerGirl()->GetGirlMove().x * NOW_SCROLL_SPEED, cameraPos.y, cameraPos.z);
				CCamera::Get()->SetTarget(g_girlCameraTarget);
				g_fGirlOldPosX = g_pNow->GetPlayerGirl()->GetGirlPos().x;
			}
		}
		if (g_pOld->GetBoyPos().x > -100.0f)
		{
			if (g_fBoyOldPosX != g_pOld->GetBoyPos().x)
			{
				//過去の背景更新
				g_pBG->Update(1);

				g_boyCameraTarget.x += g_pOld->GetPlayerBoy()->GetBoyMove().x * OLD_SCROLL_SPEED;

				CCamera::Set(OLD_CAMERA);
				XMFLOAT3 cameraPos = CCamera::Get()->GetPos();
				CCamera::Get()->SetPos(cameraPos.x += g_pOld->GetPlayerBoy()->GetBoyMove().x * OLD_SCROLL_SPEED, cameraPos.y, cameraPos.z);
				CCamera::Get()->SetTarget(g_boyCameraTarget);
				g_fBoyOldPosX = g_pOld->GetBoyPos().x;
			}
		}

		if (GetKeyTrigger(VK_R)) {
			StartFadeOut(SCENE_GAME);
		}
	}
	CSound::Update();
}

//=============================
//		描画
//=============================
void DrawSceneGame() {
	d3dDeviceContext = GetDeviceContext();

	//背景描画
	g_pBG->Draw();

	//ビューポートを設定　上画面
	CCamera::Set(NOW_CAMERA);
	d3dDeviceContext->RSSetViewports(1, &viewPorts[0]);
	//今描画
	g_pNow->Draw();
	g_pEffect->DrawEffect();
	g_pGimmick->NowDraw();
	g_pBreak->DrawBreak();
	g_pTorct->DrawTorch();

	//ビューポートを設定　下画面
	CCamera::Set(OLD_CAMERA);
	d3dDeviceContext->RSSetViewports(1, &viewPorts[1]);
	//過去描画
	g_pOld->Draw();
	g_pEffect->DrawEffect();
	g_pGimmick->OldDraw();
	g_pBreak->DrawBreak();
	g_pTorct->DrawTorch();

	//ビューポートの設定を元に戻す
	d3dDeviceContext->RSSetViewports(1, &viewPortsReset);

	DrawPause();
	//ゴール描画
	g_pGoal->Draw();

}

//=============================
//		過去取得
//=============================
Old* GetOld() {
	return g_pOld;
}

//=============================
//		現在取得
//=============================
Now* GetNow() {
	return g_pNow;
}