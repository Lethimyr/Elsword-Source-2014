CREATE PROCEDURE [dbo].[CGGA_contribution_gp_select_total]
AS
set nocount on
set transaction isolation level read uncommitted

/*
20081201. mk8253.

{ call dbo.CGGA_contribution_gp_select_total }

{ call dbo.CGGA_contribution_gp_select_total }
인자없음

1 return ( @1 )
@1 ; TotalContributionGP bigint
*/

SELECT
    SUM(CONVERT(BIGINT, Contribution))
FROM dbo.CGGAContributionGP


