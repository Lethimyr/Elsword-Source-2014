CREATE PROCEDURE [dbo].[wlup_win_lose_reset]
	@iLoginUID_ [int],
	@strCharType_ [nvarchar](4000),
	@iOK [int] = 0
AS
set nocount on;

if len(@strCharType_) = 0 or @strCharType_ is null
    begin    select @iOK = -1    goto end_proc    end

declare @strLogin nvarchar(20)
,       @strToken nvarchar(1)
select  @strLogin = Login
,       @strToken = N','
    from dbo.Users with (nolock)
        where LoginUID = @iLoginUID_
if @strLogin is null
    begin    select @iOK = -2    goto end_proc    end

declare @Character table
(
    CharType tinyint not null primary key clustered
,   Win int null
,   Lose int null
)

while right(@strCharType_, 1) = @strToken
    begin
        select @strCharType_ = substring(@strCharType_, 1, len(@strCharType_) - 1)
    end

declare @iCharType int
,       @iStart int
,       @iEnd int
select  @iStart = 1
,       @iEnd = 1
while 1 = 1
begin
    select @iEnd = charindex(@strToken, @strCharType_, @iStart + 1)
    if @iEnd = 0
        begin    select @iEnd = 4000    end
    select @iCharType = substring(@strCharType_, @iStart, @iEnd - @iStart)
    if @iCharType not between 0 and 255
        begin    select @iOK = -3    goto end_proc    end

    insert into @Character
            (
                CharType
            )
        select  @iCharType

    if @iEnd = 4000
        begin    break    end

    select @iStart = @iEnd + 1
end

if exists(select * from @Character as a where not exists(select * from dbo.Characters as b with (nolock) where b.Login = @strLogin and a.CharType = b.CharType))
    begin    select @iOK = -4    goto end_proc    end

update a
    set a.Win = b.Win
,       a.Lose = b.Lose
        from @Character as a
        join dbo.Characters as b with (nolock)
            on a.CharType = b.CharType and b.Login = @strLogin

begin tran
    update a
        set a.Win = 0
,           a.Lose = 0
            from dbo.Characters as a with (nolock)
                where a.Login = @strLogin
                    and exists(select * from @Character as b where a.CharType = b.CharType)
    if @@error <> 0
        begin    select @iOK = -11    goto fail_tran    end

    insert into dbo.WLWinLoseResetLog
            (
                LoginUID
,               CharType
,               Win
,               Lose
            )
        select  @iLoginUID_
,               CharType
,               Win
,               Lose
            from @Character
    if @@error <> 0
        begin    select @iOK = -12    goto fail_tran    end
commit tran

goto end_proc

fail_tran:
rollback tran

end_proc:
select @iOK


