CREATE PROCEDURE [dbo].[up_schedule_statistics]
AS
/*
매일 1회
*/

SET NOCOUNT ON
SET XACT_ABORT ON

DECLARE @sdtMidnight smalldatetime
SELECT @sdtMidnight = CONVERT(NVARCHAR(10), GetDate(), 120)

--유저별 카운팅
declare @iDailyNewUsers int
,       @iWeeklyNewUsers int
,       @iMonthlyNewUsers int
,       @iDailyUniqueUsers int
,       @iWeeklyUniqueUsers int
,       @iMonthlyUniqueUsers int
,       @iTotalUsers int
,       @iDailyNewUserCount int
,       @iWeeklyNewUserCount int
,       @iMonthlyNewUserCount int
,       @iDailyUniqueUserCount int
,       @iWeeklyUniqueUserCount int
,       @iMonthlyUniqueUserCount int
,       @iTotalUserCount int

select  @iDailyNewUsers = 0
,       @iWeeklyNewUsers = 0
,       @iMonthlyNewUsers = 0
,       @iDailyUniqueUsers = 0
,       @iWeeklyUniqueUsers = 0
,       @iMonthlyUniqueUsers = 0
,       @iTotalUsers = 0
,       @iDailyNewUserCount = 0
,       @iWeeklyNewUserCount = 0
,       @iMonthlyNewUserCount = 0
,       @iDailyUniqueUserCount = 0
,       @iWeeklyUniqueUserCount = 0
,       @iMonthlyUniqueUserCount = 0
,       @iTotalUserCount = 0

select  @iDailyNewUsers = sum(case when dateadd(dd, -1, @sdtMidnight) <= FirstLogin and FirstLogin < @sdtMidnight then 1 else 0 end)
,       @iWeeklyNewUsers = sum(case when dateadd(wk, -1, @sdtMidnight) <= FirstLogin and FirstLogin < @sdtMidnight then 1 else 0 end)
,       @iMonthlyNewUsers = sum(case when dateadd(mm, -1, @sdtMidnight) <= FirstLogin and FirstLogin < @sdtMidnight then 1 else 0 end)
,       @iDailyUniqueUsers = sum(case when dateadd(dd, -1, @sdtMidnight) <= LastLogin and LastLogin < @sdtMidnight then 1 else 0 end)
,       @iWeeklyUniqueUsers = sum(case when dateadd(wk, -1, @sdtMidnight) <= LastLogin and LastLogin < @sdtMidnight then 1 else 0 end)
,       @iMonthlyUniqueUsers = sum(case when dateadd(mm, -1, @sdtMidnight) <= LastLogin and LastLogin < @sdtMidnight then 1 else 0 end)
,       @iTotalUsers = sum(case when LoginUID is not null then 1 else 0 end)
,       @iDailyNewUserCount = sum(case when dateadd(dd, -1, @sdtMidnight) <= FirstLogin and FirstLogin < @sdtMidnight and IPAddress <> N'0.0.0.0' then 1 else 0 end)
,       @iWeeklyNewUserCount = sum(case when dateadd(wk, -1, @sdtMidnight) <= FirstLogin and FirstLogin < @sdtMidnight and IPAddress <> N'0.0.0.0' then 1 else 0 end)
,       @iMonthlyNewUserCount = sum(case when dateadd(mm, -1, @sdtMidnight) <= FirstLogin and FirstLogin < @sdtMidnight and IPAddress <> N'0.0.0.0' then 1 else 0 end)
,       @iDailyUniqueUserCount = sum(case when dateadd(dd, -1, @sdtMidnight) <= LastLogin and LastLogin < @sdtMidnight and IPAddress <> N'0.0.0.0' then 1 else 0 end)
,       @iWeeklyUniqueUserCount = sum(case when dateadd(wk, -1, @sdtMidnight) <= LastLogin and LastLogin < @sdtMidnight and IPAddress <> N'0.0.0.0' then 1 else 0 end)
,       @iMonthlyUniqueUserCount = sum(case when dateadd(mm, -1, @sdtMidnight) <= LastLogin and LastLogin < @sdtMidnight and IPAddress <> N'0.0.0.0' then 1 else 0 end)
,       @iTotalUserCount = sum(case when LoginUID is not null and IPAddress <> N'0.0.0.0' then 1 else 0 end)
    from dbo.Users with (nolock)
        option (maxdop 1)

--금일 동접 산출
DECLARE @iDailyMaxUsers   int,
        @iDailyAvgUsers   int,
        @iWeeklyAvgUsers  int,
        @iMonthlyAvgUsers int

SELECT @iDailyMaxUsers   = 0,
       @iDailyAvgUsers   = 0,
       @iWeeklyAvgUsers  = 0,
       @iMonthlyAvgUsers = 0

