CREATE PROCEDURE [dbo].[CGGA_contribution_gp_select_user]
	@iLoginUID_ [int],
	@iOK [int] = 0
AS
set nocount on
set transaction isolation level read uncommitted

/*
20081201. mk8253.

{ call dbo.CGGA_contribution_gp_select_user ( %d ) }

{ call dbo.CGGA_contribution_gp_select_user ( @1 ) }
@1 ; LoginUID int

1 return ( @1 )
@1 ; Contribution int
*/

SELECT Contribution
FROM dbo.CGGAContributionGP
WHERE
    LoginUID = @iLoginUID_


