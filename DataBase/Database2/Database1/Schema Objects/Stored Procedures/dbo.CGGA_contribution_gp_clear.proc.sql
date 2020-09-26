CREATE PROCEDURE [dbo].[CGGA_contribution_gp_clear]
	@iOK [int] = 0
AS
set nocount on
set transaction isolation level read uncommitted

/*
20081107. microcat.

{ call dbo.CGGA_contribution_gp_clear }

{ call dbo.CGGA_contribution_gp_clear }
인자없음

1 return ( @1 )
@1 ; OK int

OK
0 ; 성공
-101이하 ; 트랜잭션 에러
*/

truncate table dbo.CGGAContributionGP


