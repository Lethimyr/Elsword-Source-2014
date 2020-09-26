/*	sutamtwo
	OK값
	-1 : 권한 부족
	-2 : 삭제 에러
	-3 : 삭제 기록 작성 에러
*/

CREATE PROCEDURE [dbo].[gup_get_tool_delete_item]
	@iItemUID_	[bigint]
,	@strID_		[nvarchar](20)
,	@strIP_		[nvarchar](15)
,	@iOK		[int] = 0


AS
SET NOCOUNT ON;


declare @iUnitUID bigint
select @iUnitUID = unitUID
	from game01.dbo.gitem with (nolock)
		where itemuid = @iItemUID_

declare @sdtnow smalldatetime
select @sdtnow = getdate()


declare @iauthlevel int
select @iauthlevel = authlevel
	from account.dbo.tuser with (nolock)
		where id = @strID_

if @iauthlevel < 1
	BEGIN	
		select @iOK = -1	
		GOTO END_PROC
	END
else 

BEGIN TRAN
	
	update game01.dbo.gitem
		set deldate = @sdtnow
			where itemuid = @iItemUID_

    IF @@ERROR <> 0 OR @@ROWCOUNT <> 1
        BEGIN    SELECT @iOK = -2    GOTO FAIL_TRAN    END


	insert into account.dbo.DeleteItems (ItemUID, Session, IP)
		select @iItemUID_, @strID_,@strIP_

    IF @@ERROR <> 0 OR @@ROWCOUNT <> 1
        BEGIN    SELECT @iOK = -3    GOTO FAIL_TRAN    END


-- 로그 남기기
	insert into dbo.GItemToolHistory(UnitUID,Itemuid,InsOrDel,session,ip,regdate)
		select @iUnitUID, @iItemUID_, 1, @strID_, @strIP_, @sdtnow
		    IF @@ERROR <> 0 OR @@ROWCOUNT <> 1
				BEGIN
					SELECT @iOK = -14 -- 내구도 아이템 insert Error
					GOTO FAIL_TRAN
				END

COMMIT TRAN

GOTO END_PROC

FAIL_TRAN:
ROLLBACK TRAN

END_PROC:

SELECT @iOK


