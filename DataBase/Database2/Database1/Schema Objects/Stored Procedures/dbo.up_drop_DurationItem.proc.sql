CREATE PROCEDURE [dbo].[up_drop_DurationItem]
	@strOwnerLogin_ [nvarchar](20),
	@iDurationItemUID_ [int]
AS
exec dbo.up_log_set_proc_count 'up_drop_DurationItem'

SET NOCOUNT ON

DECLARE     @iGP              int,
            @bCash            bit,
            @iGoodsID         int,
            @bShowWeb         bit,
            @bPatternGoods    bit,
            @iReturnGP        int,
            @iBasePrice       int,
            @bBaseCash        bit,
            @strBuyer         nvarchar(20),
            @regDate          smalldatetime,
            @iDuration        int
   -- SELECT -1, 0
    --RETURN

-- Owner의 존재,
declare
    @iLoginUID int
select
    @iLoginUID = LoginUID
from dbo.Users
with (nolock)
where
    Login = @strOwnerLogin_

if @iLoginUID is null
BEGIN
    SELECT -1
    RETURN
END

-- Owner가 소유한 상품인지 확인, 상품의 ID 알아내기, Duration 갯수 알아내기 
IF NOT EXISTS(SELECT * FROM dbo.DurationItemObjectList(NOLOCK) WHERE DurationItemUID = @iDurationItemUID_ and OwnerLogin = @strOwnerLogin_)
BEGIN
    SELECT -2
    RETURN
END
SELECT @iGoodsID = GoodsID, @strBuyer = BuyerLogin, @iDuration = Duration, @regDate = RegDate FROM dbo.DurationItemObjectList(NOLOCK) WHERE DurationItemUID = @iDurationItemUID_ and OwnerLogin = @strOwnerLogin_

BEGIN TRAN
    DELETE FROM dbo.DurationItemObjectList WHERE DurationItemUID = @iDurationItemUID_
    IF @@ERROR <> 0
        GOTO lbTran_Abort

    insert into [Log].dbo.ICLOItemCountItem
    with (updlock)
    (
        LoginUID
    ,   ItemUID
    ,   BuyerLogin
    ,   ItemID
    ,   RegDate
    ,   DelDate
    )
    select
        @iLoginUID
    ,   @iDurationItemUID_
    ,   @strBuyer
    ,   @iGoodsID
    ,   @regDate
    ,   current_timestamp

    IF @@ERROR <> 0
        GOTO lbTran_Abort

    SET NOCOUNT OFF

COMMIT TRAN
GOTO lbTran_Success

lbTran_Abort:
ROLLBACK TRAN
SELECT -5
RETURN

lbTran_Success:
SELECT 0
RETURN