SELECT @iDailyMaxUsers   = IsNull(Max(CASE WHEN DateAdd(dd, -1, @sdtMidnight) <= [Date] AND [Date] < @sdtMidnight THEN numUsers END), 0),
       @iDailyAvgUsers   = IsNull(Avg(CASE WHEN DateAdd(dd, -1, @sdtMidnight) <= [Date] AND [Date] < @sdtMidnight THEN numUsers END), 0),
       @iWeeklyAvgUsers  = IsNull(Avg(CASE WHEN DateAdd(wk, -1, @sdtMidnight) <= [Date] AND [Date] < @sdtMidnight THEN numUsers END), 0),
       @iMonthlyAvgUsers = IsNull(Avg(numUsers), 0)
    FROM dbo.TotalConnectUsers(NOLOCK)
        WHERE DateAdd(mm, -1, @sdtMidnight) <= [Date] AND [Date] < @sdtMidnight

--최고동접시각
DECLARE @sdtMaxTime smalldatetime
SELECT @sdtMaxTime = @sdtMidnight
SELECT TOP 1 @sdtMaxTime = IsNull([Date], @sdtMidnight)
    FROM dbo.TotalConnectUsers(NOLOCK)
        WHERE DateAdd(dd, -1, @sdtMidnight) <= [Date] AND [Date] < @sdtMidnight AND numUsers = @iDailyMaxUsers

--기록
insert into dbo.StatStatistics with (updlock)
        (
            RegDate
,           MaxTime
,           DailyMaxUsers
,           TotalUsers
,           DailyNewUsers
,           WeeklyNewUsers
,           MonthlyNewUsers
,           DailyUniqueUsers
,           WeeklyUniqueUsers
,           MonthlyUniqueUsers
,           DailyAvgUsers
,           WeeklyAvgUsers
,           MonthlyAvgUsers
,           TotalUserCount
,           DailyNewUserCount
,           WeeklyNewUserCount
,           MonthlyNewUserCount
,           DailyUniqueUserCount
,           WeeklyUniqueUserCount
,           MonthlyUniqueUserCount
        )
    select  @sdtMidnight
,           @sdtMaxTime
,           @iDailyMaxUsers
,           @iTotalUsers
,           @iDailyNewUsers
,           @iWeeklyNewUsers
,           @iMonthlyNewUsers
,           @iDailyUniqueUsers
,           @iWeeklyUniqueUsers
,           @iMonthlyUniqueUsers
,           @iDailyAvgUsers
,           @iWeeklyAvgUsers
,           @iMonthlyAvgUsers
,           @iTotalUserCount
,           @iDailyNewUserCount
,           @iWeeklyNewUserCount
,           @iMonthlyNewUserCount
,           @iDailyUniqueUserCount
,           @iWeeklyUniqueUserCount
,           @iMonthlyUniqueUserCount

--microcat. 20071121. 유저 플레이 타임 기록
insert into dbo.StatPlayTime(UserClass, TotalPlayTime, ConnectCount, UserCount, RegDate)
    select Grade, sum(PlayTime), count(*), count(distinct LoginUID), @sdtMidnight
        from dbo.vSUTUserTrace with (nolock)
            where dateadd(dd, -1, @sdtMidnight) <= EndDate AND EndDate < @sdtMidnight
                group by Grade
                    option (maxdop 1)

--PC방 IP별 카운팅 기록
INSERT INTO dbo.StatPCBangIPCount(IP, TotalPlayTime, ConnectCount, UserCount, RegDate)
    SELECT IP, SUM(PlayTime), COUNT(ID), COUNT(DISTINCT Login), @sdtMidnight
        FROM dbo.UserPCBangCount(NOLOCK)
            WHERE DateAdd(dd, -1, @sdtMidnight) <= ConnectTime AND ConnectTime < @sdtMidnight
                GROUP BY IP

--PC방 아이디별 카운팅 기록
INSERT INTO dbo.StatPCBangLoginCount(Login, TotalPlayTime, ConnectCount, RegDate)
    SELECT Login, SUM(PlayTime), COUNT(ID), @sdtMidnight
        FROM dbo.UserPCBangCount(NOLOCK)
            WHERE DateAdd(dd, -1, @sdtMidnight) <= ConnectTime AND ConnectTime < @sdtMidnight
                GROUP BY Login
                    OPTION (MAXDOP 1)

--모드카운트 기록
INSERT INTO dbo.StatModeCount(RegDate, ModeCode, ModeCount)
    SELECT @sdtMidnight, ModeCode, ModeCount
        FROM dbo.UserModeCount --신뢰성 있는 데이터 이동을 위해서 nolock을 생략함

--모드카운트 초기화
truncate table dbo.UserModeCount

--microcat. 20071011. 빌링 에러 통계 기록
insert into dbo.BEStatBillingErrorCount with (updlock)
        (
            RegDate
,           ErrorName
,           [Count]
        )
    select  dateadd(dd, -1, @sdtMidnight)
,           ErrorName
,           [Count]
        from dbo.BEDataBillingErrorCount with (updlock)

