CREATE PROCEDURE [dbo].[CGGA_contribution_gp_select_top5]
AS
set nocount on
set transaction isolation level read uncommitted

/*
20081201. mk8253.

{ call dbo.CGGA_contribution_gp_select_top5 }

{ call dbo.CGGA_contribution_gp_select_top5 }
인자없음

5 return ( @1 )
@1 ; NickName nvarchar(24)
@2 ; Contribution int
*/
SELECT
    c.nick
,   a.Contribution
from
(
    SELECT TOP 5
        LoginUID
    ,   Contribution
    FROM dbo.CGGAContributionGP
    ORDER BY Contribution DESC
) AS a
JOIN dbo.users AS b
ON a.LoginUID = b.LoginUID
JOIN dbo.NickNames AS c
ON b.Login = c.LOGIN


