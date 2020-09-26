CREATE PROCEDURE [dbo].[gup_update_community_opt]
    @iUnitUID_			[bigint],
	@iInsertFriendDeny_	[tinyint],
	@iManToManChatDeny_ [tinyint],
	@iPartyDeny_		[tinyint],
	@iPrivateDealDeny_	[tinyint],
    @iOK        [int] = 0
AS

SET NOCOUNT ON;

-- 해당 Unit이 없는 경우 
IF NOT EXISTS ( select * from dbo.GUnit with (nolock) where UnitUID = @iUnitUID_ )
	BEGIN  select @iOK = -1	goto END_PROC END

-- Insert
IF NOT EXISTS( SELECT * FROM dbo.GDenyOption WITH( NOLOCK ) WHERE UnitUID = @iUnitUID_ )
	BEGIN
		BEGIN TRAN
			INSERT INTO dbo.GDenyOption with (updlock) ( UnitUID, QuestionNo, CodeNo )
				SELECT  @iUnitUID_, 1, @iInsertFriendDeny_
				union all
				SELECT  @iUnitUID_, 2, @iManToManChatDeny_
				union all
				SELECT  @iUnitUID_, 3, @iPartyDeny_
				union all
				SELECT  @iUnitUID_, 4, @iPrivateDealDeny_
	
		IF @@ERROR <> 0 OR @@ROWCOUNT <> 4
			BEGIN    
				SELECT @iOK = -2
				GOTO FAIL_TRAN
			END
		COMMIT TRAN
	END

-- Update
ELSE
	BEGIN
		BEGIN TRAN
			UPDATE dbo.GDenyOption WITH(updlock)
				SET CodeNo = 
					CASE questionNo 
						when 1 then @iInsertFriendDeny_ 
						when 2 then @iManToManChatDeny_ 
						when 3 then @iPartyDeny_ 
						when 4 then @iPrivateDealDeny_ 
					END
				WHERE UnitUID = @iUnitUID_
				
			IF @@ERROR <> 0 OR @@ROWCOUNT <> 4
			BEGIN
				SELECT @iOK = -3
				GOTO FAIL_TRAN
			END

		COMMIT TRAN
	END

GOTO END_PROC

FAIL_TRAN:
ROLLBACK TRAN

END_PROC:
SELECT @iOK


