CREATE PROCEDURE [dbo].[MSGA_mission_system_merge]
	@iLoginUID_ [int],
	@iMainMissionID_ [int],
	@iSubMissionID_ [int],
	@iProgressRate_ [smallint],
	@iOK [int] = 0
AS
set nocount on
set transaction isolation level read uncommitted

/*
20090309. microcat.

L"{ call dbo.MSGA_mission_system_merge ( %d, %d, %d, %d ) }"

{ call dbo.MSGA_mission_system_merge ( @1, @2, @3, @4 ) }
@1 ; @iLoginUID_ [int],
@2 ; @iMainMissionID_ [int],
@3 ; @iSubMissionID_ [int],
@4 ; @iProgressRate_ [smallint],

1 return ( @1 )
@1 ; OK int
    0        ; 성공
    -1       ; 유저 정보가 없음
    -2       ; 메인 미션 정보가 없음
    -101이하 ; 트랜잭션 에러

dbo.GMSMissionSystemLog.Status
    1 ; 기간 만료
    2 ; 유저에 의해서 삭제됨
    3 ; 유저에 의해서 미션 완료
*/

if not exists
(
    select *
    from dbo.Users
    where
        LoginUID = @iLoginUID_
)
begin    select @iOK = -1    goto end_proc    end

if not exists
(
    select *
    from dbo.GMSMissionSystemMain
    where
        LoginUID = @iLoginUID_
    and MainMissionID = @iMainMissionID_
)
begin    select @iOK = -2    goto end_proc    end

if exists
(
    select *
    from dbo.GMSMissionSystemSub
    where
        LoginUID = @iLoginUID_
    and MainMissionID = @iMainMissionID_
    and SubMissionID = @iSubMissionID_
)
begin
    begin transaction

    update a
    with (updlock)
    set
        ProgressRate = @iProgressRate_
    from dbo.GMSMissionSystemSub
    as a
    with (updlock)
    where
        LoginUID = @iLoginUID_
    and MainMissionID = @iMainMissionID_
    and SubMissionID = @iSubMissionID_

    if @@error <> 0
    begin    select @iOK = -101    goto fail_tran    end
end
else
begin
    begin transaction

    insert into dbo.GMSMissionSystemSub
    with (updlock)
    (
        LoginUID
    ,   MainMissionID
    ,   SubMissionID
    ,   ProgressRate
    )
    select
        @iLoginUID_
    ,   @iMainMissionID_
    ,   @iSubMissionID_
    ,   @iProgressRate_

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


