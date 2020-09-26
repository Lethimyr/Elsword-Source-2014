CREATE PROCEDURE [dbo].[up_schedule_indigo_reset]
AS
SET NOCOUNT ON
SET XACT_ABORT ON

--백업된 테이블이 존재하면 더이상 만들지 않는다.
DECLARE @strNow nvarchar(6)
,       @strIndigoUserBackup nvarchar(32)
,       @strIndigoRankBackup nvarchar(32)
SELECT  @strNow = CONVERT(nvarchar(6), GetDate(), 12)
SELECT  @strIndigoUserBackup = N'dbo.IndigoUser_' + @strNow
,       @strIndigoRankBackup = N'dbo.IndigoRank_' + @strNow
IF NOT EXISTS(SELECT * FROM dbo.SysObjects(NOLOCK)
                WHERE id in (
                                object_id(N'dbo.IndigoUser')
,                               object_id(N'dbo.IndigoRank')
,                               object_id(N'dbo.IndigoSupplyItemByRank')
,                               object_id(N'dbo.IndigoSupplyType')
,                               object_id(N'dbo.IndigoSupplyItemByRP')
,                               object_id(N'dbo.IndigoSupplyLog')
,                               object_id(N'dbo.IndigoBesta')
                            )
                    and OBJECTPROPERTY(id, N'IsUserTable') = 1
            )
    OR
    EXISTS(SELECT * FROM dbo.SysObjects(NOLOCK)
                WHERE id in (

                                object_id(@strIndigoUserBackup)
,                               object_id(@strIndigoRankBackup)
                            )
                    and OBJECTPROPERTY(id, N'IsUserTable') = 1
            )
        BEGIN
            PRINT N'Table Error'
            RETURN
        END

DECLARE @strSQL nvarchar(4000)
SELECT @strSQL = N'SELECT LoginUID, RP, Win, Lose, WinRate, Rank INTO ' + @strIndigoUserBackup + N' FROM dbo.IndigoUser(NOLOCK); '
SELECT @strSQL = @strSQL + N'SELECT Rank, Login, Nick, LoginUID, RP, Win, Lose, WinRate, Change INTO ' + @strIndigoRankBackup + N' FROM dbo.IndigoRank(NOLOCK); '
SELECT @strSQL = @strSQL + N'ALTER TABLE ' + @strIndigoUserBackup + N' ADD Constraint PKC_IndigoUser_' + @strNow + N' PRIMARY KEY CLUSTERED (LoginUID) WITH FILLFACTOR = 80; '
SELECT @strSQL = @strSQL + N'ALTER TABLE ' + @strIndigoRankBackup + N' ADD Constraint PKC_IndigoRank_' + @strNow + N' PRIMARY KEY CLUSTERED (Rank) WITH FILLFACTOR = 80;'
exec dbo.sp_executesql @strSQL

DECLARE @IndigoSupplyItemByRank TABLE
(
    [No] int not null identity(1, 1) primary key
,   CharType tinyint not null
,   StartRank int not null
,   EndRank int not null
,   SupplyType int not null
)

INSERT INTO @IndigoSupplyItemByRank(CharType, StartRank, EndRank, SupplyType)
    SELECT CharType, StartRank, EndRank, SupplyType
        FROM dbo.IndigoSupplyItemByRank(NOLOCK)

DECLARE @iLoop1 int, @iLoopMax1 int
SELECT @iLoop1 = 1, @iLoopMax1 = MAX([No]) FROM @IndigoSupplyItemByRank
WHILE @iLoop1 <= @iLoopMax1
    BEGIN
        DECLARE @iCharType1 tinyint, @iStartRank int, @iEndRank int, @iSupplyType1 int
        SELECT  @iCharType1 = CharType
,               @iStartRank = StartRank
,               @iEndRank = EndRank
,               @iSupplyType1 = SupplyType
            FROM @IndigoSupplyItemByRank
                WHERE [No] = @iLoop1

        DECLARE @strP1 nvarchar(4000)
        SELECT @strP1 = N''

        --아이템은 유저당 최대 181개까지... 이렇게 주는 경우는 없겠지...
        SELECT @strP1 = @strP1 + CAST(ItemID as nvarchar(10)) + N',' + CAST(Factor as nvarchar(10)) + N','
            FROM dbo.IndigoSupplyType(NOLOCK)
                WHERE SupplyType = @iSupplyType1

        DECLARE @IndigoSupplyUserByRank TABLE
        (
            [No] int not null identity(1, 1) primary key
,           [Login] nvarchar(20) not null
,           LoginUID int not null
        )

        DELETE FROM @IndigoSupplyUserByRank

        INSERT INTO @IndigoSupplyUserByRank([Login], LoginUID)
            SELECT a.[Login], a.LoginUID
                FROM dbo.IndigoRank as a(NOLOCK)
                    WHERE @iStartRank <= a.Rank AND a.Rank <= @iEndRank
                        AND (@iCharType1 = 255 OR @iCharType1 = (SELECT TOP 1 b.CharType FROM dbo.Characters as b(NOLOCK) WHERE a.[Login] = b.[Login] AND b.CharType BETWEEN 0 AND 6 ORDER BY b.[EXP] DESC))

        DECLARE @iLoop2 int, @iLoopMax2 int, @iLoopLimit2 int
        SELECT @iLoop2 = MIN([No]), @iLoopMax2 = MAX([No]), @iLoopLimit2 = 150 FROM @IndigoSupplyUserByRank
        WHILE @iLoop2 <= @iLoopMax2
            BEGIN
                DECLARE @strU1 nvarchar(4000)
                SELECT @strU1 = N''
                SELECT @strU1 = @strU1 + [Login] + N','
                    FROM @IndigoSupplyUserByRank
                        WHERE [No] BETWEEN @iLoop2 AND @iLoop2 + @iLoopLimit2 - 1

                DECLARE @iOK1 int
                EXEC @iOK1 = dbo.up_insert_item @strU1, @strP1, N',', 0

                INSERT INTO dbo.IndigoSupplyLog(LoginUID, SupplyType, OK)
                    SELECT LoginUID, @iSupplyType1, @iOK1
                        FROM @IndigoSupplyUserByRank
                            WHERE [No] BETWEEN @iLoop2 AND @iLoop2 + @iLoopLimit2 - 1

                SELECT @iLoop2 = @iLoop2 + @iLoopLimit2
            END
        SELECT @iLoop1 = @iLoop1 + 1
    END

