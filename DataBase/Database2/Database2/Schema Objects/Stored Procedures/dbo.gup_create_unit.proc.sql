CREATE PROCEDURE [dbo].[gup_create_unit] 
	@iUserUID_ [bigint],
	@strNickName_ [nvarchar](16),
	@iUnitClass_ [tinyint],
	@iOK [int] = 0,
	@iUnitUID [bigint] = 0
AS
SET NOCOUNT ON;

DECLARE @bDeleted bit,
        @iUsSize int,
        @sdtNow smalldatetime
,		@iStartSpirit smallint
--	수정 1
,		@sdtNickDate smalldatetime

SELECT  @sdtNow = GetDate()

SELECT  @iStartSpirit = StartSpirit
	FROM dbo.GResurrectionStoneCnt WITH (NOLOCK)

-- 계정의 삭제여부 및 캐릭터 슬롯 개수 정보 얻기.
SELECT @bDeleted = Deleted, @iUsSize = USSize FROM dbo.GUser WITH(NOLOCK) WHERE UserUID = @iUserUID_

IF @bDeleted = 1    -- 해당 UserUID가 이미 삭제된 경우
    BEGIN    SELECT @iOK = -1    GOTO END_PROC    END

-- 현재 보유한 캐릭터 수가 이미 제한에 걸린 경우
if (select count(*) from dbo.gunit(NOLOCK) where deleted = 0 and UserUID = @iUserUID_) >= @iUsSize
    BEGIN    SELECT @iOK = -3    GOTO END_PROC    END

-- 일일 최대 생성수에 걸리는 경우
--IF ((select count(*) from dbo.gunit with (nolock) where useruid = @iUserUID_ and regdate between convert(nvarchar(10), getdate(), 120)+' 00:00:00' and convert(nvarchar(10), getdate(), 120)+' 23:59:00') = 4)
--	BEGIN	SELECT @iOK = -23    GOTO END_PROC    END
--

-- 동일한 닉네임이 존재하는 경우
IF EXISTS(SELECT * FROM dbo.GUnitNickName WITH(NOLOCK) WHERE NickName = @strNickName_)
    BEGIN    SELECT @iOK = -2    GOTO END_PROC    END

-- 삭제된 닉네임 중 해당 닉네임이 존재하는 경우
--	수정 1
IF EXISTS(SELECT * FROM dbo.GDeletedNickNameHistory with (nolock) WHERE NickName = @strNickName_)
    BEGIN    
		SELECT TOP 1 @sdtNickDate = RegDate FROM GDeletedNickNameHistory WHERE NickName = @strNickName_ ORDER BY RegDate DESC
		IF (DATEADD(DD,-14,@sdtNow) < @sdtNickDate)
			BEGIN	SELECT @iOK = -222  SELECT @sdtNickDate = DATEADD(DD,14,@sdtNickDate) GOTO END_PROC	END
	END


