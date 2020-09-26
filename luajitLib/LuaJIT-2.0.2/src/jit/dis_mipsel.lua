-- lua header. UTF-8 인코딩 인식을 위해 이 줄은 지우지 마세요.
----------------------------------------------------------------------------
-- LuaJIT MIPSEL disassembler wrapper module.
--
-- Copyright (C) 2005-2013 Mike Pall. All rights reserved.
-- Released under the MIT license. See Copyright Notice in luajit.h
----------------------------------------------------------------------------
-- This module just exports the little-endian functions from the
-- MIPS disassembler module. All the interesting stuff is there.
------------------------------------------------------------------------------

local require = require

module(...)

local dis_mips = require(_PACKAGE.."dis_mips")

create = dis_mips.create_el
disass = dis_mips.disass_el
regname = dis_mips.regname

