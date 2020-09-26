CREATE PROCEDURE [dbo].[gms_mission_system_register]
	@iLoginUID_ [int],
	@strPActiveMission_ [nvarchar](4000),
	@strPAccomplishMission_ [nvarchar](4000),
	@iMainMissionID_ [int],
	@iLimitDay_ [int],
	@strSubMission_ [nvarchar](4000),
	@iOK [int] = 0,
	@iError [int] = 0
AS
/*microcat
작성일 : 2007년 10월 23일
사용 : 유저가 미션을 등록하려 한다.
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
declare @strToken nvarchar(1)
,       @iStart int
,       @iEnd int
select  @strToken = N','
----
declare @PActiveMission table
(
    MainMissionID int not null
--,   primary key clustered
--    (
--        MainMissionID
--    )
)

select  @iStart = 1
,       @iEnd = 1

while right(@strPActiveMission_, 1) = @strToken
    begin
        select @strPActiveMission_ = substring(@strPActiveMission_, 1, Len(@strPActiveMission_) - 1)
    end

while 1 = 1
    begin
        select @iEnd = charindex(@strToken, @strPActiveMission_, @iStart + 1)

        if @iEnd = 0
            begin    select @iEnd = 4000    end

        insert into @PActiveMission
                (
                    MainMissionID
                )
            select  substring(@strPActiveMission_, @iStart, @iEnd - @iStart)
                where len(@strPActiveMission_) > 0
        if @@error <> 0
            begin    select @iOK = -2    goto end_proc    end

        if @iEnd = 4000
            begin    break    end

        select @iStart = @iEnd + 1
    end

if exists
    (
        select *
            from dbo.GMSMissionSystemMain as a with (nolock)
            join @PActiveMission as b
                on a.MainMissionID = b.MainMissionID
                and a.LoginUID = @iLoginUID_
    )
    begin    select @iOK = -3    goto end_proc    end
----
declare @PAccomplishMission table
(
    MainMissionID int not null
--,   primary key clustered
--    (
--        MainMissionID
--    )
)

select  @iStart = 1
,       @iEnd = 1

while right(@strPAccomplishMission_, 1) = @strToken
    begin
        select @strPAccomplishMission_ = substring(@strPAccomplishMission_, 1, Len(@strPAccomplishMission_) - 1)
    end

while 1 = 1
    begin
        select @iEnd = charindex(@strToken, @strPAccomplishMission_, @iStart + 1)

        if @iEnd = 0
            begin    select @iEnd = 4000    end

        insert into @PAccomplishMission
                (
                    MainMissionID
                )
            select  substring(@strPAccomplishMission_, @iStart, @iEnd - @iStart)
                where len(@strPAccomplishMission_) > 0
        if @@error <> 0
            begin    select @iOK = -4    goto end_proc    end

        if @iEnd = 4000
            begin    break    end

        select @iStart = @iEnd + 1
    end

if exists
    (
        select *
            from dbo.GMSMissionSystemUniqueLog as a with (nolock)
            join @PAccomplishMission as b
                on a.MainMissionID = b.MainMissionID
                and a.LoginUID = @iLoginUID_
    )
    begin    select @iOK = -5    goto end_proc    end
----
if exists
    (
        select *
            from dbo.GMSMissionSystemMain with (nolock)
                where LoginUID = @iLoginUID_
                and MainMissionID = @iMainMissionID_
    )
    begin    select @iOK = -6    goto end_proc    end
----
if exists
    (
        select *
            from dbo.GMSMissionSystemUniqueLog with (nolock)
                where LoginUID = @iLoginUID_
                and MainMissionID = @iMainMissionID_
    )
    begin    select @iOK = -7    goto end_proc    end
----
declare @SubMission table
(
    SubMissionID int not null
,   ProgressRate smallint not null
--,   primary key clustered
--    (
--        SubMissionID
--    )
)

select  @iStart = 1
,       @iEnd = 1

while right(@strSubMission_, 1) = @strToken
    begin
        select @strSubMission_ = substring(@strSubMission_, 1, Len(@strSubMission_) - 1)
    end

while 1 = 1
    begin
        select @iEnd = charindex(@strToken, @strSubMission_, @iStart + 1)

        if @iEnd = 0
            begin    select @iEnd = 4000    end

        insert into @SubMission
                (
                    SubMissionID
,                   ProgressRate
                )
            select  substring(@strSubMission_, @iStart, @iEnd - @iStart)
,                   0
                where len(@strSubMission_) > 0
        if @@error <> 0
            begin    select @iOK = -8    goto end_proc    end

        if @iEnd = 4000
            begin    break    end

        select @iStart = @iEnd + 1
    end
----
declare @sdtDate smalldatetime
select  @sdtDate = getdate()

begin tran
    insert into dbo.GMSMissionSystemMain with (updlock)
            (
                LoginUID
,               MainMissionID
,               RegDate
,               LimitDate
            )
        select  @iLoginUID_
,               @iMainMissionID_
,               @sdtDate
,               dateadd(dd, @iLimitDay_, @sdtDate)
    if @@error <> 0
        begin    select @iOK = -101, @iError = @@error    goto fail_tran    end

    insert into dbo.GMSMissionSystemSub with (updlock)
            (
                LoginUID
,               MainMissionID
,               SubMissionID
,               ProgressRate
            )
        select  @iLoginUID_
,               @iMainMissionID_
,               SubMissionID
,               ProgressRate
            from @SubMission
    if @@error <> 0
        begin    select @iOK = -102, @iError = @@error    goto fail_tran    end
commit tran

goto end_proc
----
fail_tran:
rollback tran

end_proc:
select @iOK


