CREATE PROCEDURE [dbo].[gup_update_item]
    @iItemUID   [bigint],
    @iUsageType [int],
    @iParam     [int],
    @iOK        [int] = 0
AS

SET NOCOUNT ON;

-- 해당 아이템이 없는 경우.
IF NOT EXISTS( SELECT * FROM dbo.GItem WITH( NOLOCK ) WHERE ItemUID = @iItemUID AND Deleted = 0 )
BEGIN
    SELECT @iOK = -1
    GOTO END_PROC
END

-- 사용 타입이 올바른지 판단한다.
IF @iUsageType < 1 OR @iUsageType > 2
BEGIN
    SELECT @iOK = -2
    GOTO END_PROC
END



-- 수량값이 더해서 음수 값을 것을 대비
DECLARE	@iQuantity	int
SELECT	@iQuantity	= Quantity 
	FROM dbo.GItemQuantity WITH (NOLOCK) 
		WHERE ItemUID = @iItemUID


BEGIN TRAN
    IF @iUsageType = 2
    BEGIN
	-- 수량성인데 더한 값이 음수 일 때 에러
		IF (@iQuantity + @iParam) < 0
		BEGIN
            SELECT @iOK = -5	GOTO FAIL_TRAN
        END

        UPDATE dbo.GItemQuantity WITH( ROWLOCK )
            SET Quantity = Quantity + @iParam
            WHERE ItemUID = @iItemUID
        IF @@ERROR <> 0 OR @@ROWCOUNT <> 1
        BEGIN
            SELECT @iOK = -3
            GOTO FAIL_TRAN
        END
    END

    -- TODO : 기간제 아이템 처리 필요.
	-- 내구도
	IF @iUsageType = 1
    BEGIN
        UPDATE dbo.GItemEndurance WITH( ROWLOCK )
            SET Endurance = Endurance + @iParam
            WHERE ItemUID = @iItemUID
        IF @@ERROR <> 0 OR @@ROWCOUNT <> 1
        BEGIN
            SELECT @iOK = -4
            GOTO FAIL_TRAN
        END
    END
    
COMMIT TRAN

GOTO END_PROC

FAIL_TRAN:
ROLLBACK TRAN

END_PROC:
SELECT @iOK


