<%@ Page Language="C#" AutoEventWireup="true" EnableEventValidation="false" CodeBehind="Trans.aspx.cs" Inherits="TransDBManagerForWeb._Default" %>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<html xmlns="http://www.w3.org/1999/xhtml" >
<head runat="server">
    <title>Trans</title>
</head>
<body>
    <form id="form1" runat="server">
    <div>
        <asp:GridView ID="GridView1" runat="server" DataKeyNames="Index" AutoGenerateColumns="False" Height="356px"
            Style="z-index: 100; left: 76px; position: absolute; top: 140px" Width="1280px" CellPadding="4" ForeColor="#333333" AllowPaging="True" OnPageIndexChanging="GridView1_PageIndexChanging" PageSize="50" OnRowUpdating="GridView1_RowUpdating" BorderColor="Black" BorderStyle="Double" OnRowEditing="GridView1_RowEditing" OnRowCommand="GridView1_RowCommand" EnableTheming="True" OnRowUpdated="GridView1_RowUpdated"  >
            <FooterStyle BackColor="#990000" Font-Bold="True" ForeColor="White" />
            <RowStyle BackColor="#FFFBD6" ForeColor="#333333" />
            <SelectedRowStyle BackColor="#FFCC66" Font-Bold="True" ForeColor="Navy" />
            <PagerStyle BackColor="#FFCC66" ForeColor="#333333" HorizontalAlign="Center" />
            <HeaderStyle BackColor="#990000" HorizontalAlign="Center" Font-Bold="True" ForeColor="White" />
            <AlternatingRowStyle BackColor="White" />            
            <Columns>
                    <asp:BoundField DataField="Index" HeaderText="Index"
                        ReadOnly="True">
                        <ItemStyle BorderColor="Gray" HorizontalAlign="Center" Width="50px" />
                    </asp:BoundField>
                    <asp:BoundField DataField="TableName" HeaderText="TableName"
                        ReadOnly="True">
                        <ItemStyle BorderColor="Gray" HorizontalAlign="Center" Width="50px" />
                    </asp:BoundField>
                    <asp:BoundField DataField="String" HeaderText="String" ReadOnly="True" >
                        <ItemStyle BorderColor="Gray"  Width="300px" />
                    </asp:BoundField>
                    <asp:TemplateField>
                        <HeaderTemplate>String_Foreign</HeaderTemplate>                        
                        <ItemTemplate>                            
                            <asp:TextBox runat="server" ID="String_Foreign"  Font-Size="Medium" Width = "300px" Height="99%" BorderStyle="Groove" TextMode="MultiLine" Text='<%# DataBinder.Eval(Container.DataItem, "String_Foreign") %>' Rows="4" ></asp:TextBox>
                        </ItemTemplate>
                        <ItemStyle BorderColor="Gray" />
                    </asp:TemplateField>
                    <asp:TemplateField>
                        <HeaderTemplate>Memo</HeaderTemplate>                        
                        <ItemTemplate>                            
                            <asp:TextBox runat="server" ID="Memo"  Font-Size="Medium" Width="150" Height="99%" BorderStyle="Groove" TextMode="MultiLine" Text='<%# DataBinder.Eval(Container.DataItem, "Memo") %>' Rows="4" ></asp:TextBox>
                        </ItemTemplate>
                        <ItemStyle BorderColor="Gray"/>
                    </asp:TemplateField>
                    <asp:BoundField DataField="Last Update Date" HeaderText="Last Update Date" ReadOnly="True" >
                        <ItemStyle BorderColor="Gray"  Width="200px" />
                    </asp:BoundField>                
                    <asp:ButtonField ButtonType="Button" CommandName="Update" Text="Update">
                        <ItemStyle BorderColor="Gray" Width="60px" />
                    </asp:ButtonField>
                
                </Columns>
            
        </asp:GridView>
    &nbsp;&nbsp;
        <asp:Button ID="btnAllUpdate" runat="server" Height="40px" OnClick="btnAllUpdate_Click"
            Style="z-index: 101; left: 912px; position: absolute; top: 16px" Text="All Update"
            Width="88px" TabIndex="50" EnableViewState="False" Visible="False" />
        &nbsp;&nbsp;
        <asp:RadioButton ID="RadioButton1" runat="server" Checked="True" GroupName="TableName"
            Style="z-index: 102; left: 76px;
            position: absolute; top: 16px" Text="All" />
        <asp:RadioButton ID="RadioButton2" runat="server" GroupName="TableName"
            Style="z-index: 103; left: 204px; position: absolute; top: 16px" Text="Item" />
        <asp:RadioButton ID="RadioButton3" runat="server" GroupName="TableName"
            Style="z-index: 104; left: 300px; position: absolute; top: 16px" Text="SetItem" />
        <asp:RadioButton ID="RadioButton4" runat="server" GroupName="TableName"
            Style="z-index: 105; left: 408px; position: absolute; top: 16px" Text="QuestTable" />
        <asp:RadioButton ID="RadioButton5" runat="server" GroupName="TableName"
            Style="z-index: 106; left: 76px; position: absolute; top: 40px" Text="SubQuestTable" />
        <asp:RadioButton ID="RadioButton6" runat="server" GroupName="TableName"
            Style="z-index: 107; left: 204px; position: absolute; top: 40px" Text="TitleTable" />
        <asp:RadioButton ID="RadioButton7" runat="server" GroupName="TableName"
            Style="z-index: 108; left: 300px; position: absolute; top: 40px" Text="TitleMission" />
        <asp:RadioButton ID="RadioButton8" runat="server" GroupName="TableName"
            Style="z-index: 109; left: 408px; position: absolute; top: 44px" Text="SubTitleMission" />
        <asp:Button ID="Button1" runat="server" Height="52px" OnClick="Button1_Click" Style="z-index: 110;
            left: 832px; position: absolute; top: 16px" Text="OK" Width="76px" TabIndex="1" />
        <asp:RadioButton ID="RadioButton9" runat="server" Style="z-index: 111; left: 540px;
            position: absolute; top: 16px" Text="StringTable" GroupName="TableName" />
        <asp:RadioButton ID="RadioButton10" runat="server" Style="z-index: 112; left: 540px;
            position: absolute; top: 40px" Text="NetErrorTable" GroupName="TableName" />
        <asp:RadioButton ID="RadioButton11" runat="server" GroupName="TableName" Style="z-index: 113;
            left: 672px; position: absolute; top: 16px" Text="TextureStringManager" />
        <asp:TextBox ID="TextPage" runat="server"  Style="z-index: 114; left: 972px; position: absolute;
            top: 112px" Width="32px"></asp:TextBox>
        <asp:Button ID="btnMove" runat="server" OnClick="btnMove_Click" Style="z-index: 115;
            left: 1056px; position: absolute; top: 112px" Text="Move" />
        <asp:RadioButton ID="RadioButton12" runat="server" GroupName="TableName" Style="z-index: 116;
            left: 672px; position: absolute; top: 44px" Text="Not Trans" />
        <asp:Label ID="Label1" runat="server" Height="16px" Style="z-index: 117; left: 1016px;
            position: absolute; top: 112px" Text="Label"></asp:Label>
        <asp:CheckBox ID="CheckBox1" runat="server" Style="z-index: 118; left: 76px; position: absolute;
            top: 112px" Text="Memo Edit" />
        <asp:RadioButton ID="RadioButton13" runat="server" GroupName="TableName" Style="z-index: 119;
            left: 76px; position: absolute; top: 68px" Text="SkillEnum" />
        <asp:RadioButton ID="RadioButton14" runat="server" GroupName="TableName" Style="z-index: 120;
            left: 204px; position: absolute; top: 68px" Text="SkillTable" />
        <asp:TextBox ID="TextSearch" runat="server" Style="z-index: 121; left: 708px; position: absolute;
            top: 108px"></asp:TextBox>
        &nbsp;
        <asp:Button ID="btnSearch" runat="server" OnClick="btnSearch_Click" Style="z-index: 122;
            left: 868px; position: absolute; top: 108px" Text="Search" TabIndex="2" />
        <asp:RadioButton ID="RadioButton15" runat="server" Checked="True" GroupName="Search"
            Style="z-index: 123; left: 584px; position: absolute; top: 108px" Text="Kr" />
        <asp:RadioButton ID="RadioButton16" runat="server" GroupName="Search" Style="z-index: 125;
            left: 636px; position: absolute; top: 108px" Text="Foreign" />
    </div>
    </form>
</body>
</html>
