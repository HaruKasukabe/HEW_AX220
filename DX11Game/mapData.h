//=============================================================================
//
// �}�b�v�f�[�^ [mapData.h]
// Author : �����@��
//
//=============================================================================
#pragma once
#include "map.h"

enum STAGE
{
	STAGE_TUTORIAL,
	STAGE_1,

	MAX_STAGE
};

OBJECT_INFO GetNowMapData(int, int);
OBJECT_INFO GetOldMapData(int, int);