//=============================================================================
//
// �ꎞ��~ [pause.cpp]
// Author : ����y�s
//
//=============================================================================
#include "pause.h"
#include "polygon.h"
#include "Texture.h"
#include "mesh.h"
#include "input.h"
#include "scene.h"
#include "Sound.h"

// ***************************************
// �}�N����`
// ***************************************
#define PAUSE_TEXTURE_PATH L"data/texture/pause.png"
#define SPAUSE_TEXTURE_PATH L"data/texture/triangle.png"
#define MAX_TEXTURE (2)


// ****************************************
// �\����
// ****************************************
struct PAUSE
{
	XMFLOAT2 m_pos;
	XMFLOAT2 m_size;
	bool m_pause;
};

// ****************************************
// �O���[�o���ϐ�
// ****************************************
static MESH	g_pauseMesh;					// �\����
static PAUSE g_pause[MAX_TEXTURE];
static ID3D11ShaderResourceView* g_pPauseTexture[MAX_TEXTURE];
static EScene g_Scene = SCENE_NONE;
static JOYINFOEX *pad; // �Q�[���p�b�h�\���{�^���̂���
static int g_padTimer;


HRESULT InitPause()
{
	ID3D11Device* pDevice = GetDevice();

	// ����������
	for (int i = 0; i < MAX_TEXTURE; i++)
	{
		g_pause[0].m_pos = XMFLOAT2(0.0f, 0.0f);
		g_pause[1].m_pos = XMFLOAT2(-300.0f, -50.0f);
		g_pause[i].m_size = XMFLOAT2(0.0f, 0.0f);
		g_pause[i].m_pause = false; // �f�t�H���g��false(�~�܂��Ă��Ȃ�)
	}

	pad = GetJoyState(0);
	g_padTimer = 0;

	// �e�N�X�`���̓ǂݍ���
	HRESULT hr;
	hr = CreateTextureFromFile(pDevice, PAUSE_TEXTURE_PATH, &g_pPauseTexture[0]);
	if (FAILED(hr))
	{
		MessageBoxA(GetMainWnd(), "�e�N�X�`���ǂݍ��݃G���[", "�|�[�Y�̃e�N�X�`��", MB_OK);
		return hr;
	}
	hr = CreateTextureFromFile(pDevice, SPAUSE_TEXTURE_PATH, &g_pPauseTexture[1]);
	if (FAILED(hr))
	{
		MessageBoxA(GetMainWnd(), "�e�N�X�`���ǂݍ��݃G���[", "�O�p�e�N�X�`��", MB_OK);
		return hr;
	}
	return hr;
}

void UninitPause()
{
	// ���b�V���J��
	for (int i = 0; i < MAX_TEXTURE; i++)
	{
		SAFE_RELEASE(g_pPauseTexture[i]);
		g_pause[i].m_pause = false;
	}
}

bool UpdatePause()
{
	pad = GetJoyState(0);
	PrintDebugProc("state:%d\n", pad->dwPOV);


	if (GetKeyTrigger(VK_M)||GetJoyButton(0, JOYBUTTON8))
	{
			g_pause[0].m_pause = true;
			g_pause[1].m_pause = true;
	}

	if (g_pause[0].m_pause)
	{
		if (GetKeyTrigger(VK_UP) || GetJoyDpadUp(0)/*��*/)
		{
			if (g_pause[1].m_pos.y < -1.0f)
			{
				g_pause[1].m_pos.y += 100.0f;
					CSound::Play(SE_PAUSE_SELECT);
			}
		}
		if (GetKeyTrigger(VK_DOWN)|| GetJoyDpadDown(0)/*��*/)
		{
			if (g_pause[1].m_pos.y > -130.0f)
			{
				g_pause[1].m_pos.y -= 100.0f;
				CSound::Play(SE_PAUSE_SELECT);
			}
		}
		if ((GetKeyPress(VK_X) || (GetJoyTrigger(0, JOYBUTTON1)) && g_pause[1].m_pos.y == 50.0f))
		{
			g_pause[0].m_pause = false;
			g_pause[1].m_pause = false;
			CSound::Play(SE_DECIDE);
		}
		else if ((GetKeyPress(VK_X)|| GetJoyTrigger(0, JOYBUTTON1)) && g_pause[1].m_pos.y == -50.0f)
		{
			// �����܂����삵�Ȃ��̂ňꎞ�I�ɖ�����
			// SetScene(SCENE_STAGE);	
			CSound::Play(SE_DECIDE);
			return true;
		}
		else if ((GetKeyPress(VK_X) || GetJoyTrigger(0, JOYBUTTON1)) && g_pause[1].m_pos.y == 50.0f)
		{
			/*���e�͑���������ł�����L�q����*/
			/*CSound::Play(SE_DECIDE);
			return true;*/
		}
	}
	return g_pause[0].m_pause;
}

void DrawPause()
{
	if (g_pause[0].m_pause)
	{
		ID3D11DeviceContext* pDC = GetDeviceContext();
		SetPolygonSize(500, 500);
		SetPolygonPos(g_pause[0].m_pos.x, g_pause[0].m_pos.y);
		SetPolygonAngle(0.0f);
		SetPolygonTexture(g_pPauseTexture[0]);
		DrawPolygon(pDC);

		pDC = GetDeviceContext();
		SetBlendState(BS_ALPHABLEND);
		SetZBuffer(false);
		SetPolygonSize(100, 100);
		SetPolygonPos(g_pause[1].m_pos.x, g_pause[1].m_pos.y);
		SetPolygonAngle(0.0f);
		SetPolygonTexture(g_pPauseTexture[1]);
		DrawPolygon(pDC);
		SetBlendState(BS_NONE);
		SetZBuffer(true);
	}
}
