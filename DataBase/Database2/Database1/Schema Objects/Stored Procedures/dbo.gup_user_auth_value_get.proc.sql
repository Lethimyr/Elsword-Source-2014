CREATE PROCEDURE [dbo].[gup_user_auth_value_get]
	@strLogin_ [nvarchar](20)
AS
/*microcat
작성일 : 2007년 5월 30일
사용 : 유저 인증값을 주자
*/
set nocount on;

declare @iLoginUID int
select  @iLoginUID = LoginUID
    from dbo.Users with (nolock)
        where Login = @strLogin_

select  Value
    from dbo.GUserAuthValue with (nolock)
        where LoginUID = @iLoginUID


