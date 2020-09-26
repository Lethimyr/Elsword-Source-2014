CREATE PROCEDURE [dbo].[gup_insert_post_item_new]
	@FromUnitUID	bigint	
,	@ToUnitNickName	nvarchar(16)
,	@Quantity		int		
,	@ScType			tinyint	
,	@ScIndex		int
,	@Title			nvarchar(38)
,	@Message		nvarchar(800)
,	@ELevel			tinyint
,	@Socket1		smallint 
,	@Socket2		smallint 
,	@Socket3		smallint 
,	@Socket4		smallint 
,	@Attribute1		tinyint
,	@Attribute2		tinyint
,	@iOK			int = 0

AS
SET NOCOUNT ON;

DECLARE @sdtNow	smalldatetime
,		@PostNo	bigint
,		@ToUnitUID	bigint
,		@Option	int
SELECT	@sdtNow	= GETDATE()

SELECT @Option = (@ELevel + @Socket1 + @Socket2 + @Socket3 + @Socket4 + @Attribute1 + @Attribute2)

SELECT @ToUnitUID = UnitUID
	FROM dbo.GunitNickName WITH (NOLOCK)
		WHERE NickName = @ToUnitNickName
			OPTION (MAXDOP 1)

/*

해당 부분은 따로 뺄꺼임 [dbo].[gup_check_insert_post_item]

-- 해당 닉네임이 존재하지 않으면 종료
IF @ToUnitUID IS NULL
	BEGIN	SELECT	@iOK = -99	GOTO END_PROC	END

-- 일반 우편이 30개 넘을 때
IF (SELECT COUNT(*) FROM dbo.GPost WITH (NOLOCK) WHERE ToUnitUID = @ToUnitUID AND SCType = 1 AND Deleted = 0) > 29
	BEGIN	SELECT	@iOK = -98	GOTO END_PROC	END

-- 상대방의 블랙 리스트에 내가 등록 되어있다면
IF EXISTS (SELECT * FROM dbo.GPost_BlackList WITH (NOLOCK) WHERE MyUID = @ToUnitUID AND BlockUID = @FromUnitUID)
	BEGIN	SELECT	@iOK = -97	GOTO END_PROC	END

*/

BEGIN TRAN
	
	-- 우체국에 넣기
	INSERT INTO dbo.GPost (FromUnitUID,	ToUnitUID, Quantity, ScType, ScIndex, RegDate, DelDate,	Title, Message)
		SELECT @FromUnitUID, @ToUnitUID, @Quantity, @ScType, @ScIndex, @sdtNow, @sdtNow, @Title, @Message
	IF @@ERROR <> 0 OR @@ROWCOUNT <> 1
		BEGIN	SELECT	@iOK = -1	GOTO FAIL_TRAN	END

	-- PostNO 가져오기
	SELECT @PostNo = @@IDENTITY
		

	-- 기타 옵션이 하나도 없을 경우 종료
	IF @Option = 0
		BEGIN	GOTO END_	END


	-- 강화에 해당된다면,
	IF @ELevel > 0
		BEGIN
			INSERT INTO dbo.GPost_Enchant (PostNo, ELevel)
				SELECT @PostNo, @ELevel
			IF @@ERROR <> 0 OR @@ROWCOUNT <> 1
				BEGIN	SELECT	@iOK = -2	GOTO FAIL_TRAN	END
		END
		
	-- 소켓에 해당된다면,
	IF (@Socket1 + @Socket2 + @Socket3 + @Socket4) > 0
		BEGIN
			INSERT INTO dbo.GPost_Socket (PostNo, Socket1, Socket2, Socket3, Socket4)
				SELECT @PostNo, @Socket1 , @Socket2 , @Socket3 , @Socket4
			IF @@ERROR <> 0 OR @@ROWCOUNT <> 1
				BEGIN	SELECT	@iOK = -3	GOTO FAIL_TRAN	END
		END

	-- 속성에 해당된다면,
	IF (@Attribute1 + @Attribute2) > 0
		BEGIN
			INSERT INTO GPost_Attribute (PostNo, Attribute1, Attribute2)
				SELECT @PostNo, @Attribute1 , @Attribute2
			IF @@ERROR <> 0 OR @@ROWCOUNT <> 1
				BEGIN	SELECT	@iOK = -4	GOTO FAIL_TRAN	END
		END

END_:
COMMIT TRAN

GOTO END_PROC

FAIL_TRAN:
ROLLBACK TRAN

END_PROC:
SELECT @iOK, @PostNo, @ToUnitUID, @sdtNow


