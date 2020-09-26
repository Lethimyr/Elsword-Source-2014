<%@ Page Language="C#" AutoEventWireup="true" CodeBehind="Login.aspx.cs" Inherits="TransDBManagerForWeb.Login" %>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<html xmlns="http://www.w3.org/1999/xhtml" >
<head runat="server">
    <title>Login</title>
</head>
<body>
    <form id="form1" runat="server">
    <div>
        <asp:TextBox ID="TextBox1" runat="server" Style="z-index: 100; left: 528px; position: absolute;
            top: 264px" Width="180px"></asp:TextBox>
        <asp:Label ID="Label1" runat="server" Style="z-index: 101; left: 488px; position: absolute;
            top: 264px" Text="ID : "></asp:Label>
        <asp:TextBox ID="TextBox2" runat="server" Style="z-index: 102; left: 528px; position: absolute;
            top: 304px" Width="180px"></asp:TextBox>
        <asp:Label ID="Label2" runat="server" Style="z-index: 103;  left: 468px; position: absolute;
            top: 308px" Text="PWD : "></asp:Label>
        <asp:Button ID="Button1" runat="server" OnClick="Button1_Click" Style="z-index: 104;
            left: 668px; position: absolute; top: 348px" Text="Login" />
        <asp:Label ID="Label3" runat="server" ForeColor="Red" Style="z-index: 106; left: 468px;
            position: absolute; top: 396px" Width="316px"></asp:Label>
    
    </div>
    </form>
</body>
</html>
