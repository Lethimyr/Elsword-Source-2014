#!/bin/sh
#
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

TBB22_INSTALL_DIR="D:\Program Files\Intel\Compiler\11.1\067\tbb"; export TBB22_INSTALL_DIR
TBB_ARCH_PLATFORM="intel64/vc8"
if [ -z "${PATH}" ]; then
    PATH="${TBB22_INSTALL_DIR}/${TBB_ARCH_PLATFORM}/bin"; export PATH
else
    PATH="${TBB22_INSTALL_DIR}/${TBB_ARCH_PLATFORM}/bin;$PATH"; export PATH
fi
if [ -z "${LIB}" ]; then
    LIB="${TBB22_INSTALL_DIR}/${TBB_ARCH_PLATFORM}/lib"; export LIB
else
    LIB="${TBB22_INSTALL_DIR}/${TBB_ARCH_PLATFORM}/lib;$LIB"; export LIB
fi
if [ -z "${INCLUDE}" ]; then
    INCLUDE="${TBB22_INSTALL_DIR}/include"; export INCLUDE
else
    INCLUDE="${TBB22_INSTALL_DIR}/include;$INCLUDE"; export INCLUDE
fi
