CREATE PROCEDURE [dbo].[PIGA_pet_info_update_fulltime_old]
	@iLoginUID_ [int],
	@strData_ [nvarchar](4000),
	@iOK [int] = 0
AS
set nocount on
set transaction isolation level read uncommitted

/*
20081024. microcat.

{ call dbo.PIGA_pet_info_update_fulltime ( %d, N'%s' ) }

{ call dbo.PIGA_pet_info_update_fulltime ( @1, @2 ) }
@1 ; LoginUID int
@2 ; Data nvarchar(4000) ; [@21,@22,@23,]n
    @21 ; PetUID int
    @22 ; PetID int
    @23 ; FullTime int

1 return ( @1 )
@1 ; OK int
*/

execute dbo.AAA_login_check
    @iLoginUID_
,   @iOK output

if @iOK <> 0
begin    select @iOK = -1    goto end_proc    end

declare
    @strToken nvarchar(1)
,   @iSplit tinyint
select
    @strToken = N','
,   @iSplit = 3

declare
    @Data table
(
    No int not null identity (0, 1)
,   Data nvarchar(11) not null
)
insert into @Data
(
    Data
)
select
    substring
    (
        @strData_
    ,   min(No)
    ,   charindex
        (
            @strToken
        ,   @StrData_ + @strToken
        ,   min(No)
        )
        - min(No)
    )
from dbo.TDNDataNo
where
    No <= len(@StrData_)
group by
    charindex
    (
        @strToken
    ,   @StrData_ + @strToken
    ,   No
    )
order by
    charindex
    (
        @strToken
    ,   @StrData_ + @strToken
    ,   No
    )
option (maxdop 1)

if @@error <> 0
begin    select @iOK = -2    goto end_proc    end

declare
    @Result table
(
    PetUID int not null
,   PetID int not null
,   FullTime int not null
)
insert into @Result
(
    PetUID
,   PetID
,   FullTime
)
select
    max(case when No % @iSplit = 0 then Data end)
,   max(case when No % @iSplit = 1 then Data end)
,   max(case when No % @iSplit = 2 then Data end)
from @Data
group by
    No / @iSplit
option (maxdop 1)

if @@error <> 0
begin    select @iOK = -3    goto end_proc    end

if exists
(
    select *
    from @Result
    as a
    left outer join
    (
        select
            PetUID
        ,   PetID
        from dbo.GPet
        where
            LoginUID = @iLoginUID_
    )
    as b
    on
        a.PetUID = b.PetUID
    and a.PetID = b.PetID
    where
        b.PetUID is null
)
begin    select @iOK = -4    goto end_proc    end

begin transaction

update a
with (updlock)
set
    a.FullTime =
    case
        when a.FullTime + b.FullTime > 6000 then 6000
        when a.FullTime + b.FullTime < 0    then 0
        else a.FullTime + b.FullTime
    end
from dbo.GPet
as a
with (updlock)
join @Result
as b
on
    a.PetUID = b.PetUID
and a.PetID = b.PetID
where
    a.LoginUID = @iLoginUID_
option (maxdop 1)

if @@error <> 0
begin    select @iOK = -101    goto fail_tran    end

commit transaction

goto end_proc

fail_tran:
rollback transaction

end_proc:
select
    @iOK


