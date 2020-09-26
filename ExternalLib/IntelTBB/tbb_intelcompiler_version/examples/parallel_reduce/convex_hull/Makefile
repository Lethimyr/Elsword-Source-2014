# Copyright 2005-2010 Intel Corporation.  All Rights Reserved.
#
# The source code contained or described herein and all documents related
# to the source code ("Material") are owned by Intel Corporation or its
# suppliers or licensors.  Title to the Material remains with Intel
# Corporation or its suppliers and licensors.  The Material is protected
# by worldwide copyright laws and treaty provisions.  No part of the
# Material may be used, copied, reproduced, modified, published, uploaded,
# posted, transmitted, distributed, or disclosed in any way without
# Intel's prior express written permission.
#
# No license under any patent, copyright, trade secret or other
# intellectual property right is granted to or conferred upon you by
# disclosure or delivery of the Materials, either expressly, by
# implication, inducement, estoppel or otherwise.  Any license under such
# intellectual property rights must be express and approved by Intel in
# writing.

# Common Makefile that builds and runs example.
PROG=convex_hull_bench
ARGS=

# The C++ compiler options
CXX = cl.exe
MYCXXFLAGS = /TP /EHsc /W3 /nologo $(TBB_SECURITY_SWITCH) /D _CONSOLE /D _MBCS /D WIN32 $(CXXFLAGS)
MYLDFLAGS =/INCREMENTAL:NO /NOLOGO /DEBUG /FIXED:NO $(LDFLAGS)

all: release test
release:
	$(CXX) convex_hull_sample.cpp /MD /O2 /D NDEBUG $(MYCXXFLAGS) /link tbb.lib $(LIBS) $(MYLDFLAGS) /OUT:convex_hull_sample.exe
	$(CXX) convex_hull_bench.cpp /MD /O2 /D NDEBUG $(MYCXXFLAGS) /link tbb.lib $(LIBS) $(MYLDFLAGS) /OUT:convex_hull_bench.exe
debug:
	$(CXX) convex_hull_sample.cpp /MDd /Od /Zi /D TBB_USE_DEBUG /D _DEBUG $(MYCXXFLAGS) /link tbb_debug.lib $(LIBS) $(MYLDFLAGS) /OUT:convex_hull_sample.exe
	$(CXX) convex_hull_bench.cpp /MDd /Od /Zi /D TBB_USE_DEBUG /D _DEBUG $(MYCXXFLAGS) /link tbb_debug.lib $(LIBS) $(MYLDFLAGS) /OUT:convex_hull_bench.exe
clean:
	@cmd.exe /C del convex_hull*.exe *.obj *.?db *.manifest
test:
	$(PROG) $(ARGS)
