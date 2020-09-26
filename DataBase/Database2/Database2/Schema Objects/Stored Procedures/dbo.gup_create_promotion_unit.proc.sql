CREATE PROCEDURE [dbo].[gup_create_promotion_unit]
	@strUserID	nvarchar(16)
,	@iOK [int] = 0
AS

SET NOCOUNT ON;
SET XACT_ABORT ON;

BEGIN TRAN

IF NOT EXISTS (SELECT * FROM ACCOUNT.DBO.MUSER WHERE USERID = @strUserID)
	BEGIN	SELECT @iOK = -1 GOTO FAIL_TRAN	END

DECLARE	@strNick1	nvarchar(16)
,		@strNick2	nvarchar(16)
,		@strNick3	nvarchar(16)
,		@strNick4	nvarchar(16)
,		@strNick5	nvarchar(16)
,		@iUserUID	bigint
,		@iUnitUID1	bigint
,		@iUnitUID2	bigint
,		@iUnitUID3	bigint
,		@iUnitUID4	bigint
,		@iUnitUID5	bigint
,		@sdtNow		smalldatetime

SELECT	@strNick1 = @strUserID + '1'
,		@strNick2 = @strUserID + '2'
,		@strNick3 = @strUserID + '3'	
,		@strNick4 = @strUserID + '4'	
,		@strNick5 = @strUserID + '5'	
,		@sdtNow = getdate()

SELECT @iUserUID = UserUID	
	FROM ACCOUNT.DBO.MUSER
		WHERE USERID = @strUserID

IF NOT EXISTS (SELECT * FROM dbo.GUser WHERE UserUID = @iUserUID)
	BEGIN
		INSERT INTO dbo.GUser (UserUID, UserID,USSize, RegDate, DelDate)
			SELECT @iUserUID, @strUserID, 6, @sdtNow, @sdtNow
	END

IF EXISTS (SELECT * FROM DBO.GUnitNickname WHERE Nickname in (@strNick1,@strNick2,@strNick3,@strNick4,@strNick5))
	BEGIN	SELECT @iOK = -2 GOTO FAIL_TRAN	END	


-- 유닛 생성
EXEC [dbo].[gup_create_unit] @iUserUID, @strNick1, 1, 0
EXEC [dbo].[gup_create_unit] @iUserUID, @strNick2, 2, 0
EXEC [dbo].[gup_create_unit] @iUserUID, @strNick3, 3, 0
EXEC [dbo].[gup_create_unit] @iUserUID, @strNick4, 4, 0
EXEC [dbo].[gup_create_unit] @iUserUID, @strNick5, 5, 0

-- 유닛 UID 추출
SELECT	@iUnitUID1	= UnitUID	FROM dbo.GUnitNickName	WHERE Nickname = @strNick1
SELECT	@iUnitUID2	= UnitUID	FROM dbo.GUnitNickName	WHERE Nickname = @strNick2
SELECT	@iUnitUID3	= UnitUID	FROM dbo.GUnitNickName	WHERE Nickname = @strNick3
SELECT	@iUnitUID4	= UnitUID	FROM dbo.GUnitNickName	WHERE Nickname = @strNick4
SELECT	@iUnitUID5	= UnitUID	FROM dbo.GUnitNickName	WHERE Nickname = @strNick5



-- 유닛 1 프로모션 의상 
EXEC [dbo].[gup_insert_item] @iUnitUID1, 128000, 0, 0, 0, 0
EXEC [dbo].[gup_insert_item] @iUnitUID1, 128001, 0, 0, 0, 0
EXEC [dbo].[gup_insert_item] @iUnitUID1, 128002, 0, 0, 0, 0
EXEC [dbo].[gup_insert_item] @iUnitUID1, 128003, 0, 0, 0, 0
EXEC [dbo].[gup_insert_item] @iUnitUID1, 128004, 0, 0, 0, 0

-- 유닛 2 프로모션 의상 
EXEC [dbo].[gup_insert_item] @iUnitUID2, 128010, 0, 0, 0, 0
EXEC [dbo].[gup_insert_item] @iUnitUID2, 128011, 0, 0, 0, 0
EXEC [dbo].[gup_insert_item] @iUnitUID2, 128012, 0, 0, 0, 0
EXEC [dbo].[gup_insert_item] @iUnitUID2, 128013, 0, 0, 0, 0
EXEC [dbo].[gup_insert_item] @iUnitUID2, 128014, 0, 0, 0, 0

