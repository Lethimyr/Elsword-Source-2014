CREATE PROCEDURE [dbo].[gms_mission_system_main_get]
	@iLoginUID_ [int],
	@iOK [int] = 0,
	@iError [int] = 0
AS
/*microcat
작성일 : 2007년 10월 23일
사용 : 유저의 메인 미션 정보를 출력
*/
set nocount on;

--dbo.GMSMissionSystemLog의 Status
--1 : 기간 만료
--2 : 유저에 의해서 삭제됨
--3 : 유저에 의해서 미션 완료

declare @Mission table
(
    MainMissionID int not null
,   RegDate smalldatetime not null
,   LimitDate smalldatetime not null
,   Deleted bit not null
,   primary key clustered
    (
        MainMissionID
    )
)

insert into @Mission
        (
            MainMissionID
,           RegDate
,           LimitDate
,           Deleted
        )
    select  MainMissionID
,           RegDate
,           LimitDate
,           case when RegDate < LimitDate and LimitDate < getdate() then 1 else 0 end
        from dbo.GMSMissionSystemMain with (nolock)
            where LoginUID = @iLoginUID_
----
begin tran
    delete a with (updlock)
        from dbo.GMSMissionSystemMain as a with (updlock)
        join @Mission as b
            on a.MainMissionID = b.MainMissionID
            and a.LoginUID = @iLoginUID_
            and b.Deleted = 1
    if @@error <> 0
        begin    select @iOK = -101, @iError = @@error    goto fail_tran    end

    insert into dbo.GMSMissionSystemLog with (updlock)
            (
                LoginUID
,               MainMissionID
,               StartDate
,               EndDate
,               Status
            )
        select  @iLoginUID_
,               MainMissionID
,               RegDate
,               LimitDate
,               1 --기간 만료
            from @Mission
                where Deleted = 1
    if @@error <> 0
        begin    select @iOK = -102, @iError = @@error    goto fail_tran    end
commit tran

goto end_proc
----
fail_tran:
rollback tran

end_proc:
if @iOK <> 0
    begin
        declare @strDB nvarchar(4000)
,               @strProc nvarchar(4000)
        select  @strDB = db_name()
,               @strProc = object_name(@@procid)
        raiserror
            (
                'microcat : %s.dbo.%s ( OK : %d, Error : %d / LoginUID : %d )'
,               16, 127
,               @strDB, @strProc
,               @iOK, @iError
,               @iLoginUID_
            )
    end

select  MainMissionID
,       RegDate
,       LimitDate
,       Deleted
    from @Mission


