//=============================================================================
//
// マップデータ [mapData.h]
// Author : 加藤　匠
//
//=============================================================================
#pragma once
#include "map.h"

typedef struct
{
	OBJECT_INFO* nowMap;
	OBJECT_INFO* pOldMap;
}MAP_DATA;

MAP_DATA GetMapData(int nStage);