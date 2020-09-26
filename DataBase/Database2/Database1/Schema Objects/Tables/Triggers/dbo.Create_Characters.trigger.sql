CREATE TRIGGER [dbo].[Create_Characters] ON [dbo].[users] FOR INSERT
AS
insert into dbo.Characters
with (updlock)
(
    Login
,   CharType
)
select
    a.Login
,   b.CharType
from Inserted
as a
with (updlock)
cross join
(
    select
        0 as CharType
    union all
    select
        1
    union all
    select
        2
)
as b


