CREATE PROCEDURE [dbo].[up_indigo_guarantee_record]
	@strLogin_ [nvarchar](20),
	@iLoginUID_ [int]
AS
SET NOCOUNT ON

DECLARE @iLoginUID int
SET @iLoginUID = -1

-- 데이터 존재 확인
IF NOT EXISTS( SELECT * FROM dbo.IndigoUser( NOLOCK ) WHERE LoginUID = @iLoginUID_ )
BEGIN
    -- 없다면 바로 추가한다. Users에 존재하는 Login인지 확인
    SELECT @iLoginUID = LoginUID FROM dbo.Users(NOLOCK) WHERE Login = @strLogin_
    IF @iLoginUID = -1
    BEGIN
        SELECT -1   -- Users에 존재하지 않는 계정
        RETURN
    END
    
    IF @iLoginUID <> @iLoginUID_
    BEGIN
        SELECT -2   -- 잘못된 LoginUID가 입력됨. DB의 값과 다름
        RETURN
    END

    -- 데이터 추가
    INSERT INTO dbo.IndigoUser(LoginUID) VALUES( @iLoginUID_ )
    IF @@ERROR <> 0
    BEGIN
        SELECT -3   -- 레코드 추가 (insert) 실패
        RETURN
    END
END

SELECT 0, RP, Win, Lose, Rank FROM dbo.IndigoUser( NOLOCK ) WHERE LoginUID = @iLoginUID_


