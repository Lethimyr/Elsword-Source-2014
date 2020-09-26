CREATE PROCEDURE [dbo].[CSGA_couple_system_merge_check]
	@iLoginUID_ [int],
	@iLoginUIDE_ [int],
	@iOK [int] = 0
AS
set nocount on
set transaction isolation level read uncommitted

/*
20081027. microcat.

{ call dbo.CSGA_couple_system_merge_check ( %d, %d ) }

{ call dbo.CSGA_couple_system_merge_check ( @1, @2 ) }
@1 ; LoginUID int ; 내꺼
@2 ; LoginUIDE int ; 차단될 유저의 UID. -1를 넣어주면 전체 유저로 약속.

1 return ( @1 )
@1 ; OK int

OK
0 ; 성공
-1 ; 내꺼 유저 정보가 존재하지 않음
-101이하 ; 트랜잭션 에러
*/

execute dbo.AAA_login_check
    @iLoginUID_
,   @iOK output

if @iOK <> 0
begin    select @iOK = -1    goto end_proc    end

declare
    @sdtDateD smalldatetime
select
    @sdtDateD = convert(nvarchar(10), current_timestamp, 120)

if exists
(
    select *
    from dbo.CSGACoupleSystemCheck
    where
        LoginUID = @iLoginUID_
    and LoginUIDE = @iLoginUIDE_
)
begin
    begin transaction

    update a
    with (updlock)
    set
        RegDateD = @sdtDateD
    from dbo.CSGACoupleSystemCheck
    as a
    with (updlock)
    where
        LoginUID = @iLoginUID_
    and LoginUIDE = @iLoginUIDE_
    option (maxdop 1)

    if @@error <> 0
    begin    select @iOK = -101    goto fail_tran    end
end
else
begin
    begin transaction

    insert into dbo.CSGACoupleSystemCheck
    with (updlock)
    (
        LoginUID
    ,   LoginUIDE
    ,   RegDateD
    )
    select
        @iLoginUID_
    ,   @iLoginUIDE_
    ,   @sdtDateD
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


