CREATE PROCEDURE [dbo].[gup_new_blacklist_user]
	@iUnitUID_ [bigint],	
	@strNickName_ [nvarchar](16),
	@iOK [int] = 0
AS
SET NOCOUNT ON;

--존재하는 닉네임인지 확인
DECLARE @iBLUnitUID_ bigint

SET @iBLUnitUID_ = (SELECT UnitUID FROM dbo.GUnitNickName WHERE NickName = @strNickName_ )
    IF @iBLUnitUID_ IS NULL
        BEGIN    SELECT @iOK = -10    GOTO END_PROC    END

--이미 등록된 유저인지 체크
IF EXISTS(SELECT * FROM dbo.GFriendList WITH(NOLOCK) WHERE UnitUID1 = @iUnitUID_ AND UnitUID2 = @iBLUnitUID_ AND Type = 4)
    BEGIN    SELECT @iOK = -1    GOTO END_PROC    END

DECLARE @sdtNow smalldatetime
SELECT  @sdtNow = GetDate()

BEGIN TRAN

--블랙리스트 유저 UnitUID 등록
    INSERT INTO dbo.GFriendList(
                               UnitUID1
,						       UnitUID2
,                              RegDate
,						       Type
                              )
        SELECT  @iUnitUID_
,				@iBLUnitUID_
,               @sdtNow
,				4						 -- 임시로 4로 넣음 : 4는 블랙리스트 타입

    IF @@ERROR <> 0 OR @@ROWCOUNT <> 1
        BEGIN    SELECT @iOK = -11    GOTO FAIL_TRAN    END

COMMIT TRAN

GOTO END_PROC

FAIL_TRAN:
ROLLBACK TRAN

END_PROC:
SELECT @iOK, @iBLUnitUID_


