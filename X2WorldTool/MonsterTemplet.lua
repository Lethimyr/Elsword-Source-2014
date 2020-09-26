		
		
-- MONSTER0 부터 시작하기		
		
		
		

		
		
		
MONSTER0 = 
{
	NAME					= "SKELETON",
	MOTION_FILE_NAME		= "Motion_Skeleton.x",
	--MOTION_ANI_TEX_XET		
	--MOTION_CHANGE_TEX_XET;

	--USE_SLASH_TRACE;
	--DRAW_SMALL_MP_BAR;


	STATE_WIN				= FALSE,
	STATE_WAITHABIT			= TRUE,
	STATE_WAITSTART			= TRUE,
	STATE_JUMP_READY		= FALSE,
	STATE_DASH				= TRUE,
	STATE_DASH_END			= TRUE,
		
}





-- 주석안에 넣어주세요~
--[[

INIT_SYSTEM = 
{
	UNIT_WIDTH		= 70.0,
	UNIT_HEIGHT		= 200.0,
	UNIT_LAYER		= X2_LAYER["XL_UNIT_0"],
}




INIT_DEVICE = 
{
	READY_TEXTURE = 
	{
		"WhitePoint.dds",
		"SlashLight.dds",
		"WhiteCircle.dds",
		"WhitePointSmall.dds",
		"Star.dds",
		"Damage_Ling.dds",
		"Damage_Line.dds",
		"Damage_Center.dds",
		"smoke.dds",
		"ColorBallBlue.dds",
		"GroundShockWave.dds",
		"ToKang.dds",
		"DropDown.tga",
		"stone.dds",
		"EnergyBall.dds",
		"MagicSquare.tga",
		"Energy_Violet.dds",
		"Ex_mark.dds",
		"Arme_FireballB2.dds",
		"Arme_Ring2.dds",
		"Arme_Critical2.dds",
		"ColorBallFire.dds",
		"MeteoCallFace.tga",
		"MeteoCallTalkBoxRight.tga",
		"MeteoCallTalkBoxLeft.tga",
		"Arme_SpecialAttack3a-1.dds",
		"EnergyBallRed.dds",
		"Ring.dds",
		"RingAlpha.dds",
		"Arme_Damage_Center.dds",
		"star1.dds",
		"star2.dds",
		"Arme_Critical.dds",
		"Arme_Damage_Ring.dds",
		"Arme_SpecialAttack1.dds",
		"Focusline.dds",
		"TimeStopFace.tga",
		"TimeStopTalkBoxLeft.tga",
		"TimeStopTalkBoxRight.tga",
		"LightningBoltFace.tga",
		"LightningBoltTalkBoxLeft.tga",
		"LightningBoltTalkBoxRight.tga",
	},
	
	READY_SOUND = 
	{
		"Slash.wav",			
		"PowerSlash.wav",		
		"HitSlash.wav",		
		"Hit.wav",	
		"swordcrash.wav",		
		"RevengeReady.wav",	
		"Step.wav",				
		"Energy.wav",			
		"Charge.wav",			
		"Break.wav",			
		"wind.wav",			
		"Change.wav",			
		"fire.wav",			
		"Fight.wav",			
		"Win.wav",			
		"Lose.wav",
		"Sound_Arme_Start1.wav",
		"Sound_Arme_Shout1.wav",
		"Sound_Arme_Shout2.wav",
		"Sound_Arme_Shout3.wav",
	},
			
	READY_XMESH = 
	{

	},
	
	READY_XSKIN_MESH = 
	{

	},
}


INIT_PHYSIC = 
{
	RELOAD_ACCEL		= 2000,
	G_ACCEL				= 4000,
	MAX_G_SPEED			= -2000,
	
	WALK_SPEED			= 600,
	RUN_SPEED			= 800,
	JUMP_SPEED			= 1500,
	DASH_JUMP_SPEED		= 1800,
}


INIT_COMPONENT = 
{
	MAX_HP				= 10000,
	MP_CHANGE_RATE		= 1,
	MP_CHARGE_RATE		= 130,
	
	
	SHADOW_SIZE			= 200,
	SHADOW_FILE_NAME	= "shadow.tga",
	
	SMALL_HP_BAR_BLUE	= "Small_HP_bar_Blue.TGA",
	SMALL_HP_BAR_RED	= "Small_HP_bar_Red.TGA",
	SMALL_HP_BAR_YELLOW = "Small_HP_bar_Yellow.TGA",
	--DRAW_SMALL_MP_BAR	= TRUE,
	
	QUESTION_MARK_SEQ		= "QuestionMarkNPC",
	EXCLAMATION_MARK_SEQ	= "ExclamationMarkNPC",
	--MIND_FLAG_HEIGHT		= 230,
	
	--HYPER_MODE_COUNT		= 0,
	--MAX_HYPER_MODE_TIME	= 30,
	
	--RAGE_COUNT_MAX	= 20,
	--RAGE_TIME_MAX		= 5,

	HITTED_TYPE			= HITTED_TYPE["HTD_MEAT"],
	FALL_DOWN			= TRUE,
	
	--USE_SLASH_TRACE	= TRUE,
	--WEAPON_FILE_NAME	= "Mesh_LizardMan_Older_Weapon.X",
	--WEAPON_BONE_NAME	= "Dummy1_Rhand",
	
	--BOSS_GAGE_FACE_TEX		= "DLG_BossState.tga",
	--BOSS_GAGE_FACE_TEX_PIECE	= "NUI_PPORU_GIANT_RED",

	
	STAT =
	{
		ATK_PHYSIC		= 100,
		ATK_MAGIC		= 0,
		ATK_FIRE		= 0,
		ATK_ICE			= 0,
		ATK_EARTH		= 0,
		ATK_LIGHTNING	= 0,
		ATK_DARK		= 0,
		ATK_LIGHT		= 0,
		ATK_UNIVERSAL	= 0,

		DEF_PHYSIC		= 50,
		DEF_MAGIC		= 0,
		DEF_FIRE		= 0,
		DEF_ICE			= 0,
		DEF_EARTH		= 0,
		DEF_LIGHTNING	= 0,
		DEF_DARK		= 0,
		DEF_LIGHT		= 0,
		DEF_UNIVERSAL	= 0,
	},
	
	HARD_LEVEL = 
	{
        EXP             = 10,
		HP				= 250,
		
		ATK_PHYSIC		= 50,
		ATK_MAGIC		= 0,
		ATK_FIRE		= 0,
		ATK_ICE			= 0,
		ATK_EARTH		= 0,
		ATK_LIGHTNING	= 0,
		ATK_DARK		= 0,
		ATK_LIGHT		= 0,
		ATK_UNIVERSAL	= 0,

		DEF_PHYSIC		= 0,
		DEF_MAGIC		= 0,
		DEF_FIRE		= 0,
		DEF_ICE			= 0,
		DEF_EARTH		= 0,
		DEF_LIGHTNING	= 0,
		DEF_DARK		= 0,
		DEF_LIGHT		= 0,
		DEF_UNIVERSAL	= 0,
		
		TRIGGER_RATE	= 0.5,
	},
}






INIT_AI = 
{
	TARGET = 
	{
		TARGET_PRIORITY 			= TARGET_PRIORITY["TP_NEAR_FIRST"],
		TARGET_INTERVAL				= 3,		-- sec
		TARGET_NEAR_RANGE			= 800,		-- 이 거리보다 가까우면 TARGET_SUCCESS_RATE에 관계없이 무조건 타게팅된다
		TARGET_RANGE				= 1500,		-- cm
		TARGET_LOST_RANGE			= 2000,		-- cm
		TARGET_SUCCESS_RATE			= 80,		-- %
		ATTACK_TARGET_RATE			= 80,		-- 나를 공격한 유닛을 타게팅할 확률
		PRESERVE_LAST_TARGET_RATE	= 60,		-- 이전에 타게팅된 유닛을 계속 타게팅할 확률
	},

	CHASE_MOVE = 
	{		
		MOVE_SPLIT_RANGE	= 600,
		DEST_GAP			= 150,	-- 목적지에서 이 거리 안에 있으면 도착했다고 판단한다
		MOVE_GAP			= 160,
		
		DIR_CHANGE_INTERVAL = 0.7,
		
		WALK_INTERVAL		= 3,
		NEAR_WALK_RATE		= 70,
		FAR_WALK_RATE		= 30,
		
		JUMP_INTERVAL		= 5,
		UP_JUMP_RATE		= 40,
		UP_DOWN_RATE		= 20,
		DOWN_JUMP_RATE		= 20,
		DOWN_DOWN_RATE		= 40,
		
		LINE_END_RANGE		= 80,	-- cm
	},	
	
	PATROL_MOVE = 	
	{
		PATROL_BEGIN_RATE		= 50,		
		PATROL_RANGE			= 500,
		PATROL_COOL_TIME		= 2,
		ONLY_THIS_LINE_GROUP	= TRUE,
	},
	
	ESCAPE_MOVE = 
	{		
		MOVE_SPLIT_RANGE	= 500,	-- cm
		ESCAPE_GAP			= 10,	-- 이 거리 보다 멀어지면 도망 성공
		
		WALK_INTERVAL		= 1.5,	-- 초
		NEAR_WALK_RATE		= 10,
		FAR_WALK_RATE		= 10,
		
		JUMP_INTERVAL		= 10,
		UP_JUMP_RATE		= 30,
		UP_DOWN_RATE		= 30,
		DOWN_JUMP_RATE		= 30,
		DOWN_DOWN_RATE		= 30,
		
		LINE_END_RANGE		= 80,	-- cm
	},
	
	ESCAPE_CONDITION = 
	{
	}
}



--]]
---------------------------------------------------------------------------------