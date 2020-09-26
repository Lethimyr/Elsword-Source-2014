CREATE PROCEDURE [dbo].[gup_update_Socket]
	@iItemUID	bigint
,	@iSoket1	smallint
,	@iSoket2	smallint
,	@iSoket3	smallint
,	@iSoket4	smallint
,	@iOK		int = 0

AS
SET NOCOUNT ON;
SET XACT_ABORT ON;

BEGIN TRAN
	IF NOT EXISTS (SELECT * FROM dbo.GItemSocket WITH (NOLOCK) WHERE ItemUID = @iItemUID)
		BEGIN
			INSERT INTO dbo.GItemSocket (ItemUID, Socket1, Socket2, Socket3, Socket4)
				SELECT @iItemUID, @iSoket1, @iSoket2, @iSoket3, @iSoket4
			IF @@ERROR <> 0 OR @@ROWCOUNT <> 1
				BEGIN	SELECT @iOK = -1	GOTO FAIL_TRAN	END
			GOTO COMMIT_TRAN
		END

		UPDATE dbo.GItemSocket
			SET Socket1	= @iSoket1
,				Socket2	= @iSoket2
,				Socket3	= @iSoket3
,				Socket4	= @iSoket4
			WHERE ItemUID = @iItemUID
	IF @@ERROR <> 0 OR @@ROWCOUNT <> 1 
		BEGIN	SELECT @iOK = -2	GOTO FAIL_TRAN	END


COMMIT_TRAN:
COMMIT TRAN

GOTO END_PROC

FAIL_TRAN:
ROLLBACK TRAN

END_PROC:
SELECT @iOK


