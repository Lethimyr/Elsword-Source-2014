CREATE PROCEDURE [dbo].[fhup_invite_temp_update]
	@iLoginUIDA_ [int],
	@strNick_ [nvarchar](24) = NULL,
	@iOK [int] = 0
AS
/*microcat
작성일 : 2007년 4월 19일
사용 : 추천받은 님을 기록
*/
SET NOCOUNT ON;

EXEC @iOK = dbo.fhup_invite_available
IF @iOK = -1
    BEGIN    GOTO END_PROC    END

DECLARE @iLoginUIDB int
SELECT  @iLoginUIDB = a.LoginUID
    FROM dbo.Users as a WITH (NOLOCK)
        WHERE EXISTS(SELECT * FROM dbo.NickNames as b WITH (NOLOCK) WHERE a.Login = b.Login AND b.Nick = @strNick_)

if @iLoginUIDB is not null
    begin
        DECLARE @bNew bit
        SELECT  @bNew = 1
        IF EXISTS(SELECT * FROM dbo.FHInviteUser WITH (NOLOCK) WHERE LoginUIDB = @iLoginUIDB)
            BEGIN    SELECT @bNew = 0    END
    end

BEGIN TRAN
    UPDATE dbo.FHInviteTemp WITH (ROWLOCK)
        SET Invited = 1
,           LoginUIDB = @iLoginUIDB
            WHERE LoginUIDA = @iLoginUIDA_
    IF @@ERROR <> 0 OR @@ROWCOUNT <> 1
        BEGIN    SELECT @iOK = -11    GOTO FAIL_TRAN    END

    if @iLoginUIDB is not null
        begin
            IF @bNew = 0
                BEGIN
                    UPDATE dbo.FHInviteUser WITH (ROWLOCK)
                        SET RCount = CASE when RCount + 1 > 20 then 20 else RCount + 1 end
                            WHERE LoginUIDB = @iLoginUIDB
                    IF @@ERROR <> 0 OR @@ROWCOUNT <> 1
                        BEGIN    SELECT @iOK = -12    GOTO FAIL_TRAN    END

                    UPDATE dbo.FHInviteInfo WITH (ROWLOCK)
                        SET Checked = 1
                            WHERE LoginUIDB = @iLoginUIDB AND Type = 0
                    IF @@ERROR <> 0 OR @@ROWCOUNT <> 1
                        BEGIN    SELECT @iOK = -13    GOTO FAIL_TRAN    END
                END
            ELSE IF @bNew = 1
                BEGIN
                    INSERT INTO dbo.FHInviteUser
                            (
                                LoginUIDB
                            )
                        SELECT  @iLoginUIDB
                    IF @@ERROR <> 0 OR @@ROWCOUNT <> 1
                        BEGIN    SELECT @iOK = -14    GOTO FAIL_TRAN    END

                    INSERT INTO dbo.FHInviteInfo
                            (
                                LoginUIDB
,                               Type
,                               Checked
                            )
                        SELECT  @iLoginUIDB
,                               Type
,                               case Type when 0 then 1 else 0 end
                            FROM dbo.FHInviteType WITH (NOLOCK)
                    IF @@ERROR <> 0 OR @@ROWCOUNT <> 5
                        BEGIN    SELECT @iOK = -15    GOTO FAIL_TRAN    END
                END
        end
COMMIT TRAN

GOTO END_PROC

FAIL_TRAN:
ROLLBACK TRAN

END_PROC:
SELECT @iOK


