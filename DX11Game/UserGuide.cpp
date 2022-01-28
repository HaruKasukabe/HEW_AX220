//===================================================
//		�������[UserGuide.cpp]
//����y�s
//2022/1/25	�t�@�C���쐬
//=====================================================
#include "UserGuide.h"
#include "fade.h"
#include "bg.h"
#include "Texture.h"
#include "polygon.h"
#include "input.h"
#include "mesh.h"

// ***************************************
// �}�N����`
// ***************************************
#define KEYBOARD_TEXTURE_PATH L"data/texture/Guide_K.png"
#define CONTROLLER_TEXTURE_PATH L"data/texture/Guide_C.png"
#define USERGUIDE_POS XMFLOAT2(0.0f, 0.0f)
#define MAX_TEXTURE (2)

// ****************************************
// �O���[�o���ϐ�
// ****************************************
static MESH	g_UserGuideMesh;					// �\����
static ID3D11ShaderResourceView* g_UserGuideTexture[MAX_TEXTURE];
static int g_UserPadTimer;
static bool bUserFlg; // ��ʂ�\�����Ă��邩
static int g_DispNum; // �\����ʂ̐���

//=============================
//		������
//=============================
HRESULT InitUserGuide() {
	HRESULT hr = MB_OK;
	ID3D11Device* pDevice = GetDevice();

	bUserFlg = false;
	g_DispNum = 0;

	hr = CreateTextureFromFile(pDevice, KEYBOARD_TEXTURE_PATH, &g_UserGuideTexture[0]);
	if (FAILED(hr))
		return hr;
	hr = CreateTextureFromFile(pDevice, CONTROLLER_TEXTURE_PATH, &g_UserGuideTexture[1]);
	if (FAILED(hr))
		return hr;



	return hr;
}

//=============================
//		�I��
//=============================
void UninitUserGuide() {
	// ���b�V���J��
	for (int i = 0; i < MAX_TEXTURE; i++)
	{
		SAFE_RELEASE(g_UserGuideTexture[i]);
	}
	g_DispNum = 0;
}

//=============================
//		�X�V
//=============================
void UpdateUserGuide() {
	if (GetKeyPress(VK_X) || GetJoyButton(0, JOYBUTTON3))
	{
		// X�L�[���������瑀������̉�ʂ����
		bUserFlg = false;
	}
	if (g_DispNum == 0 && (GetJoyDpadRight(0) || GetKeyPress(VK_RIGHT)))
	{
		// �L�[�{�[�h�\���̂Ƃ��E������������۰װ��\��
		g_DispNum = 1;
	}
	if (g_DispNum == 1 && (GetJoyDpadLeft(0) || GetKeyPress(VK_LEFT)))
	{
		// �L�[�{�[�h�\���̂Ƃ��E������������۰װ��\��
		g_DispNum = 0;
	}

	if (g_DispNum > 1 || g_DispNum < 0)
	{
		g_DispNum = 0;
	}
}

//=============================
//		�`��
//=============================
void DrawUserGuide() {
	if (bUserFlg) {
		ID3D11DeviceContext* pDC = GetDeviceContext();
		SetBlendState(BS_ALPHABLEND);
		SetPolygonSize(SCREEN_WIDTH, SCREEN_HEIGHT);
		SetPolygonPos(USERGUIDE_POS.x,USERGUIDE_POS.y);
		SetPolygonAngle(0.0f);
		SetPolygonTexture(g_UserGuideTexture[g_DispNum]);
		DrawPolygon(pDC);
		SetBlendState(BS_NONE);
	}
}

//=============================
//	�`��t���O��ݒ�
//=============================
void SetUserGuideFlg(bool Flg)
{
	bUserFlg = Flg;
}

//=============================
//	�`��t���O��n��
//=============================
bool GetUserGuideFlg()
{
	return bUserFlg;
}