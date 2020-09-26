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

/*!
 * polyover.h : extern declarations for polyover.cpp
*/
#include "rpolygon.h"
#include "tbb/mutex.h"
#include "tbb/spin_mutex.h"

extern void OverlayOnePolygonWithMap(Polygon_map_t *resultMap, RPolygon *myPoly, Polygon_map_t  *map2, tbb::spin_mutex *rMutex);

extern void SerialOverlayMaps(Polygon_map_t **resultMap, Polygon_map_t *map1, Polygon_map_t *map2);

// extern void NaiveParallelOverlay(Polygon_map_t **result_map, Polygon_map_t *polymap1, Polygon_map_t *polymap2);
extern void NaiveParallelOverlay(Polygon_map_t *&result_map, Polygon_map_t &polymap1, Polygon_map_t &polymap2);

extern void SplitParallelOverlay(Polygon_map_t **result_map, Polygon_map_t *polymap1, Polygon_map_t *polymap2);

extern void CheckPolygonMap(Polygon_map_t *checkMap);
extern bool ComparePolygonMaps(Polygon_map_t *map1, Polygon_map_t *map2);

