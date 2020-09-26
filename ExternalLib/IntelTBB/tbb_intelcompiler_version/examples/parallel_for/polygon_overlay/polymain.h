/*
    Copyright 2005-2010 Intel Corporation.  All Rights Reserved.

    The source code contained or described herein and all documents related
    to the source code ("Material") are owned by Intel Corporation or its
    suppliers or licensors.  Title to the Material remains with Intel
    Corporation or its suppliers and licensors.  The Material is protected
    by worldwide copyright laws and treaty provisions.  No part of the
    Material may be used, copied, reproduced, modified, published, uploaded,
    posted, transmitted, distributed, or disclosed in any way without
    Intel's prior express written permission.

    No license under any patent, copyright, trade secret or other
    intellectual property right is granted to or conferred upon you by
    disclosure or delivery of the Materials, either expressly, by
    implication, inducement, estoppel or otherwise.  Any license under such
    intellectual property rights must be express and approved by Intel in
    writing.
*/

#include "pover_global.h"  // for declaration of DEFINE and INIT

DEFINE Polygon_map_t *gPolymap1 INIT(0);
DEFINE Polygon_map_t *gPolymap2 INIT(0);
DEFINE Polygon_map_t *gResultMap INIT(0);

extern void Usage(int argc, char **argv);

extern bool ParseCmdLine(int argc, char **argv );

extern bool GenerateMap(Polygon_map_t **newMap, int xSize, int ySize, int gNPolygons, colorcomp_t maxR, colorcomp_t maxG, colorcomp_t maxB);

extern bool PolygonsOverlap(RPolygon *p1, RPolygon *p2, int &xl, int &yl, int &xh, int &yh);

extern void CheckPolygonMap(Polygon_map_t *checkMap);

extern bool CompOnePolygon(RPolygon *p1, RPolygon *p2);

extern bool PolygonsEqual(RPolygon *p1, RPolygon *p2);

extern bool ComparePolygonMaps(Polygon_map_t *map1, Polygon_map_t *map2);

extern void SetRandomSeed(int newSeed);

extern int NextRan(int n);