DECLARE @IndigoSupplyItemByRP TABLE
(
    [No] int not null identity(1, 1) primary key
,   CharType tinyint not null
,   StartRP int not null
,   EndRP int not null
,   SupplyType int not null
)

INSERT INTO @IndigoSupplyItemByRP(CharType, StartRP, EndRP, SupplyType)
    SELECT CharType, StartRP, EndRP, SupplyType
        FROM dbo.IndigoSupplyItemByRP(NOLOCK)

DECLARE @iLoop3 int, @iLoopMax3 int
SELECT @iLoop3 = 1, @iLoopMax3 = MAX([No]) FROM @IndigoSupplyItemByRP
WHILE @iLoop3 <= @iLoopMax3
    BEGIN
        DECLARE @iCharType3 tinyint, @iStartRP int, @iEndRP int, @iSupplyType3 int
        SELECT  @iCharType3 = CharType
,               @iStartRP = StartRP
,               @iEndRP = EndRP
,               @iSupplyType3 = SupplyType
            FROM @IndigoSupplyItemByRP
                WHERE [No] = @iLoop3

        DECLARE @strP3 nvarchar(4000)
        SELECT @strP3 = N''

        --아이템은 유저당 최대 181개까지... 이렇게 주는 경우는 없겠지...
        SELECT @strP3 = @strP3 + CAST(ItemID as nvarchar(10)) + N',' + CAST(Factor as nvarchar(10)) + N','
            FROM dbo.IndigoSupplyType(NOLOCK)
                WHERE SupplyType = @iSupplyType3

        DECLARE @IndigoSupplyUserByRP TABLE
        (
            [No] int not null identity(1, 1) primary key
,           [Login] nvarchar(20) not null
,           LoginUID int not null
        )

        DELETE FROM @IndigoSupplyUserByRP

        INSERT INTO @IndigoSupplyUserByRP([Login], LoginUID)
            SELECT a.[Login], a.LoginUID
                FROM dbo.IndigoRank as a(NOLOCK)
                    WHERE @iStartRP <= a.RP AND a.RP <= @iEndRP
                        AND (@iCharType3 = 255 OR @iCharType3 = (SELECT TOP 1 b.CharType FROM dbo.Characters as b(NOLOCK) WHERE a.[Login] = b.[Login] AND b.CharType BETWEEN 0 AND 6 ORDER BY b.[EXP] DESC))

        DECLARE @iLoop4 int, @iLoopMax4 int, @iLoopLimit4 int
        SELECT @iLoop4 = MIN([No]), @iLoopMax4 = MAX([No]), @iLoopLimit4 = 150 FROM @IndigoSupplyUserByRP
        WHILE @iLoop4 <= @iLoopMax4
            BEGIN
                DECLARE @strU3 nvarchar(4000)
                SELECT @strU3 = N''
                SELECT @strU3 = @strU3 + [Login] + N','
                    FROM @IndigoSupplyUserByRP
                        WHERE [No] BETWEEN @iLoop4 AND @iLoop4 + @iLoopLimit4 - 1

                DECLARE @iOK3 int
                EXEC @iOK3 = dbo.up_insert_item @strU3, @strP3, N',', 0

                INSERT INTO dbo.IndigoSupplyLog(LoginUID, SupplyType, OK)
                    SELECT LoginUID, @iSupplyType3, @iOK3
                        FROM @IndigoSupplyUserByRP
                            WHERE [No] BETWEEN @iLoop4 AND @iLoop4 + @iLoopLimit4 - 1

                SELECT @iLoop4 = @iLoop4 + @iLoopLimit4
            END
        SELECT @iLoop3 = @iLoop3 + 1
    END

INSERT INTO dbo.IndigoBesta( [Login], Nick, RP, Win, Lose, WinRate )
    SELECT Login, Nick, RP, WIn, Lose, WinRate FROM dbo.IndigoRank(NOLOCK) WHERE Rank = 1

TRUNCATE TABLE dbo.IndigoUser
TRUNCATE TABLE dbo.IndigoRank


