//=============================================================================
//
// マップデータ [mapData.h]
// Author : 加藤　匠
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