/*	sutamtwo
	2008-10-07
	타이틀 장/탈착 
	넘어오는 타이틀 ID 값이 0 이면 탈착,
	0 이상이면 장착
*/

create PROCEDURE [dbo].[gup_update_title_mission]
	@iUnitUID	bigint
,	@TitleID	smallint
,	@SubMission1 smallint
,	@SubMission2 smallint
,	@SubMission3 smallint
,	@SubMission4 smallint
,	@SubMission5 smallint
,	@iOK  int = 0


AS

SET NOCOUNT ON ;


DECLARE	@iNew	tinyint
SELECT	@iNew = (SELECT COUNT(*) FROM dbo.GTitle_Mission WITH (NOLOCK) WHERE UnitUID = @iUnitUID AND TitleID = @TitleID)

BEGIN TRAN

	-- 미션 INSERT (@iNew = 0)
	IF	(@iNew = 0)
		BEGIN
			INSERT INTO dbo.GTitle_Mission (UnitUID, TitleID, SubMission1, SubMission2, SubMission3, SubMission4, SubMission5)
				SELECT @iUnitUID, @TitleID, @SubMission1, @SubMission2, @SubMission3, @SubMission4, @SubMission5
			IF @@ERROR <> 0 OR @@ROWCOUNT <> 1
				BEGIN	SELECT @iOK = -1	GOTO FAIL_TRAN	END
		END

	-- 미션 UPDATE (@iNew > 0)
	ELSE
		BEGIN
			UPDATE dbo.GTitle_Mission
					SET SubMission1 = @SubMission1
					,	SubMission2 = @SubMission2
					,	SubMission3 = @SubMission3
					,	SubMission4 = @SubMission4
					,	SubMission5 = @SubMission5
						WHERE UnitUID = @iUnitUID AND TitleID = @TitleID
				IF @@ERROR <> 0 OR @@ROWCOUNT > 1
					BEGIN	SELECT @iOK = -2	GOTO FAIL_TRAN	END
		END
	
COMMIT TRAN

GOTO END_PROC

FAIL_TRAN:
ROLLBACK TRAN

END_PROC:
SELECT @iOK


