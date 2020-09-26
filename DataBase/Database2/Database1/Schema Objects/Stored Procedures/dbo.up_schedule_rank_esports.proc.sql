CREATE PROCEDURE [dbo].[up_schedule_rank_esports]
AS
SET NOCOUNT ON;

declare @original table
(
    grade int not null,
    login nvarchar(20) not null,
    nick nvarchar(24) not null,
    rankingpoint int not null,
    win int not null,
    loss int not null,
    winrate real not null
)

insert into @original(grade, login, nick, rankingpoint, win, loss, winrate)
    select a.grade, a.login, c.nick, b.rankingpoint, b.win, b.loss, b.winrate
            from dbo.users as a(nolock)
                join dbo.esports_playerlist as b(nolock) on a.login = b.login
                join dbo.nicknames as c(nolock) on b.login = c.login
                    order by a.grade asc, b.rankingpoint desc, b.win desc, b.loss asc, b.winrate desc, b.regdate desc

declare @output0 table(grade int not null,    rank int not null identity(1, 1) primary key,    login nvarchar(20) not null,    nick nvarchar(24) not null,    rankingpoint int not null,    win int not null,    loss int not null,    winrate real not null)
declare @output1 table(grade int not null,    rank int not null identity(1, 1) primary key,    login nvarchar(20) not null,    nick nvarchar(24) not null,    rankingpoint int not null,    win int not null,    loss int not null,    winrate real not null)
declare @output2 table(grade int not null,    rank int not null identity(1, 1) primary key,    login nvarchar(20) not null,    nick nvarchar(24) not null,    rankingpoint int not null,    win int not null,    loss int not null,    winrate real not null)
declare @output3 table(grade int not null,    rank int not null identity(1, 1) primary key,    login nvarchar(20) not null,    nick nvarchar(24) not null,    rankingpoint int not null,    win int not null,    loss int not null,    winrate real not null)
declare @output4 table(grade int not null,    rank int not null identity(1, 1) primary key,    login nvarchar(20) not null,    nick nvarchar(24) not null,    rankingpoint int not null,    win int not null,    loss int not null,    winrate real not null)
declare @output5 table(grade int not null,    rank int not null identity(1, 1) primary key,    login nvarchar(20) not null,    nick nvarchar(24) not null,    rankingpoint int not null,    win int not null,    loss int not null,    winrate real not null)
declare @output6 table(grade int not null,    rank int not null identity(1, 1) primary key,    login nvarchar(20) not null,    nick nvarchar(24) not null,    rankingpoint int not null,    win int not null,    loss int not null,    winrate real not null)
declare @output7 table(grade int not null,    rank int not null identity(1, 1) primary key,    login nvarchar(20) not null,    nick nvarchar(24) not null,    rankingpoint int not null,    win int not null,    loss int not null,    winrate real not null)
declare @output8 table(grade int not null,    rank int not null identity(1, 1) primary key,    login nvarchar(20) not null,    nick nvarchar(24) not null,    rankingpoint int not null,    win int not null,    loss int not null,    winrate real not null)
declare @output9 table(grade int not null,    rank int not null identity(1, 1) primary key,    login nvarchar(20) not null,    nick nvarchar(24) not null,    rankingpoint int not null,    win int not null,    loss int not null,    winrate real not null)
declare @output10 table(grade int not null,    rank int not null identity(1, 1) primary key,    login nvarchar(20) not null,    nick nvarchar(24) not null,    rankingpoint int not null,    win int not null,    loss int not null,    winrate real not null)
declare @output11 table(grade int not null,    rank int not null identity(1, 1) primary key,    login nvarchar(20) not null,    nick nvarchar(24) not null,    rankingpoint int not null,    win int not null,    loss int not null,    winrate real not null)

insert into @output0 select * from @original where grade = 0
insert into @output1 select * from @original where grade = 1
insert into @output2 select * from @original where grade = 2
insert into @output3 select * from @original where grade = 3
insert into @output4 select * from @original where grade = 4
insert into @output5 select * from @original where grade = 5
insert into @output6 select * from @original where grade = 6
insert into @output7 select * from @original where grade = 7
insert into @output8 select * from @original where grade = 8
insert into @output9 select * from @original where grade = 9
insert into @output10 select * from @original where grade = 10
insert into @output11 select * from @original where grade = 11

insert into dbo.log_rank_esports select * from dbo.rank_esports(nolock)
truncate table dbo.rank_esports
insert into dbo.rank_esports select * from @output0
insert into dbo.rank_esports select * from @output1
insert into dbo.rank_esports select * from @output2
insert into dbo.rank_esports select * from @output3
insert into dbo.rank_esports select * from @output4
insert into dbo.rank_esports select * from @output5
insert into dbo.rank_esports select * from @output6
insert into dbo.rank_esports select * from @output7
insert into dbo.rank_esports select * from @output8
insert into dbo.rank_esports select * from @output9
insert into dbo.rank_esports select * from @output10
insert into dbo.rank_esports select * from @output11


