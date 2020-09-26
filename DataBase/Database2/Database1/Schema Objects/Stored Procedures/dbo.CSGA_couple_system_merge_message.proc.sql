CREATE PROCEDURE [dbo].[CSGA_couple_system_merge_message]
	@iLoginUID_ [int],
	@strMessage_ [nvarchar](256),
	@iOK [int] = 0
AS
set nocount on
set transaction isolation level read uncommitted

/*
20081111. microcat.

{ call dbo.CSGA_couple_system_merge_message ( %d, N'%s' ) }

{ call dbo.CSGA_couple_system_merge_message ( @1, @2 ) }
@1 ; LoginUID int
@2 ; Message nvarchar(256)

1 return ( @1 )
@1 ; OK int

OK
0 ; 성공
-1 ; 내꺼 유저 정보가 존재하지 않음
-2 ; 내 커플 정보가 존재하지 않음
-101이하 ; 트랜잭션 에러
*/

execute dbo.AAA_login_check
    @iLoginUID_
,   @iOK output

if @iOK <> 0
begin    select @iOK = -1    goto end_proc    end

if not exists
(
    select *
    from dbo.CSGACoupleSystem
    where
        LoginUID = @iLoginUID_
)
begin    select @iOK = -2    goto end_proc    end

if exists
(
    select *
    from dbo.CSGACoupleSystemMessage
    where
        LoginUID = @iLoginUID_
)
begin
    begin transaction

    update a
    with (updlock)
    set
        a.Message = @strMessage_
    from dbo.CSGACoupleSystemMessage
    as a
    with (updlock)
    where
        LoginUID = @iLoginUID_
    option (maxdop 1)

    if @@error <> 0
    begin    select @iOK = -101    goto fail_tran    end
end
else
begin
    begin transaction

    insert into dbo.CSGACoupleSystemMessage
    with (updlock)
    (
        LoginUID
    ,   Message
    )
    select
        @iLoginUID_
    ,   @strMessage_
    option (maxdop 1)

    if @@error <> 0
    begin    select @iOK = -102    goto fail_tran    end
end

commit transaction

goto end_proc

fail_tran:
rollback transaction

end_proc:
select
    @iOK


