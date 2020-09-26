using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace ContentSelector
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();

            try
            {
                ScriptTreeView.Nodes.Clear();
                ScriptTreeManager.ReadWorldInfo();
                ScriptTreeManager.AddtoTreeView(ScriptTreeView.Nodes);
                ScriptTreeView.ExpandAll();
            }
            catch(Exception e)
            {
                MessageBox.Show("Default Path Open Failed : " + e.Message);
            }
            
        }

        private void CheckAllParentNodes(TreeNode treeNode, bool nodeChecked)
        {
            treeNode.Checked = nodeChecked;
            ((TreeData)treeNode.Tag).m_bOn = nodeChecked;
            
            if (treeNode.Parent != null)
                CheckAllParentNodes(treeNode.Parent, nodeChecked);
        }

        private void CheckAllChildNodes(TreeNode treeNode, bool nodeChecked)
        {
            foreach (TreeNode node in treeNode.Nodes)
            {
                node.Checked = nodeChecked;
                ((TreeData)node.Tag).m_bOn = nodeChecked;
                if (node.Nodes.Count > 0)
                {
                    // If the current node has child nodes, call the CheckAllChildsNodes method recursively.
                    this.CheckAllChildNodes(node, nodeChecked);
                }
            }
        }




        private void treeView1_AfterCheck(object sender, TreeViewEventArgs e)
        {
            // The code only executes if the user caused the checked state to change.
            if(e.Action != TreeViewAction.Unknown)
            {
                ((TreeData)e.Node.Tag).m_bOn = e.Node.Checked;
                if(e.Node.Checked && e.Node.Parent != null )
                {
                     this.CheckAllParentNodes(e.Node.Parent, true);
                }
                else if(!e.Node.Checked && e.Node.Nodes.Count > 0)
                {
                    this.CheckAllChildNodes(e.Node, false);
                }
            }
        }
        

        private void button1_Click(object sender, EventArgs e)
        {
            FolderBrowserDialog FBDialog = new FolderBrowserDialog();
            FBDialog.ShowNewFolderButton = false;
            FBDialog.Description = "Select svn resources folder..";
            FBDialog.ShowDialog();

            if( true == ScriptTreeManager.SetResourceDir(FBDialog.SelectedPath) )
            {
                ScriptTreeView.Nodes.Clear();
                ScriptTreeManager.ReadWorldInfo();
                ScriptTreeManager.AddtoTreeView(ScriptTreeView.Nodes);
                ScriptTreeView.ExpandAll();
            }
        }

        private void aboutToolStripMenuItem_Click(object sender, EventArgs e)
        {
            MessageBox.Show("Elsword ScriptTool ver 0.01");
        }

        private void menuStrip1_ItemClicked(object sender, ToolStripItemClickedEventArgs e)
        {

        }

        private void openScriptToolStripMenuItem_Click(object sender, EventArgs e)
        {
            
        }

        private void SaveMetaDataToolStripMenuItem_Click(object sender, EventArgs e)
        {
            SaveFileDialog SFDialog = new SaveFileDialog();
            SFDialog.DefaultExt = "met";
            SFDialog.AddExtension = true;
            SFDialog.OverwritePrompt = true;
            SFDialog.ShowDialog();

            string filename = SFDialog.FileName;
            string code = textBoxCode.Text;

            ScriptTreeManager.SaveMetadata(filename, code);
        }

        private void LoadMetaDataToolStripMenuItem_Click(object sender, EventArgs e)
        {
            OpenFileDialog OFDialog = new OpenFileDialog();
            OFDialog.Multiselect = false;
            OFDialog.ShowDialog();

            string filename = OFDialog.FileName;

            
            ScriptTreeManager.LoadMetadata(filename);

            ScriptTreeView.Nodes.Clear();
            ScriptTreeManager.AddtoTreeView(ScriptTreeView.Nodes);
            ScriptTreeView.ExpandAll();
            
        }

        private void ButtonExport_Click(object sender, EventArgs e)
        {
            FolderBrowserDialog FBDialog = new FolderBrowserDialog();
            FBDialog.ShowNewFolderButton = true;
            FBDialog.Description = "Export files to...";
            FBDialog.ShowDialog();
            Encoding encoding;
            switch (comboBoxEncoding.SelectedIndex)
            {
                case 0:
                    {
                        encoding = Encoding.Default;
                    } break;
                case 1:
                    {
                        encoding = Encoding.UTF8;                        
                    } break;
                default:
                    {
                        MessageBox.Show("Choose Encoding Method!");
                        return;
                    }
            }
            ScriptTreeManager.ExportScript(FBDialog.SelectedPath, CheckBoxAddCode.Checked, textBoxCode.Text, encoding);

            MessageBox.Show("Export Complete");
        }

        private void button1_Click_1(object sender, EventArgs e)
        {
            ScriptTreeView.Nodes.Clear();
            ScriptTreeManager.ReadWorldInfo();
            ScriptTreeManager.AddtoTreeView(ScriptTreeView.Nodes);
            ScriptTreeView.ExpandAll();
        }

        private void ScriptTreeView_AfterSelect(object sender, TreeViewEventArgs e)
        {

        }

        private void checkedListBox2_SelectedIndexChanged(object sender, EventArgs e)
        {

        }

        private void ButtonCharacterExport_Click(object sender, EventArgs e)
        {
            FolderBrowserDialog FBDialog = new FolderBrowserDialog();
            FBDialog.ShowNewFolderButton = true;
            FBDialog.Description = "Export files to...";
            FBDialog.ShowDialog();
            Encoding encoding;
            switch (comboBoxEncoding.SelectedIndex)
            {
                case 0:
                    {
                        encoding = Encoding.Default;                        
                    } break;
                case 1:
                    {
                        encoding = Encoding.UTF8;                        
                    } break;
                default:
                    {
                        MessageBox.Show("Choose Encoding Method!");
                        return;
                    }
            }
            CharacterScriptCreator.CreateScript(ref checkedListBoxCharacter, ref checkedListBoxNewMark, FBDialog.SelectedPath, CheckBoxAddCode.Checked, textBoxCode.Text, encoding);

            MessageBox.Show("Export Complete");

        }
                        
    }
}