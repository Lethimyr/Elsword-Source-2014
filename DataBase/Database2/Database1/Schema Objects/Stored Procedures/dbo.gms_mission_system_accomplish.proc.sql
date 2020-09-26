CREATE PROCEDURE [dbo].[gms_mission_system_accomplish]
	@iLoginUID_ [int],
	@iMainMissionID_ [int],
	@bUnique_ [bit],
	@iOK [int] = 0,
	@iError [int] = 0
AS
/*microcat
작성일 : 2007년 10월 23일
사용 : 유저가 미션을 완료하려 한다.
*/
set nocount on;

--dbo.GMSMissionSystemLog의 Status
--1 : 기간 만료
--2 : 유저에 의해서 삭제됨
--3 : 유저에 의해서 미션 완료

if not exists
    (
        select *
            from dbo.Users with (nolock)
                where LoginUID = @iLoginUID_
    )
    begin    select @iOK = -1    goto end_proc    end
----
declare @sdtStartDate smalldatetime
select  @sdtStartDate = RegDate
    from dbo.GMSMissionSystemMain with (nolock)
        where LoginUID = @iLoginUID_
        and MainMissionID = @iMainMissionID_

if @sdtStartDate is null
    begin    select @iOK = -2    goto end_proc    end
----
declare @sdtDate smalldatetime
select  @sdtDate = getdate()

begin tran
    delete a with (updlock)
        from dbo.GMSMissionSystemMain as a with (updlock)
            where a.LoginUID = @iLoginUID_
            and a.MainMissionID = @iMainMissionID_
    if @@error <> 0
        begin    select @iOK = -101, @iError = @@error    goto fail_tran    end

    insert into dbo.GMSMissionSystemUniqueLog with (updlock)
            (
                LoginUID
,               MainMissionID
,               StartDate
,               EndDate
            )
        select  @iLoginUID_
,               @iMainMissionID_
,               @sdtStartDate
,               @sdtDate
            where @bUnique_ = 1
    if @@error <> 0
        begin    select @iOK = -102, @iError = @@error    goto fail_tran    end

    insert into dbo.GMSMissionSystemLog with (updlock)
            (
                LoginUID
,               MainMissionID
,               StartDate
,               EndDate
,               Status
            )
        select  @iLoginUID_
,               @iMainMissionID_
,               @sdtStartDate
,               @sdtDate
,               3 --유저에 의해서 미션 완료
    if @@error <> 0
        begin    select @iOK = -103, @iError = @@error    goto fail_tran    end
commit tran

goto end_proc
----
fail_tran:
rollback tran

end_proc:
select @iOK


