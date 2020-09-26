CREATE PROCEDURE [dbo].[up_schedule_rank_indigo]
AS
SET NOCOUNT ON
SET XACT_ABORT ON

IF NOT EXISTS(SELECT * FROM dbo.SysObjects(NOLOCK)
                WHERE id in (
                                object_id('dbo.IndigoRank_old')
,                               object_id('dbo.IndigoRank')
,                               object_id('dbo.IndigoUser')
,                               object_id('dbo.Users')
,                               object_id('dbo.NickNames')
                            )
                    and OBJECTPROPERTY(id, N'IsUserTable') = 1
            )
    OR
    EXISTS(SELECT * FROM dbo.SysObjects(NOLOCK)
                WHERE id in (
                                object_id('dbo.IndigoRank_Temp')
                            )
                    and OBJECTPROPERTY(id, N'IsUserTable') = 1
            )
        BEGIN
            PRINT 'Table Error'
            RETURN
        END

DECLARE @strSQL nvarchar(4000)
SELECT @strSQL = 'TRUNCATE TABLE dbo.IndigoRank_old; '
SELECT @strSQL = @strSQL + 'DROP INDEX IndigoRank_old.IXUC_IndigoRank_Rank; '
SELECT @strSQL = @strSQL + 'DROP INDEX IndigoRank_old.IXU_IndigoRank_Login; '
SELECT @strSQL = @strSQL + 'DROP INDEX IndigoRank_old.IXU_IndigoRank_LoginUID; '
SELECT @strSQL = @strSQL + 'DROP INDEX IndigoRank_old.IXU_IndigoRank_Nick;'
exec dbo.sp_executesql @strSQL

INSERT INTO dbo.IndigoRank_old([Login], Nick, LoginUID, RP, Win, Lose, WinRate, Change)
    SELECT b.[Login], c.Nick, a.LoginUID, a.RP, a.Win, a.Lose, a.WinRate, 0
        FROM dbo.IndigoUser as a(NOLOCK) 
            JOIN dbo.Users as b(NOLOCK) on a.LoginUID = b.LoginUID
            JOIN dbo.NickNames as c(NOLOCK) on b.[Login] = c.[Login]
                WHERE NOT EXISTS(SELECT * FROM dbo.UAGUserAuth as d WITH (NOLOCK) WHERE a.LoginUID = d.LoginUID AND d.AuthLevel not between -2 and 0)
                    ORDER BY a.RP DESC, a.Win DESC, a.Lose ASC

SELECT @strSQL = 'CREATE UNIQUE CLUSTERED INDEX IXUC_IndigoRank_Rank on dbo.IndigoRank_old(Rank) WITH FILLFACTOR = 80; '
SELECT @strSQL = @strSQL + 'CREATE UNIQUE INDEX IXU_IndigoRank_Login on dbo.IndigoRank_old([Login]) WITH FILLFACTOR = 80; '
SELECT @strSQL = @strSQL + 'CREATE UNIQUE INDEX IXU_IndigoRank_LoginUID on dbo.IndigoRank_old(LoginUID) WITH FILLFACTOR = 80; '
SELECT @strSQL = @strSQL + 'CREATE UNIQUE INDEX IXU_IndigoRank_Nick on dbo.IndigoRank_old(Nick) WITH FILLFACTOR = 80;'
exec dbo.sp_executesql @strSQL

UPDATE b
    SET b.Rank = a.Rank 
        FROM dbo.IndigoRank_old as a(NOLOCK)
            JOIN dbo.IndigoUser as b(NOLOCK) on a.LoginUID = b.LoginUID

UPDATE a
    SET a.Change = b.Rank - a.Rank
        FROM dbo.IndigoRank_old as a(NOLOCK)
            JOIN dbo.IndigoRank as b(NOLOCK) on a.LoginUID = b.LoginUID AND a.Rank < 501

exec dbo.sp_rename 'IndigoRank', 'IndigoRank_Temp'
exec dbo.sp_rename 'IndigoRank_old', 'IndigoRank'
exec dbo.sp_rename 'IndigoRank_Temp', 'IndigoRank_old'

truncate table dbo.GIIIndigoIslandRank
insert into dbo.GIIIndigoIslandRank with (updlock)
        (
            Rank
,           Nick
,           CharType
,           RP
,           Win
,           Lose
,           WinRate
,           Change
        )
    select  a.Rank
,           a.Nick
,           (select top 1 CharType from dbo.Characters as b with (updlock) where a.Login = b.Login order by [EXP] desc)
,           a.RP
,           a.Win
,           a.Lose
,           a.WinRate
,           a.Change
    from dbo.IndigoRank as a with (updlock)
        where a.Rank <= 255
            option (maxdop 1)


