CREATE PROCEDURE [dbo].[RCR_rank_character_abuse]
	@iLoginUID_ [int],
	@iOK [int] = 0
AS
set nocount on

if not exists
(
    select *
    from dbo.Users
    with (nolock)
    where
        LoginUID = @iLoginUID_
)
begin    select @iOK = -1    goto end_proc    end

declare
    @sdtDate smalldatetime
select
    @sdtDate = convert(nvarchar(10), getdate(), 120)

if exists
(
    select *
    from dbo.RCRRankCharacterAbuse
    with (nolock)
    where
        LoginUID = @iLoginUID_
    and RegDate = @sdtDate
)
begin    select @iOK = -2    goto end_proc    end

begin tran
    insert into dbo.RCRRankCharacterAbuse
    with (updlock)
    (
        LoginUID
    ,   RegDate
    ,   Abused
    )
    select
        @iLoginUID_
    ,   @sdtDate
    ,   0
    option (maxdop 1)
    if @@error <> 0
    begin    select @iOK = -101    goto fail_tran    end
commit tran

goto end_proc

fail_tran:
rollback tran

end_proc:
select
    @iOK


