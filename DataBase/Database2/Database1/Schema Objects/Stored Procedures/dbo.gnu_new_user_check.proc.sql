CREATE PROCEDURE [dbo].[gnu_new_user_check]
	@strLogin_ [nvarchar](20),
	@iOK [int] = 0
AS
/*microcat
작성일 : 2008년 1월 31일
사용 : 최초 계정 생성후 게임에 진입한 유저인지 아닌지 리턴
*/
set nocount on;

if not exists
    (
        select *
            from dbo.Users with (nolock)
                where Login = @strLogin_
                and IPAddress <> N'0.0.0.0'
    )
    begin    select @iOK = -1    end

select @iOK