--microcat. 20071011. 빌링 에러 통계 삭제
truncate table dbo.BEDataBillingErrorCount

--microcat. 20071121. 신규유저 접속유지수(Stat)
insert into Stat.dbo.DataNewUser(FirstLogin, LastLogin, LoginCount)
    select convert(nvarchar(10), a.FirstLogin, 120), dateadd(dd, -1, @sdtMidnight), count(*)
        from dbo.Users as a with (nolock)
            where DateAdd(dd, -31, @sdtMidnight) <= a.FirstLogin AND a.FirstLogin < @sdtMidnight
                and exists(select * from dbo.vSUTUserTrace as b with (nolock) where a.LoginUID = b.LoginUID AND dateadd(dd, -1, @sdtMidnight) <= EndDate AND EndDate < @sdtMidnight)
                    group by convert(nvarchar(10), a.FirstLogin, 120)
                        option (MAXDOP 1)

--유실유저수 기록(Stat)
INSERT INTO Stat.dbo.DataLostUser
        (
            RegDate
,           CharType
,           Level
,           Type
,           NumCount
        )
    SELECT  DateAdd(dd, -1, @sdtMidnight)
,           a.CharType
,           a.Level
,           CASE
                WHEN DateADD(wk, -1, @sdtMidnight) <= b.LastLogin AND b.LastLogin < @sdtMidnight THEN 1
                WHEN DateADD(wk, -3, @sdtMidnight) <= b.LastLogin AND b.LastLogin < DateADD(wk, -1, @sdtMidnight) THEN 2
                WHEN DateADD(mm, -3, @sdtMidnight) <= b.LastLogin AND b.LastLogin < DateADD(wk, -3, @sdtMidnight) THEN 3
                ELSE 4
            END
,           Count(*)
        FROM dbo.Characters as a WITH (NOLOCK)
            JOIN dbo.Users as b WITH (NOLOCK)
                ON a.Login = b.Login
                    GROUP BY    a.CharType
,                               a.Level
,                               CASE
                                    WHEN DateADD(wk, -1, @sdtMidnight) <= b.LastLogin AND b.LastLogin < @sdtMidnight THEN 1
                                    WHEN DateADD(wk, -3, @sdtMidnight) <= b.LastLogin AND b.LastLogin < DateADD(wk, -1, @sdtMidnight) THEN 2
                                    WHEN DateADD(mm, -3, @sdtMidnight) <= b.LastLogin AND b.LastLogin < DateADD(wk, -3, @sdtMidnight) THEN 3
                                    ELSE 4
                                END
                        OPTION (MAXDOP 1)

--복귀유저수 기록(Stat)
INSERT INTO Stat.dbo.DataReversionUser
        (
            RegDate
,           CharType
,           Level
,           Type
,           NumCount
        )
    SELECT  DateAdd(dd, -1, @sdtMidnight)
,           a.CharType
,           a.level
,           CASE
                WHEN DateADD(wk, -1, @sdtMidnight) <= c.RegDate AND c.RegDate < DateADD(dd, -1, @sdtMidnight) THEN 1
                WHEN DateADD(wk, -3, @sdtMidnight) <= c.RegDate AND c.RegDate < DateADD(wk, -1, @sdtMidnight) THEN 2
                WHEN DateADD(mm, -3, @sdtMidnight) <= c.RegDate AND c.RegDate < DateADD(wk, -3, @sdtMidnight) THEN 3
                ELSE 4
            END
,           Count(*)
        FROM dbo.Characters as a WITH (NOLOCK)
            JOIN
                (
                    SELECT  Login
,                           LoginUID
                        FROM dbo.Users WITH (NOLOCK)
                            WHERE DateAdd(dd, -1, @sdtMidnight) <= LastLogin AND LastLogin < @sdtMidnight
                ) as b
                ON a.Login = b.Login
            LEFT OUTER JOIN
                (
                    SELECT  LoginUID
,                           MAX(RegDate) as RegDate
                        FROM Stat.dbo.DataLastLogin WITH (NOLOCK)
                            WHERE DateAdd(mm, -3, @sdtMidnight) <= RegDate AND RegDate < DateAdd(dd, -1, @sdtMidnight)
                                GROUP BY LoginUID
                ) as c
                ON b.LoginUID = c.LoginUID
                    GROUP BY    a.CharType
,                               a.Level
,                               CASE
                                    WHEN DateADD(wk, -1, @sdtMidnight) <= c.RegDate AND c.RegDate < DateADD(dd, -1, @sdtMidnight) THEN 1
                                    WHEN DateADD(wk, -3, @sdtMidnight) <= c.RegDate AND c.RegDate < DateADD(wk, -1, @sdtMidnight) THEN 2
                                    WHEN DateADD(mm, -3, @sdtMidnight) <= c.RegDate AND c.RegDate < DateADD(wk, -3, @sdtMidnight) THEN 3
                                    ELSE 4
                                END
                        OPTION (MAXDOP 1)


