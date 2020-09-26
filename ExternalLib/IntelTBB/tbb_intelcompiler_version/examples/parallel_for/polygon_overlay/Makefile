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

# Just specify your program basename
PROG=Pover
ARGS=

# The C++ compiler options
CXX = cl.exe

# Uncomment one of next lines to choose user interface type (console, gdiplus, direct draw)
#UI = con
UI = gdi
#UI = dd

# Machine architecture, auto-detected from TBB_ARCH_PLATFORM by default
# Use XARCH variable to change it. See index.html for more information
ARCH0 = $(TBB_ARCH_PLATFORM)-
ARCHA = $(ARCH0:\vc7.1-=)
ARCHB = $(ARCHA:\vc8-=)
ARCHC = $(ARCHB:\vc9-=)
ARCH3 = $(ARCHC:ia32=x86)
ARCH4 = $(ARCH3:intel64=AMD64)
XARCH = $(ARCH4:-=x86)

MAKEINC = ../../common/gui/Makefile.win

all: release test
release:
	@$(MAKE) -f $(MAKEINC) UI=$(UI) CXX="$(CXX)" CXXFLAGS="$(CXXFLAGS)" LFLAGS="$(LDFLAGS) tbb.lib tbbmalloc.lib $(LIBS)" XARCH=$(XARCH) RCNAME=pover SOURCE=*.cpp EXE=$(PROG).exe build_one
debug:
	@$(MAKE) -f $(MAKEINC) UI=$(UI) DEBUG=_debug CXX="$(CXX)" CXXFLAGS="$(CXXFLAGS) /D TBB_USE_DEBUG" LFLAGS="$(LDFLAGS) tbb_debug.lib tbbmalloc_debug.lib $(LIBS)" XARCH=$(XARCH) RCNAME=pover SOURCE=*.cpp EXE=$(PROG).exe build_one
clean:
	@cmd.exe /C del $(PROG).exe *.obj *.?db *.manifest vc7.1\pover.res
test:
	$(PROG) $(ARGS)
