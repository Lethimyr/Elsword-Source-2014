CREATE PROCEDURE [dbo].[gup_pointbag_get]
	@iLoginUID_ [int],
	@bReturn_ [bit] = 0,
	@iOK [int] = 0
AS
/*microcat
작성일 : 2007년 4월 12일
사용 : 유저의 포인트백 정보를 얻는다.
*/
SET NOCOUNT ON;

DECLARE @iPoint int
,       @iPlayTime int
,       @sdtNow smalldatetime
SELECT  @sdtNow = GetDate()

IF NOT EXISTS(SELECT * FROM dbo.Users WITH (NOLOCK) WHERE LoginUID = @iLoginUID_)
    BEGIN    SELECT @iOK = -1, @iPoint = 0, @iPlayTime = 0    GOTO END_PROC    END

DECLARE @sdtRegDate smalldatetime
,       @bNewUser bit
,       @strFirst nvarchar(3)
SELECT  @bNewUser = 0
,       @strFirst = N'-01'

SELECT  @iPoint = Point
    FROM dbo.GPointBag WITH (NOLOCK)
        WHERE LoginUID = @iLoginUID_

declare @GPointBag table
(
    RegDate nvarchar(7) primary key clustered
)

IF @iPoint IS NULL
    BEGIN
        SELECT  @iPoint = 500
,               @sdtRegDate = @sdtNow
,               @iPlayTime = 0
,               @bNewUser = 1
    END
ELSE
    BEGIN
        SELECT  @iPlayTime = PlayTime
,               @sdtRegDate = RegDate
            FROM dbo.GPointBagAttendance WITH (NOLOCK)
                WHERE LoginUID = @iLoginUID_

        insert into @GPointBag(RegDate)
            select a.RegDate
                from
                    (
                        select  convert(nvarchar(7), RegDate, 120) as RegDate
,                               count(*) as [Count]
                        from dbo.GPointBagCalender with (nolock)
                        where LoginUID = @iLoginUID_ and RegDate < convert(nvarchar(7), @sdtNow, 120) + @strFirst
                        group by convert(nvarchar(7), RegDate, 120)
                    ) as a
                left outer join
                    (
                        select  convert(nvarchar(7), RegDate, 120) as RegDate
                        from dbo.GpointBagMonth with (nolock)
                        where LoginUID = @iLoginUID_ and RegDate < convert(nvarchar(7), @sdtNow, 120) + @strFirst
                        group by convert(nvarchar(7), RegDate, 120)
                    ) as b
                on a.RegDate = b.RegDate
                    where b.RegDate is null
                        and a.[Count] = datediff(dd, a.RegDate + @strFirst, dateadd(mm, 1, a.RegDate + @strFirst))
    END

BEGIN TRAN
    IF @bNewUser = 1
        BEGIN
            INSERT INTO dbo.GPointBag
                    (
                        LoginUID
,                       Point
                    )
                SELECT  @iLoginUID_
,                       @iPoint
            IF @@ERROR <> 0 OR @@ROWCOUNT <> 1
                BEGIN    SELECT @iOK = -11    GOTO FAIL_TRAN    END

            INSERT INTO dbo.GPointBagAttendance
                    (
                        LoginUID
,                       PlayTime
,                       RegDate
                    )
                SELECT  @iLoginUID_
,                       @iPlayTime
,                       @sdtNow
            IF @@ERROR <> 0 OR @@ROWCOUNT <> 1
                BEGIN    SELECT @iOK = -12    GOTO FAIL_TRAN    END
        END
    else if @bNewUser = 0
        begin
            update a
                set a.SealType = a.SealType + 0x10
                    from dbo.GPointBagCalender as a with (nolock)
                    join @GPointBag as b
                    on a.LoginUID = @iLoginUID_
                        and convert(nvarchar(7), a.RegDate, 120) = b.RegDate
                        and a.SealType & 0x10 <> 0x10
            if @@error <> 0
                begin    select @iOK = -13    goto fail_tran    end

            insert into dbo.GPointBagMonth(LoginUID, RegDate)
                select  @iLoginUID_
,                       dateadd(mm, 1, RegDate + @strFirst) - 1
                    from @GPointBag
            if @@error <> 0
                begin    select @iOK = -14    goto fail_tran    end
        end
COMMIT TRAN

GOTO END_PROC

FAIL_TRAN:
ROLLBACK TRAN

END_PROC:
if @bReturn_ = 1
    begin
        return @iOK
    end
else
    begin
        SELECT  @iOK
,               @iPoint
,               @iPlayTime
,               @sdtRegDate
    end


