#pragma once

#if defined(PATCHER_COUNTRY_DE)
	#include "PatcherStringSet\PatcherStringSet_DE.h"
#elif defined(PATCHER_COUNTRY_FR)
	#include "PatcherStringSet\PatcherStringSet_FR.h"
#elif defined(PATCHER_COUNTRY_IT)
	#include "PatcherStringSet\PatcherStringSet_IT.h"
#elif defined(PATCHER_COUNTRY_PL)
	#include "PatcherStringSet\PatcherStringSet_PL.h"
#elif defined(PATCHER_COUNTRY_ES)
	#include "PatcherStringSet\PatcherStringSet_ES.h"
#elif defined(PATCHER_COUNTRY_EN)
	#include "PatcherStringSet\PatcherStringSet_EN.h"
#elif defined(PATCHER_COUNTRY_US)
	#include "PatcherStringSet\PatcherStringSet_US.h"
#elif defined(PATCHER_COUNTRY_JP)
	#include "PatcherStringSet\PatcherStringSet_US.h"
#elif defined(PATCHER_COUNTRY_ID)
	#include "PatcherStringSet\PatcherStringSet_ID.h"
#elif defined(PATCHER_COUNTRY_TH)
	#include "PatcherStringSet\PatcherStringSet_TH.h"
#elif defined(PATCHER_COUNTRY_BR)
	#include "PatcherStringSet\PatcherStringSet_BR.h"
#elif defined(PATCHER_COUNTRY_PH)
	#include "PatcherStringSet\PatcherStringSet_PH.h"
#elif defined(PATCHER_COUNTRY_TW) || defined(PATCHER_COUNTRY_HK)
	#include "PatcherStringSet\PatcherStringSet_TW.h"
#else
	#include "PatcherStringSet\PatcherStringSet_US.h"
#endif