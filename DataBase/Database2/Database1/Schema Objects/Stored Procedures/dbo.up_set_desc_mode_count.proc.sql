CREATE PROCEDURE [dbo].[up_set_desc_mode_count]
	@strData_ [nvarchar](4000),
	@iOK [int] = 0
AS
/*microcat
작성일 : 2006년 12월 11일
사용 : 게임서버
*/
SET NOCOUNT ON;

DECLARE @DescModeCount TABLE
(
    ModeCode int not null primary key
,   ModeName nvarchar(20) not null
)

WHILE RIGHT(@strData_, 1) = ','
    BEGIN
        SELECT @strData_ = SubString(@strData_, 1, Len(@strData_) - 1)
    END

DECLARE @strToken nvarchar(1)
,       @iStart int
,       @iMid int
,       @iEnd int
SELECT  @strToken = N','
,       @iStart = 1
,       @iMid = 1
,       @iEnd = 1
WHILE 1 = 1
    BEGIN
        SELECT  @iMid = CharIndex(@strToken, @strData_, @iStart)
,               @iEnd = CharIndex(@strToken, @strData_, @iMid + 1)

        IF @iMid = 0
            BEGIN    SELECT @iOK = -1    GOTO END_PROC    END
        IF @iEnd = 0
            BEGIN    SELECT @iEnd = 4000    END

        INSERT INTO @DescModeCount(ModeCode, ModeName) --모드가 중복해서 들어오지 않는다고 가정함 -__;;;
            SELECT  SubString(@strData_, @iStart, @iMid - @iStart)
,                   SubString(@strData_, @iMid + 1, @iEnd - @iMid - 1)

        IF @iEnd = 4000
            BEGIN    BREAK    END

        SELECT @iStart = @iEnd + 1
    END

BEGIN TRAN
    update a with (updlock)
        set a.ModeName = b.ModeName
            from dbo.DescModeCount as a with (updlock)
            join @DescModeCount as b
                on a.ModeCode = b.ModeCode
    if @@error <> 0
        begin    select @iOK = -101    goto fail_tran    end

    insert into dbo.DescModeCount with (updlock)
            (
                ModeCode
,               ModeName
            )
        select  a.ModeCode
,               a.ModeName
            from @DescModeCount as a
                where not exists
                    (
                        select *
                            from dbo.DescModeCount as b with (updlock)
                                where a.ModeCode = b.ModeCode
                    )
    if @@error <> 0
        begin    select @iOK = -102    goto fail_tran    end
COMMIT TRAN

GOTO END_PROC

FAIL_TRAN:
ROLLBACK TRAN

END_PROC:
SELECT @iOK


