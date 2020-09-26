CREATE PROCEDURE [dbo].[NMW_net_marble_goodsinfolist_select]
	@strGoodsName_ [nvarchar](50) = N'',
	@iCharType_ [smallint] = -1,
	@bGoodsClass_ [bit] = 0,
	@bAttack_ [bit] = 0,
	@bDefence_ [bit] = 0,
	@bHPInc_ [bit] = 0,
	@bPrice_ [bit] = 0,
	@bDESC_ [bit] = 0
AS
set nocount on

declare
    @strGoodsName nvarchar(50)
select
    @StrGoodsName = substring(@strGoodsName_, 1, 49) + N'%'

declare
    @str nvarchar(4000)
select
    @str = N'
    select
        GoodsID
    ,   GoodsName
    ,   Description
    ,   IsLimited
    ,   Duration
    ,   CharType
    ,   Kind
    ,   ItemType
    ,   ShowWeb
    ,   HotCount
    ,   Price
    ,   IsNewGoods
    ,   Cash
    ,   GoodsClass
    ,   Img
    ,   RegDate
    ,   IsSubSetGoods
    ,   SetGoodsNum
    ,   Recom
    ,   Category
    ,   Attack
    ,   Defence
    ,   HPInc
    ,   LastingPrice
    from dbo.NMWNetMarbleGoodsInfoList
    with (nolock)
    where
        CharType in ( @1, @2 )'
    + case when @strGoodsName_ <> N'' then N'
    and GoodsName like @3' else N'' end
    + N'
    order by
        RegDate desc'
    + case
        when @bGoodsClass_ = 1 then N'
    ,   GoodsClass' + case when @bDESC_ = 1 then N' DESC' else N' ASC' end
        when @bAttack_ = 1 then N'
    ,   Attack' + case when @bDESC_ = 1 then N' DESC' else N' ASC' end
        when @bDefence_ = 1 then N'
    ,   Defence' + case when @bDESC_ = 1 then N' DESC' else N' ASC' end
        when @bHPInc_ = 1 then N'
    ,   HPInc' + case when @bDESC_ = 1 then N' DESC' else N' ASC' end
        when @bPrice_ = 1 then N'
    ,   Price' + case when @bDESC_ = 1 then N' DESC' else N' ASC' end
        else N''
        end
    + N'
    option (maxdop 1)'

--print @str
--/*
exec dbo.sp_executesql
    @str
,   N'@1 int, @2 int, @3 nvarchar(50)'
,   -1
,   @iCharType_
,   @strGoodsName
--*/


