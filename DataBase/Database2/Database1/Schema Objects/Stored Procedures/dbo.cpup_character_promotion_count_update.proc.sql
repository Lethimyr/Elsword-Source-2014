CREATE PROCEDURE [dbo].[cpup_character_promotion_count_update]
	@strData_ [nvarchar](4000),
	@iOK [int] = 0
AS
/*microcat
작성일 : 2007년 7월 2일
사용 : 캐릭터별 전직별 플레이 카운트
*/
set nocount on;

declare @strToken nvarchar(1)
select  @strToken = N','

while right(@strData_, 1) = @strToken
    begin
        select @strData_ = substring(@strData_, 1, Len(@strData_) - 1)
    end

declare @iCharType tinyint
,       @iPromotion tinyint
,       @iCount int
,       @iStart int
,       @iEnd int
select  @iStart = 1
,       @iEnd = 1

declare @Data table
(
    CharType tinyint not null
,   Promotion tinyint not null
,   [Count] int not null
,   primary key clustered (CharType, Promotion)
)
while 1 = 1
    begin
        select @iEnd = charindex(@strToken, @strData_, @iStart + 1)
        if @iEnd = 0
            begin    select @iOK = -1    goto end_proc    end
        select @iCharType = substring(@strData_, @iStart, @iEnd - @iStart)
        if not exists(select * from dbo.CPCharacter with (nolock) where CharType = @iCharType)
            begin    select @iOK = -2    goto end_proc    end
        select @iStart = @iEnd + 1

        select @iEnd = charindex(@strToken, @strData_, @iStart + 1)
        if @iEnd = 0
            begin    select @iOK = -3    goto end_proc    end
        select @iPromotion = substring(@strData_, @iStart, @iEnd - @iStart)
        if not exists(select * from dbo.CPPromotion with (nolock) where Promotion = @iPromotion)
            begin    select @iOK = -4    goto end_proc    end
        select @iStart = @iEnd + 1

        select @iEnd = charindex(@strToken, @strData_, @iStart + 1)
        if @iEnd = 0
            begin    select @iEnd = 4000    end
        select @iCount = substring(@strData_, @iStart, @iEnd - @iStart)
        if @iCount < 0
            begin    select @iOK = -5    goto end_proc    end

        insert into @Data
                (
                    CharType
,                   Promotion
,                   [Count]
                )
            select  @iCharType
,                   @iPromotion
,                   @iCount

        if @iEnd = 4000
            begin    break    end

        select @iStart = @iEnd + 1
    end

begin tran
    update a set a.[Count] = a.[Count] + b.[Count]
        from dbo.CPDailyCount as a with (nolock)
        join @Data as b on a.CharType = b.CharType and a.Promotion = b.Promotion
    if @@error <> 0
        begin    select @iOK = -11    goto fail_tran    end

    insert into dbo.CPDailyCount
            (
                CharType
,               Promotion
,               [Count]
            )
        select  CharType
,               Promotion
,               [Count]
            from @Data as a
                where not exists(select * from dbo.CPDailyCount as b with (nolock) where a.CharType = b.CharType and a.Promotion = b.Promotion)
    if @@error <> 0
        begin    select @iOK = -12    goto fail_tran    end
commit tran

goto end_proc

fail_tran:
rollback tran

end_proc:
select @iOK


