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

// support for GUI for polygon overlay demo
//
#ifndef _POVER_VIDEO_H_
#define _POVER_VIDEO_H_
#include "../../common/gui/video.h"

#include "pover_global.h"  // for declaration of DEFINE and INIT

DEFINE class video *gVideo INIT(0);

DEFINE int n_next_frame_calls INIT(0);
DEFINE int frame_skips INIT(10);
extern bool g_next_frame();
extern bool g_last_frame();

class pover_video: public video {
    void on_process();
public:
#ifdef _WINDOWS
    bool graphic_display(){return video::win_hInstance != (HINSTANCE)NULL;}
#else
    bool graphic_display() { return true;} // fix this for Linux
#endif
    //void on_key(int key);
};

DEFINE int g_xwinsize INIT(1024);
DEFINE int g_ywinsize INIT(768);

DEFINE int map1XLoc INIT(10);
DEFINE int map1YLoc INIT(10);
DEFINE int map2XLoc INIT(270);
DEFINE int map2YLoc INIT(10);
DEFINE int maprXLoc INIT(530);
DEFINE int maprYLoc INIT(10);

DEFINE const char *g_windowTitle INIT("Polygon Overlay");
DEFINE bool g_useGraphics INIT(true);

extern bool initializeVideo(int argc, char **argv);

extern void rt_sleep(int msec);

#endif  // _POVER_VIDEO_H_
