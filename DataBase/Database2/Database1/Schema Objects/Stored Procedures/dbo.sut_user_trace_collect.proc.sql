CREATE PROCEDURE [dbo].[sut_user_trace_collect]
	@iLoginUID_ [int],
	@sdtStartDate_ [smalldatetime],
	@sdtEndDate_ [smalldatetime],
	@iGrade_ [tinyint],
	@iPlayTime_ [int],
	@bPCBang_ [bit],
	@bGuildServer_ [bit],
	@strCharacterData_ [nvarchar](4000),
	@iOK [int] = 0,
	@iError [int] = 0
AS
/*microcat
작성일 : 2007년 10월 22일
사용 : 유저의 추적 결과를 수집
*/
set nocount on;

declare @iUID int
select  @iUID = -1

--유저 체크
if not exists
    (
        select *
            from dbo.Users with (nolock)
                where LoginUID = @iLoginUID_
    )
    begin    select @iOK = -1    goto end_proc    end

--캐릭터 정보
declare @Character table
(
    CharType tinyint not null
,   [EXP] bigint not null
,   Level int not null
,   PlayCount int not null
,   Turtle bit not null
--,   primary key clustered
--    (
--        CharType
--    )
)

declare @strToken nvarchar(1)
,       @iStart int
,       @iEnd int
,       @iCharType tinyint
,       @iEXP bigint
,       @iLevel int
,       @iPlayCount int
,       @bTurtle bit
select  @strToken = N','
,       @iStart = 1
,       @iEnd = 1

while right(@strCharacterData_, 1) = @strToken
    begin
        select @strCharacterData_ = substring(@strCharacterData_, 1, len(@strCharacterData_) - 1)
    end

while 1 = 1
    begin
        select @iEnd = charindex(@strToken, @strCharacterData_, @iStart + 1)
        if @iEnd = 0
            begin    select @iOK = -2    goto end_proc    end
        select @iCharType = substring(@strCharacterData_, @iStart, @iEnd - @iStart)
        select @iStart = @iEnd + 1

        select @iEnd = charindex(@strToken, @strCharacterData_, @iStart + 1)
        if @iEnd = 0
            begin    select @iOK = -3    goto end_proc    end
        select @iEXP = substring(@strCharacterData_, @iStart, @iEnd - @iStart)
        select @iStart = @iEnd + 1
        
        select @iEnd = charindex(@strToken, @strCharacterData_, @iStart + 1)
        if @iEnd = 0
            begin    select @iOK = -4    goto end_proc    end
        select @iLevel = substring(@strCharacterData_, @iStart, @iEnd - @iStart)
        select @iStart = @iEnd + 1

        select @iEnd = charindex(@strToken, @strCharacterData_, @iStart + 1)
        if @iEnd = 0
            begin    select @iOK = -5    goto end_proc    end
        select @iPlayCount = substring(@strCharacterData_, @iStart, @iEnd - @iStart)
        select @iStart = @iEnd + 1

        select @iEnd = charindex(@strToken, @strCharacterData_, @iStart + 1)
        if @iEnd = 0
            begin    select @iEnd = 4000    end
        select @bTurtle = substring(@strCharacterData_, @iStart, @iEnd - @iStart)
        if exists
            (
                select *
                    from @Character
                        where CharType = @iCharType
            )
            begin    select @iOK = -6    goto end_proc    end
        insert into @Character
                (
                    CharType
,                   [EXP]
,                   Level
,                   PlayCount
,                   Turtle
                )
            select  @iCharType
,                   @iEXP
,                   @iLevel
,                   @iPlayCount
,                   @bTurtle
        if @@error <> 0
            begin    select @iOK = -7    goto end_proc    end
        if @iEnd = 4000
            begin    break    end
        select @iStart = @iEnd + 1
    end

--데이터 입력
begin tran
    insert into dbo.SUTUserTrace with (updlock)
            (
                LoginUID
,               StartDate
,               EndDate
,               Grade
,               PlayTime
,               PCBang
,               GuildServer
            )
        select  @iLoginUID_
,               @sdtStartDate_
,               @sdtEndDate_
,               @iGrade_
,               @iPlayTime_
,               @bPCBang_
,               @bGuildServer_
    if @@error <> 0
        begin    select @iOK = -101, @iError = @@error    goto fail_tran    end

    select @iUID = @@identity

    insert into dbo.SUTUserTraceCharacter with (updlock)
            (
                LoginUID
,               UID
,               CharType
,               [EXP]
,               Level
,               PlayCount
,               Turtle
            )
        select  @iLoginUID_
,               @iUID
,               CharType
,               [EXP]
,               Level
,               PlayCount
,               Turtle
            from @Character
    if @@error <> 0
        begin    select @iOK = -102, @iError = @@error    goto fail_tran    end
commit tran

goto end_proc

fail_tran:
rollback tran

end_proc:
select @iOK


