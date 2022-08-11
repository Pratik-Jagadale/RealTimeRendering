#ifndef TERRAIN_H_
#define TERRAIN_H_

#include "Common.h"

BOOL InItTerrain(void);
void UnitiallizaTerrain(void);
void drawTerrain(void);
void initializeTerrainCoords(void);
BOOL loadHeightMapData(const char *filename);

#endif