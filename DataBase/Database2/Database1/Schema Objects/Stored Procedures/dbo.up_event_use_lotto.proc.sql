CREATE PROCEDURE [dbo].[up_event_use_lotto]
	@strLogin_ [nvarchar](20),
	@iLotto1_ [tinyint],
	@iLotto2_ [tinyint],
	@iLotto3_ [tinyint],
	@iLotto4_ [tinyint],
	@iLotto5_ [tinyint]
AS
exec dbo.up_log_set_proc_count 'up_event_use_lotto'

SET NOCOUNT ON

DECLARE @iOK int

-- SELECT -1
-- RETURN

DECLARE @iRound int
IF EXISTS(SELECT TOP 1 * FROM dbo.EventLottoR(NOLOCK) ORDER BY [Round] DESC)
    BEGIN
        SELECT @iRound = MAX([Round]) FROM dbo.EventLottoR(NOLOCK)
    END
ELSE
    BEGIN
        SELECT @iRound = 1
        INSERT INTO dbo.EventLottoR([Round]) VALUES(@iRound)
    END
SET @iRound = (@iRound - 1) * 7
IF GetDate() BETWEEN DateAdd(Day, @iRound, '2005-10-14 17:00') AND DateAdd(Day, @iRound, '2005-10-15 17:00')
    BEGIN    SET @iOK = -1    goto END_PROC    END    --사용시간대가 아님

IF NOT EXISTS( SELECT [Count] FROM dbo.EventLottoU(NOLOCK) WHERE login = @strLogin_ )
    BEGIN    SET @iOK = -2    GOTO END_PROC    END    -- 복권을 한번도 얻어보지도 못한 유저.

DECLARE @iCount    int
SET @iCount = 0
SELECT @iCount = [Count] FROM dbo.EventLottoU(NOLOCK) WHERE login = @strLogin_
IF @iCount = 0
    BEGIN    SET @iOK = -3    GOTO END_PROC    END    -- 남은 복권이 한장도 없음.

IF NOT( (@iLotto1_ BETWEEN 1 AND 12) AND (@iLotto2_ BETWEEN 1 AND 12) AND (@iLotto3_ BETWEEN 1 AND 12)
        AND (@iLotto4_ BETWEEN 1 AND 12) AND (@iLotto5_ BETWEEN 1 AND 12) )
    BEGIN    SET @iOK = -7    goto END_PROC    END    --Lotto 번호를 초과한 녀석이 있음

DECLARE @BeforeSort TABLE
    (   ID       int    IDENTITY(1, 1) NOT NULL PRIMARY KEY,
        Lotto    int    NOT NULL	    )
DECLARE @AfterSort TABLE
    (   ID       int    IDENTITY(1, 1) NOT NULL PRIMARY KEY,
        Lotto    int    NOT NULL                    )

INSERT INTO @BeforeSort(Lotto) VALUES(@iLotto1_)
INSERT INTO @BeforeSort(Lotto) VALUES(@iLotto2_)
INSERT INTO @BeforeSort(Lotto) VALUES(@iLotto3_)
INSERT INTO @BeforeSort(Lotto) VALUES(@iLotto4_)
INSERT INTO @BeforeSort(Lotto) VALUES(@iLotto5_)

IF ( SELECT COUNT(*) FROM @BeforeSort WHERE Lotto = @iLotto1_ ) > 1
        OR ( SELECT COUNT(*) FROM @BeforeSort WHERE Lotto = @iLotto2_ ) > 1
        OR ( SELECT COUNT(*) FROM @BeforeSort WHERE Lotto = @iLotto3_ ) > 1
        OR ( SELECT COUNT(*) FROM @BeforeSort WHERE Lotto = @iLotto4_ ) > 1
    BEGIN    SET @iOK = -4    goto END_PROC    END    --Lotto 번호중 중복 번호가 있음

INSERT INTO @AfterSort(Lotto) SELECT Lotto FROM @BeforeSort ORDER BY Lotto