BEGIN TRAN

    -- unit 생성.
    INSERT INTO dbo.GUnit( UserUID, UnitClass, [Exp], [Level], GamePoint, VSPoint, VSPointMax, BaseHP,
        AtkPhysic, AtkMagic, DefPhysic, DefMagic, SPoint, Win, Lose, Seceder, RegDate, DelDate, LastPosition  )
        SELECT  @iUserUID_, @iUnitClass_, 0, 1, 0, 0, 0, 0, 
                0, 0, 0, 0, 1, 0, 0, 0, @sdtNow, @sdtNow, 20000
		    
	IF @@ERROR <> 0 OR @@ROWCOUNT <> 1
    BEGIN    
        SELECT @iOK = -12    
        GOTO FAIL_TRAN    
    END

    SELECT @iUnitUID = @@IDENTITY

    INSERT INTO dbo.GUnitNickName( UnitUID, NickName, RegDate )
        SELECT  @iUnitUID, @strNickName_, @sdtNow
    IF @@ERROR <> 0 OR @@ROWCOUNT <> 1
    BEGIN    
        SELECT @iOK = -13    
        GOTO FAIL_TRAN
    END


    -- 커뮤니티 옵션 생성
	INSERT INTO dbo.GDenyOption( UnitUID, QuestionNo, CodeNo )
        SELECT  @iUnitUID, 1, 1
    IF @@ERROR <> 0 OR @@ROWCOUNT <> 1
    BEGIN    
        SELECT @iOK = -14
        GOTO FAIL_TRAN
    END

	INSERT INTO dbo.GDenyOption( UnitUID, QuestionNo, CodeNo )
        SELECT  @iUnitUID, 2, 1
    IF @@ERROR <> 0 OR @@ROWCOUNT <> 1
    BEGIN    
        SELECT @iOK = -14
        GOTO FAIL_TRAN
    END

	INSERT INTO dbo.GDenyOption( UnitUID, QuestionNo, CodeNo )
        SELECT  @iUnitUID, 3, 1
    IF @@ERROR <> 0 OR @@ROWCOUNT <> 1
    BEGIN    
        SELECT @iOK = -14
        GOTO FAIL_TRAN
    END

	INSERT INTO dbo.GDenyOption( UnitUID, QuestionNo, CodeNo )
        SELECT  @iUnitUID, 4, 1
    IF @@ERROR <> 0 OR @@ROWCOUNT <> 1
    BEGIN    
        SELECT @iOK = -14
        GOTO FAIL_TRAN
    END

	-- [[기본 스킬 셋팅]]

	-- 로우의 호출 기본 퀘스트 넣기
	INSERT INTO dbo.GQuests (UnitUID, QuestID, SubQuest0, SubQuest1, SubQuest2, SubQuest3, SubQuest4, RegDate)
		SELECT @iUnitUID, 13, 1,0,0,0,0,GETDATE()

	--엘소드
	IF @iUnitClass_ = 1
	BEGIN
		INSERT INTO dbo.GSkill( UnitUID, SkillID, RegDate )
        SELECT  @iUnitUID, 10000, @sdtNow
				IF @@ERROR <> 0 OR @@ROWCOUNT <> 1
					BEGIN    SELECT @iOK = -16    GOTO FAIL_TRAN    END

		INSERT INTO dbo.GSkillSlot( UnitUID, Slot01, Slot02, Slot03 )
					SELECT  @iUnitUID, 10000, 0, 0
				IF @@ERROR <> 0 OR @@ROWCOUNT <> 1
					BEGIN    SELECT @iOK = -15    GOTO FAIL_TRAN    END
	END

	--아아샤
	IF @iUnitClass_ = 2
	BEGIN
		INSERT INTO dbo.GSkill( UnitUID, SkillID, RegDate )
        SELECT  @iUnitUID, 20030, @sdtNow
				IF @@ERROR <> 0 OR @@ROWCOUNT <> 1
					BEGIN    SELECT @iOK = -18    GOTO FAIL_TRAN    END

		INSERT INTO dbo.GSkillSlot( UnitUID, Slot01, Slot02, Slot03 )
					SELECT  @iUnitUID, 20030, 0, 0
				IF @@ERROR <> 0 OR @@ROWCOUNT <> 1
					BEGIN    SELECT @iOK = -17    GOTO FAIL_TRAN    END
	END

	--레나
	IF @iUnitClass_ = 3
	BEGIN
		INSERT INTO dbo.GSkill( UnitUID, SkillID, RegDate )
        SELECT  @iUnitUID, 30000, @sdtNow
				IF @@ERROR <> 0 OR @@ROWCOUNT <> 1
					BEGIN    SELECT @iOK = -20    GOTO FAIL_TRAN    END

		INSERT INTO dbo.GSkillSlot( UnitUID, Slot01, Slot02, Slot03 )
					SELECT  @iUnitUID, 30000, 0, 0
				IF @@ERROR <> 0 OR @@ROWCOUNT <> 1
					BEGIN    SELECT @iOK = -19    GOTO FAIL_TRAN    END
	END

	--레이븐
	IF @iUnitClass_ = 4
	BEGIN
		INSERT INTO dbo.GSkill( UnitUID, SkillID, RegDate )
        SELECT  @iUnitUID, 40010, @sdtNow
				IF @@ERROR <> 0 OR @@ROWCOUNT <> 1
					BEGIN    SELECT @iOK = -20    GOTO FAIL_TRAN    END

		INSERT INTO dbo.GSkillSlot( UnitUID, Slot01, Slot02, Slot03 )
					SELECT  @iUnitUID, 40010, 0, 0
				IF @@ERROR <> 0 OR @@ROWCOUNT <> 1
					BEGIN    SELECT @iOK = -19    GOTO FAIL_TRAN    END
	END


	-- 근성도 
	INSERT INTO dbo.GSpirit (UnitUID, Spirit, RegDate)
		SELECT @iUnitUID, @iStartSpirit, @sdtNow
			IF @@ERROR <> 0 OR @@ROWCOUNT <> 1
					BEGIN    SELECT @iOK = -21    GOTO FAIL_TRAN    END

COMMIT TRAN

GOTO END_PROC

FAIL_TRAN:
ROLLBACK TRAN

END_PROC:
SELECT @iOK, @iUnitUID, ISNULL(@sdtNickDate,'2000-01-01 00:00')


