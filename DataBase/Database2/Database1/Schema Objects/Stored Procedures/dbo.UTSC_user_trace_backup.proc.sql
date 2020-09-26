CREATE PROCEDURE [dbo].[UTSC_user_trace_backup]
	@bOnlyCreateView_ [int] = 0
,   @iOK [int] = 0
AS
set nocount on
set transaction isolation level read uncommitted

/*
20081229. microcat.
*/

if @bOnlyCreateView_ = 1
begin    goto end_proc    end

declare
    @strBackupTable1 nvarchar(4000)
,   @strBackupTable2 nvarchar(4000)
select
    @strBackupTable1
    =   N'SUTUserTrace_'
    +   convert(nvarchar(6), dateadd(mm, -1, current_timestamp), 112)
,   @strBackupTable2
    =   N'SUTUserTraceCharacter_'
    +   convert(nvarchar(6), dateadd(mm, -1, current_timestamp), 112)

if exists
(
    select *
    from dbo.sysobjects
    where
        Name = @strBackupTable1
)
begin    goto end_proc    end

if not exists
(
    select *
    from dbo.sysobjects
    where
        Name = N'SUTUserTrace_New'
)
begin
    execute dbo.sp_executesql
N'
create table dbo.SUTUserTrace_New
(
    LoginUID int not null
,   UID int not null identity(1, 1)
,   StartDate smalldatetime not null
,   EndDate smalldatetime not null
,   Grade tinyint not null
,   PlayTime int not null
,   PCBang bit not null
,   GuildServer bit not null
);
create clustered index IXC_SUTUserTrace_StartDate_EndDate
on dbo.SUTUserTrace_New
(
    StartDate
,   EndDate
)
with fillfactor = 80;
create unique nonclustered index IXU_SUTUserTrace_LoginUID_UID
on dbo.SUTUserTrace_New
(
    LoginUID
,   UID
)
with fillfactor = 80;
'
end

if not exists
(
    select *
    from dbo.sysobjects
    where
        Name = N'SUTUserTraceCharacter_New'
)
begin
    execute dbo.sp_executesql
N'
create table dbo.SUTUserTraceCharacter_New
(
    LoginUID int not null
,   UID int not null
,   CharType tinyint not null
,   [EXP] bigint not null
,   Level int not null
,   PlayCount int not null
,   Turtle bit not null
);
create unique clustered index IXUC_SUTUserTraceCharacter_LoginUID_UID_CharType
on dbo.SUTUserTraceCharacter_New
(
    LoginUID
,   UID
,   CharType
)
with fillfactor = 80;
'
end

execute dbo.sp_rename
    N'SUTUserTrace'
,   @strBackupTable1

execute dbo.sp_rename
    N'SUTUserTrace_New'
,   N'SUTUserTrace'

execute dbo.sp_rename
    N'SUTUserTraceCharacter'
,   @strBackupTable2

execute dbo.sp_rename
    N'SUTUserTraceCharacter_New'
,   N'SUTUserTraceCharacter'

end_proc:
declare
    @Data1 table
(
    No int not null identity(1, 1)
,   Data nvarchar(4000) not null
)
insert into @Data1
(
    Data
)
select
N'select
    LoginUID
,   UID
,   '
+   case
        when substring(name, 14, 6) = N'' then N'1'
        else substring(name, 14, 6)
    end
+   ' as UID2
,   StartDate
,   EndDate
,   Grade
,   PlayTime
,   PCBang
,   GuildServer
from dbo.'
+   name
+   N'
with (nolock)'
from dbo.sysobjects
where
    name like N'SUTUserTrace%'
and name not like N'SUTUserTraceCharacter%'

declare
    @strResult1 nvarchar(4000)
select
    @strResult1 = N''
select
    @strResult1
=   @strResult1
+   case No
        when 1 then N'ALTER VIEW [dbo].[vSUTUserTrace] AS
'
        else N'
union all
'
    end
+   Data
from @Data1

execute dbo.sp_executesql
    @strResult1

declare
    @Data2 table
(
    No int not null identity(1, 1)
,   Data nvarchar(4000) not null
)
insert into @Data2
(
    Data
)
select
N'select
    LoginUID
,   UID
,   '
+   case
        when substring(name, 23, 6) = N'' then N'1'
        else substring(name, 23, 6)
    end
+   ' as UID2
,   CharType
,   [EXP]
,   Level
,   PlayCount
,   Turtle
from dbo.'
+   name
+   N'
with (nolock)'
from dbo.sysobjects
where
    name like N'SUTUserTraceCharacter%'

declare
    @strResult2 nvarchar(4000)
select
    @strResult2 = N''
select
    @strResult2
=   @strResult2
+   case No
        when 1 then N'ALTER VIEW [dbo].[vSUTUserTraceCharacter] AS
'
        else N'
union all
'
    end
+   Data
from @Data2

execute dbo.sp_executesql
    @strResult2


