CREATE PROCEDURE [dbo].[Get_ELInfo_for_Web]
	@iOK_			[int] = 0
AS 

/* 
sutamtwo
작성일 : 2007년 12월 13일
사용 : 엘을 찾아라 이벤트 현황
*/
SET NOCOUNT ON;
SET XACT_ABORT ON;

BEGIN TRAN

	truncate table dbo.Get_EL_Info
	truncate table dbo.Get_Super_EL_Info

	-- 엘 조각
	insert into dbo.Get_EL_Info(userid,quantity)
		select d.Userid, sum(b.quantity) as Quantity--b.quantity 
			from game01.dbo.gitem as a with (nolock), 
					game01.dbo.gitemquantity as b with (nolock), 
						game01.dbo.gunit as c with (nolock),
							account.dbo.muser as d with (nolock)
				where a.itemid = 129000 and a.deleted = 0 and a.itemuid = b.itemuid and a.unituid = c.unituid and c.useruid = d.useruid
					group by  d.userid order by quantity desc	

		IF @@error <> 0
			BEGIN	SELECT @iOK_ = -103	GOTO FAIL_TRAN	END

	-- 슈퍼 엘 
	insert into dbo.Get_Super_EL_Info(userid,quantity)
		select d.Userid, sum(b.quantity) as Quantity--b.quantity 
			from game01.dbo.gitem as a with (nolock), 
					game01.dbo.gitemquantity as b with (nolock), 
						game01.dbo.gunit as c with (nolock),
							account.dbo.muser as d with (nolock)
				where a.itemid = 129010 and a.deleted = 0 and a.itemuid = b.itemuid and a.unituid = c.unituid and c.useruid = d.useruid
					group by  d.userid order by quantity desc	


		IF @@error <> 0
			BEGIN	SELECT @iOK_ = -103	GOTO FAIL_TRAN	END

COMMIT TRAN

GOTO END_PROC

FAIL_TRAN:
ROLLBACK TRAN

END_PROC:

SELECT @iOK_