-- 유닛 3 프로모션 의상 
EXEC [dbo].[gup_insert_item] @iUnitUID3, 128005, 0, 0, 0, 0   
EXEC [dbo].[gup_insert_item] @iUnitUID3, 128006, 0, 0, 0, 0   
EXEC [dbo].[gup_insert_item] @iUnitUID3, 128007, 0, 0, 0, 0   
EXEC [dbo].[gup_insert_item] @iUnitUID3, 128008, 0, 0, 0, 0   
EXEC [dbo].[gup_insert_item] @iUnitUID3, 128009, 0, 0, 0, 0   

-- 유닛 4 프로모션 의상 
EXEC [dbo].[gup_insert_item] @iUnitUID4, 128072, 0, 0, 0, 0   
EXEC [dbo].[gup_insert_item] @iUnitUID4, 128073, 0, 0, 0, 0   
EXEC [dbo].[gup_insert_item] @iUnitUID4, 128074, 0, 0, 0, 0   
EXEC [dbo].[gup_insert_item] @iUnitUID4, 128075, 0, 0, 0, 0   
EXEC [dbo].[gup_insert_item] @iUnitUID4, 128076, 0, 0, 0, 0   

-- 유닛 5 프로모션 의상 
EXEC [dbo].[gup_insert_item] @iUnitUID5, 130134, 0, 0, 0, 0   
EXEC [dbo].[gup_insert_item] @iUnitUID5, 130135, 0, 0, 0, 0   
EXEC [dbo].[gup_insert_item] @iUnitUID5, 130136, 0, 0, 0, 0   
EXEC [dbo].[gup_insert_item] @iUnitUID5, 130137, 0, 0, 0, 0   
EXEC [dbo].[gup_insert_item] @iUnitUID5, 130138, 0, 0, 0, 0   



-- 유닛 1 던전 클리어
EXEC [dbo].[gup_all_dungeonclear] @iUnitUID1

 -- 유닛 2 던전 클리어    
EXEC [dbo].[gup_all_dungeonclear] @iUnitUID2
                                                                  
 -- 유닛 3 던전 클리어    
EXEC [dbo].[gup_all_dungeonclear] @iUnitUID3

 -- 유닛 4 던전 클리어  
EXEC [dbo].[gup_all_dungeonclear] @iUnitUID4

 -- 유닛 5 던전 클리어  
EXEC [dbo].[gup_all_dungeonclear] @iUnitUID5


-- 유닛 기본 능력 세팅
UPDATE dbo.GUnit
	SET	[EXP] = 6500000
,		Gamepoint = 50000000
,		spoint = 801
		WHERE UnitUID in (@iUnitUID1, @iUnitUID2, @iUnitUID3, @iUnitUID4, @iUnitUID5)


-- 프로모션 무기 입히기	
UPDATE dbo.GItem
	SET InventoryCategory = 9
,		SlotID = 10
		WHERE UnitUID in (@iUnitUID1, @iUnitUID2, @iUnitUID3, @iUnitUID4, @iUnitUID5)
			AND ItemID in (128000,128010, 128005, 128072, 130134)

-- 프로모션 상의 입히기
UPDATE dbo.GItem
	SET InventoryCategory = 9
,		SlotID = 2
		WHERE UnitUID in (@iUnitUID1, @iUnitUID2, @iUnitUID3, @iUnitUID4, @iUnitUID5)
			AND ItemID in (128001, 128011, 128006, 128073, 130135)

-- 프로모션 하의 입히기
UPDATE dbo.GItem
	SET InventoryCategory = 9
,		SlotID = 4
		WHERE UnitUID in (@iUnitUID1, @iUnitUID2, @iUnitUID3, @iUnitUID4, @iUnitUID5)
			AND ItemID in (128002, 128012, 128007, 128074, 130136)

-- 프로모션 장갑 입히기
UPDATE dbo.GItem
	SET InventoryCategory = 9
,		SlotID = 6
		WHERE UnitUID in (@iUnitUID1, @iUnitUID2, @iUnitUID3, @iUnitUID4, @iUnitUID5)
			AND ItemID in (128003, 128013, 128008, 128075, 130137)

-- 프로모션 신발 입히기
UPDATE dbo.GItem
	SET InventoryCategory = 9
,		SlotID = 8
		WHERE UnitUID in (@iUnitUID1, @iUnitUID2, @iUnitUID3, @iUnitUID4, @iUnitUID5)
			AND ItemID in (128004, 128014, 128009, 128076, 130138)



COMMIT TRAN

GOTO END_PROC

FAIL_TRAN:
ROLLBACK TRAN

END_PROC:
SELECT @iOK, @strUserID