SELECT @iLotto1_ = y.Lotto1, @iLotto2_ = y.Lotto2, @iLotto3_ = y.Lotto3, @iLotto4_ = y.Lotto4, @iLotto5_ = y.Lotto5
    FROM ( SELECT Lotto1 = SUM(x.Lotto1), Lotto2 = SUM(x.Lotto2), Lotto3 = SUM(x.Lotto3), Lotto4 = SUM(x.Lotto4), Lotto5 = SUM(x.Lotto5)
                FROM ( SELECT   Lotto1 = CASE WHEN ID = 1 THEN Lotto END,
                                Lotto2 = CASE WHEN ID = 2 THEN Lotto END,
                                Lotto3 = CASE WHEN ID = 3 THEN Lotto END,
                                Lotto4 = CASE WHEN ID = 4 THEN Lotto END,
                                Lotto5 = CASE WHEN ID = 5 THEN Lotto END
                            FROM @AfterSort
                    ) AS x
        ) AS y

BEGIN TRAN

    INSERT INTO dbo.EventLottoN(Login, Lotto1, Lotto2, Lotto3, Lotto4, Lotto5, Lotto)
        VALUES(@strLogin_,
                CASE @iLotto1_
                    WHEN 1 THEN '엘리시스'  WHEN 2 THEN '리르' WHEN 3 THEN '아르메' WHEN 4 THEN '라스'
                    WHEN 5 THEN '고르고스'  WHEN 6 THEN 'MC옥동자' WHEN 7 THEN '파투세이' WHEN 8 THEN '엘레나'
                    WHEN 9 THEN '카미키' WHEN 10 THEN '카제아제' WHEN 11 THEN '가도센' WHEN 12 THEN 'MC래이'
                END,
                CASE @iLotto2_
                    WHEN 1 THEN '엘리시스'  WHEN 2 THEN '리르' WHEN 3 THEN '아르메' WHEN 4 THEN '라스'
                    WHEN 5 THEN '고르고스'  WHEN 6 THEN 'MC옥동자' WHEN 7 THEN '파투세이' WHEN 8 THEN '엘레나'
                    WHEN 9 THEN '카미키' WHEN 10 THEN '카제아제' WHEN 11 THEN '가도센' WHEN 12 THEN 'MC래이'
                END,
                CASE @iLotto3_
                    WHEN 1 THEN '엘리시스'  WHEN 2 THEN '리르' WHEN 3 THEN '아르메' WHEN 4 THEN '라스'
                    WHEN 5 THEN '고르고스'  WHEN 6 THEN 'MC옥동자' WHEN 7 THEN '파투세이' WHEN 8 THEN '엘레나'
                    WHEN 9 THEN '카미키' WHEN 10 THEN '카제아제' WHEN 11 THEN '가도센' WHEN 12 THEN 'MC래이'
                END,
                CASE @iLotto4_
                    WHEN 1 THEN '엘리시스'  WHEN 2 THEN '리르' WHEN 3 THEN '아르메' WHEN 4 THEN '라스'
                    WHEN 5 THEN '고르고스'  WHEN 6 THEN 'MC옥동자' WHEN 7 THEN '파투세이' WHEN 8 THEN '엘레나'
                    WHEN 9 THEN '카미키' WHEN 10 THEN '카제아제' WHEN 11 THEN '가도센' WHEN 12 THEN 'MC래이'
                END,
                CASE @iLotto5_
                    WHEN 1 THEN '엘리시스'  WHEN 2 THEN '리르' WHEN 3 THEN '아르메' WHEN 4 THEN '라스'
                    WHEN 5 THEN '고르고스'  WHEN 6 THEN 'MC옥동자' WHEN 7 THEN '파투세이' WHEN 8 THEN '엘레나'
                    WHEN 9 THEN '카미키' WHEN 10 THEN '카제아제' WHEN 11 THEN '가도센' WHEN 12 THEN 'MC래이'
                END,
                dbo.fnCalcLotto(@iLotto1_, @iLotto2_, @iLotto3_, @iLotto4_, @iLotto5_))
    IF @@ERROR <> 0    BEGIN    SET @iOK = -5    GOTO Failed    END

    SET @iCount = @iCount - 1
    IF @iCount < 0    BEGIN    SET @iCount = 0    END
    UPDATE dbo.EventLottoU SET [Count] = @iCount WHERE login = @strLogin_
    IF @@ERROR <> 0    BEGIN    SET @iOK = -6    GOTO Failed    END

COMMIT TRAN
SET @iOK = 0
goto END_PROC

Failed:
ROLLBACK TRAN

END_PROC:
SELECT @iOK
RETURN


