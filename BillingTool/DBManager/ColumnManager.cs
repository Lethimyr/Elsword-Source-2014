using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.IO;

namespace DBManager
{
    public partial class ColumnManager : Form
    {
        DataGridView dgvTemp;
        int m_nSelectTab;

        public ColumnManager()
        {
            InitializeComponent();
        }

        public void SetDataGridView(DataGridView dataGridView)
        {
            dgvTemp = dataGridView;
        }

        public void SetCurrentTab(int nSelectTab)
        {
            m_nSelectTab = nSelectTab;
        }

        void ColumnLoad_BillingTable()
        {
            switch (m_nSelectTab)
            {
                case 0:
                    {
                        checkedListBox1.Items.Add("IndexKey");
                        checkedListBox1.Items.Add("ID");
                        checkedListBox1.Items.Add("Name");
                        checkedListBox1.Items.Add("Description");
                        checkedListBox1.Items.Add("Description In Shop");
                        checkedListBox1.Items.Add("Model Name");
                        checkedListBox1.Items.Add("Texture Change XET Name");
                        checkedListBox1.Items.Add("ELSWORD_MODEL_NAME");
                        checkedListBox1.Items.Add("ELSWORD_XET_NAME");
                        checkedListBox1.Items.Add("AISHA_MODEL_NAME");
                        checkedListBox1.Items.Add("AISHA_XET_NAME");
                        checkedListBox1.Items.Add("RENA_MODEL_NAME");
                        checkedListBox1.Items.Add("RENA_XET_NAME");
                        checkedListBox1.Items.Add("RAVEN_MODEL_NAME");
                        checkedListBox1.Items.Add("RAVEN_XET_NAME");
                        checkedListBox1.Items.Add("EVE_MODEL_NAME");
                        checkedListBox1.Items.Add("EVE_XET_NAME");
                        checkedListBox1.Items.Add("Anim XET Name");
                        checkedListBox1.Items.Add("Anim Name");
                        checkedListBox1.Items.Add("Shop Image");
                        checkedListBox1.Items.Add("Drop Viewer");
                        checkedListBox1.Items.Add("Item Type");
                        checkedListBox1.Items.Add("Item Type_");
                        checkedListBox1.Items.Add("ITEMTYPE_C");
                        checkedListBox1.Items.Add("Fashion");
                        checkedListBox1.Items.Add("Vested");
                        checkedListBox1.Items.Add("Can Enchant");
                        checkedListBox1.Items.Add("Use Type");
                        checkedListBox1.Items.Add("Use Type_");
                        checkedListBox1.Items.Add("USETYPE_C");
                        checkedListBox1.Items.Add("Attach FrameName");
                        checkedListBox1.Items.Add("FO_Elsword_X");
                        checkedListBox1.Items.Add("FO_Elsword_Y");
                        checkedListBox1.Items.Add("FO_Elsword_Z");
                        checkedListBox1.Items.Add("FO_Lena_X");
                        checkedListBox1.Items.Add("FO_Lena_Y");
                        checkedListBox1.Items.Add("FO_Lena_Z");
                        checkedListBox1.Items.Add("FO_Aisha_X");
                        checkedListBox1.Items.Add("FO_Aisha_Y");
                        checkedListBox1.Items.Add("FO_Aisha_Z");
                        checkedListBox1.Items.Add("FO_Raven_X");
                        checkedListBox1.Items.Add("FO_Raven_Y");
                        checkedListBox1.Items.Add("FO_Raven_Z");
                        checkedListBox1.Items.Add("FO_Eve_X");
                        checkedListBox1.Items.Add("FO_Eve_Y");
                        checkedListBox1.Items.Add("FO_Eve_Z");
                        checkedListBox1.Items.Add("Can HyperMode");
                        checkedListBox1.Items.Add("Period Type");
                        checkedListBox1.Items.Add("Period Type_");
                        checkedListBox1.Items.Add("PERIODTYPE_C");
                        checkedListBox1.Items.Add("Endurance");
                        checkedListBox1.Items.Add("EnduranceDamageMin");
                        checkedListBox1.Items.Add("EnduranceDamageMax");
                        checkedListBox1.Items.Add("Repair ED");
                        checkedListBox1.Items.Add("Repair VP");
                        checkedListBox1.Items.Add("Quantity");
                        checkedListBox1.Items.Add("Price Type");
                        checkedListBox1.Items.Add("Price Type_");
                        checkedListBox1.Items.Add("PRICETYPE_C");
                        checkedListBox1.Items.Add("Price");
                        checkedListBox1.Items.Add("Price PvP Point");
                        checkedListBox1.Items.Add("Use Condition");
                        checkedListBox1.Items.Add("Use Condition_");
                        checkedListBox1.Items.Add("USECONDITION_C");
                        checkedListBox1.Items.Add("Unit Type");
                        checkedListBox1.Items.Add("Unit Type_");
                        checkedListBox1.Items.Add("UNITTYPE_C");
                        checkedListBox1.Items.Add("Unit Class");
                        checkedListBox1.Items.Add("Unit Class_");
                        checkedListBox1.Items.Add("UNITCLASS_C");
                        checkedListBox1.Items.Add("Use Level");
                        checkedListBox1.Items.Add("Eqip Position");
                        checkedListBox1.Items.Add("Eqip Position_");
                        checkedListBox1.Items.Add("EQIPPOSITION_C");
                        checkedListBox1.Items.Add("Item Grade");
                        checkedListBox1.Items.Add("Item Grade_");
                        checkedListBox1.Items.Add("ITEMGRADE_C");
                        checkedListBox1.Items.Add("BASE_HP");
                        checkedListBox1.Items.Add("ATK_PHYSIC");
                        checkedListBox1.Items.Add("ATK_MAGIC");
                        checkedListBox1.Items.Add("DEF_PHYSIC");
                        checkedListBox1.Items.Add("DEF_MAGIC");
                        checkedListBox1.Items.Add("SA_NUM");
                        checkedListBox1.Items.Add("SA_01");
                        checkedListBox1.Items.Add("SA_02");
                        checkedListBox1.Items.Add("SA_03");
                        checkedListBox1.Items.Add("SA_04");
                        checkedListBox1.Items.Add("SA_05");
                        checkedListBox1.Items.Add("SA_06");
                        checkedListBox1.Items.Add("SA_07");
                        checkedListBox1.Items.Add("SA_08");
                        checkedListBox1.Items.Add("SA_09");
                        checkedListBox1.Items.Add("SA_10");
                        checkedListBox1.Items.Add("SO_01");
                        checkedListBox1.Items.Add("SO_02");
                        checkedListBox1.Items.Add("SO_03");
                        checkedListBox1.Items.Add("SO_04");
                        checkedListBox1.Items.Add("SO_05");
                        checkedListBox1.Items.Add("SO_06");
                        checkedListBox1.Items.Add("SO_07");
                        checkedListBox1.Items.Add("SO_08");
                        checkedListBox1.Items.Add("SO_09");
                        checkedListBox1.Items.Add("COOL_TIME");
                        checkedListBox1.Items.Add("SET_ID");
                        checkedListBox1.Items.Add("Can Use Inventory");
                        checkedListBox1.Items.Add("NoEquip");
                        checkedListBox1.Items.Add("SLASH_TRACE_COLOR");
                        checkedListBox1.Items.Add("SLASH_TRACE_TIP_COLOR");
                        checkedListBox1.Items.Add("HYPER_SLASH_TRACE_COLOR");
                        checkedListBox1.Items.Add("HYPER_SLASH_TRACE_TIP_COLOR");
                        checkedListBox1.Items.Add("ELSWORD_ITEM_SCALE_ROTATE");
                        checkedListBox1.Items.Add("AISHA_ITEM_SCALE_ROTATE");
                        checkedListBox1.Items.Add("RENA_ITEM_SCALE_ROTATE");
                        checkedListBox1.Items.Add("RAVEN_ITEM_SCALE_ROTATE");
                        checkedListBox1.Items.Add("EVE_ITEM_SCALE_ROTATE");
                        checkedListBox1.Items.Add("ATTACHED_MESH_NAME1");
                        checkedListBox1.Items.Add("ATTACHED_MESH_BONE_NAME1");
                        checkedListBox1.Items.Add("ATTACHED_MESH_NAME2");
                        checkedListBox1.Items.Add("ATTACHED_MESH_BONE_NAME2");
                        checkedListBox1.Items.Add("ATTACHED_MESH_NAME3");
                        checkedListBox1.Items.Add("ATTACHED_MESH_BONE_NAME3");
                        checkedListBox1.Items.Add("ATTACHED_MESH_NAME4");
                        checkedListBox1.Items.Add("ATTACHED_MESH_BONE_NAME4");
                        checkedListBox1.Items.Add("ATTACHED_MESH_NAME5");
                        checkedListBox1.Items.Add("ATTACHED_MESH_BONE_NAME5");
                        checkedListBox1.Items.Add("Model Name1");
                        checkedListBox1.Items.Add("Attach FrameName1");
                        checkedListBox1.Items.Add("ATTACHED_PARTICLE_NAME1");
                        checkedListBox1.Items.Add("ATTACHED_PARTICLE_BONE_NAME1");
                        checkedListBox1.Items.Add("ATTACHED_PARTICLE_NAME2");
                        checkedListBox1.Items.Add("ATTACHED_PARTICLE_BONE_NAME2");                        
                        checkedListBox1.Items.Add("Regdate");

                    }
                    break;
                case 1:
                    {
                        checkedListBox1.Items.Add("IndexKey");
                        checkedListBox1.Items.Add("ID");
                        checkedListBox1.Items.Add("TYPE");
                        checkedListBox1.Items.Add("TYPE_");
                        checkedListBox1.Items.Add("TYPE_C");
                        checkedListBox1.Items.Add("VALUE1");
                        checkedListBox1.Items.Add("VALUE2");
                        checkedListBox1.Items.Add("VALUE3");
                        checkedListBox1.Items.Add("Regdate");
                    }
                    break;
                case 2:
                    {
                        checkedListBox1.Items.Add("IndexKey");
                        checkedListBox1.Items.Add("ID");
                        checkedListBox1.Items.Add("Description");
                        checkedListBox1.Items.Add("Socket Type");
                        checkedListBox1.Items.Add("Socket Type_");
                        checkedListBox1.Items.Add("Socket Type_C");
                        checkedListBox1.Items.Add("BASE_HP");
                        checkedListBox1.Items.Add("ATK_PHYSIC");
                        checkedListBox1.Items.Add("ATK_MAGIC");
                        checkedListBox1.Items.Add("DEF_PHYSIC");
                        checkedListBox1.Items.Add("DEF_MAGIC");
                        checkedListBox1.Items.Add("ResistBlaze");
                        checkedListBox1.Items.Add("ResistWater");
                        checkedListBox1.Items.Add("ResistNature");
                        checkedListBox1.Items.Add("ResistWind");
                        checkedListBox1.Items.Add("ResistLight");
                        checkedListBox1.Items.Add("ResistDark");
                        checkedListBox1.Items.Add("HyperModeTime");
                        checkedListBox1.Items.Add("RepairPriceSale");
                        checkedListBox1.Items.Add("MoveSpeed");
                        checkedListBox1.Items.Add("JumpSpeed");
                        checkedListBox1.Items.Add("HyperModeChargeRate");
                        checkedListBox1.Items.Add("ExtraDamageType");
                        checkedListBox1.Items.Add("ExtraDamageType_");
                        checkedListBox1.Items.Add("ExtraDamageType_C");
                        checkedListBox1.Items.Add("Rate");
                        checkedListBox1.Items.Add("Time");
                        checkedListBox1.Items.Add("DamagePerSec");
                        checkedListBox1.Items.Add("Dungeon Only");
                        checkedListBox1.Items.Add("Over Lap");
                        checkedListBox1.Items.Add("Using");
                        checkedListBox1.Items.Add("Random Count");
                        checkedListBox1.Items.Add("Enchant Level");
                        checkedListBox1.Items.Add("DamageReduceValue");
                        checkedListBox1.Items.Add("DamageReduceRate");
                        checkedListBox1.Items.Add("MonsterIDDamageReduced");
                        checkedListBox1.Items.Add("DamageReduceValueByMonsterAttack");
                        checkedListBox1.Items.Add("DamageReduceRateByMonsterAttack");
                        checkedListBox1.Items.Add("EvadeRate");
                        checkedListBox1.Items.Add("CriticalRate");
                        checkedListBox1.Items.Add("CriticalValue");
                        checkedListBox1.Items.Add("Skill ID");
                        checkedListBox1.Items.Add("MPUpRateByAttacked");
                        checkedListBox1.Items.Add("MPUpValueByAttacked");
                        checkedListBox1.Items.Add("DamageUpValueAtDown");
                        checkedListBox1.Items.Add("ExtraDamageResistType");
                        checkedListBox1.Items.Add("ExtraDamageResistType_");
                        checkedListBox1.Items.Add("ExtraDamageResistType_C");
                        checkedListBox1.Items.Add("ExtraDamageResistValue");
                        checkedListBox1.Items.Add("AllSkillLevelUp");
                        checkedListBox1.Items.Add("IncreaseHP");
                        checkedListBox1.Items.Add("DamagePercentUpAtMonster");
                        checkedListBox1.Items.Add("MonsterIDDamagePercentUp");
                        checkedListBox1.Items.Add("ImmuneExtraDamageType");
                        checkedListBox1.Items.Add("ImmuneExtraDamageType_");
                        checkedListBox1.Items.Add("ImmuneExtraDamageType_C");
                        checkedListBox1.Items.Add("ExtraDamageImmunePercent");
                        checkedListBox1.Items.Add("SpeedUpManaGather");
                        checkedListBox1.Items.Add("IncreaseEDPercent");
                        checkedListBox1.Items.Add("IncreaseDCBExpPercent");
                        checkedListBox1.Items.Add("EvadePerUpAtMonster");
                        checkedListBox1.Items.Add("MonsterIDEvadePercentUp");
                        checkedListBox1.Items.Add("AntiEvadeRate");
                        checkedListBox1.Items.Add("AnimSpeedRate");
                        checkedListBox1.Items.Add("TimeSuperArmor");
                        checkedListBox1.Items.Add("PercentSuperArmor");
                        checkedListBox1.Items.Add("DescEvadeByMonsterAttack");
                        checkedListBox1.Items.Add("PVP_Only");
                        checkedListBox1.Items.Add("Regdate");

                    }
                    break;
                case 3:
                    {
                        checkedListBox1.Items.Add("IndexKey");
                        checkedListBox1.Items.Add("SocketDataType");
                        checkedListBox1.Items.Add("SocketDataType_");
                        checkedListBox1.Items.Add("GroupID");
                        checkedListBox1.Items.Add("Rate");
                        checkedListBox1.Items.Add("Regdate");
                    }
                    break;
                case 4:
                    {
                        checkedListBox1.Items.Add("IndexKey");
                        checkedListBox1.Items.Add("GroupID");
                        checkedListBox1.Items.Add("EnchantLvBegin");
                        checkedListBox1.Items.Add("EnchantLvEnd");
                        checkedListBox1.Items.Add("OptionID");
                        checkedListBox1.Items.Add("Option_Des");
                        checkedListBox1.Items.Add("Rate");
                        checkedListBox1.Items.Add("Regdate");
                    }
                    break;
                case 5:
                    {
                        checkedListBox1.Items.Add("IndexKey");
                        checkedListBox1.Items.Add("SetID");
                        checkedListBox1.Items.Add("SetName");
                        checkedListBox1.Items.Add("SetItem_Des");
                        checkedListBox1.Items.Add("NeedPartsNum");
                        checkedListBox1.Items.Add("Option1");
                        checkedListBox1.Items.Add("Option2");
                        checkedListBox1.Items.Add("Option3");
                        checkedListBox1.Items.Add("Option4");
                        checkedListBox1.Items.Add("Option5");
                        checkedListBox1.Items.Add("Option6");
                        checkedListBox1.Items.Add("Option7");
                        checkedListBox1.Items.Add("Option8");
                        checkedListBox1.Items.Add("Option9");                        
                        checkedListBox1.Items.Add("Regdate");
                    }
                    break;
                case 6:
                    {
                        checkedListBox1.Items.Add("IndexKey");
                        checkedListBox1.Items.Add("BoxID");
                        checkedListBox1.Items.Add("BoxName");
                        checkedListBox1.Items.Add("Give All");
                        checkedListBox1.Items.Add("Unit Select");
                        checkedListBox1.Items.Add("Give Character");
                        checkedListBox1.Items.Add("RCount");
                        checkedListBox1.Items.Add("Restore Spirit");
                        checkedListBox1.Items.Add("Key ID");
                        checkedListBox1.Items.Add("Key Name");
                        checkedListBox1.Items.Add("Key Count");
                        checkedListBox1.Items.Add("Unit Class_01");
                        checkedListBox1.Items.Add("Unit Class_01_");
                        checkedListBox1.Items.Add("Unit Class_01_C");
                        checkedListBox1.Items.Add("Group ID_01");
                        checkedListBox1.Items.Add("Unit Class_02");
                        checkedListBox1.Items.Add("Unit Class_02_");
                        checkedListBox1.Items.Add("Unit Class_02_C");
                        checkedListBox1.Items.Add("Group ID_02");
                        checkedListBox1.Items.Add("Unit Class_03");
                        checkedListBox1.Items.Add("Unit Class_03_");
                        checkedListBox1.Items.Add("Unit Class_03_C");
                        checkedListBox1.Items.Add("Group ID_03");
                        checkedListBox1.Items.Add("Unit Class_04");
                        checkedListBox1.Items.Add("Unit Class_04_");
                        checkedListBox1.Items.Add("Unit Class_04_C");
                        checkedListBox1.Items.Add("Group ID_04");
                        checkedListBox1.Items.Add("Unit Class_05");
                        checkedListBox1.Items.Add("Unit Class_05_");
                        checkedListBox1.Items.Add("Unit Class_05_C");
                        checkedListBox1.Items.Add("Group ID_05");
                        checkedListBox1.Items.Add("Unit Class_06");
                        checkedListBox1.Items.Add("Unit Class_06_");
                        checkedListBox1.Items.Add("Unit Class_06_C");
                        checkedListBox1.Items.Add("Group ID_06");
                        checkedListBox1.Items.Add("Unit Class_07");
                        checkedListBox1.Items.Add("Unit Class_07_");
                        checkedListBox1.Items.Add("Unit Class_07_C");
                        checkedListBox1.Items.Add("Group ID_07");
                        checkedListBox1.Items.Add("Unit Class_08");
                        checkedListBox1.Items.Add("Unit Class_08_");
                        checkedListBox1.Items.Add("Unit Class_08_C");
                        checkedListBox1.Items.Add("Group ID_08");
                        checkedListBox1.Items.Add("Unit Class_09");
                        checkedListBox1.Items.Add("Unit Class_09_");
                        checkedListBox1.Items.Add("Unit Class_09_C");
                        checkedListBox1.Items.Add("Group ID_09");
                        checkedListBox1.Items.Add("Unit Class_10");
                        checkedListBox1.Items.Add("Unit Class_10_");
                        checkedListBox1.Items.Add("Unit Class_10_C");
                        checkedListBox1.Items.Add("Group ID_10");
                        checkedListBox1.Items.Add("Unit Class_11");
                        checkedListBox1.Items.Add("Unit Class_11_");
                        checkedListBox1.Items.Add("Unit Class_11_C");
                        checkedListBox1.Items.Add("Group ID_11");
                        checkedListBox1.Items.Add("Unit Class_12");
                        checkedListBox1.Items.Add("Unit Class_12_");
                        checkedListBox1.Items.Add("Unit Class_12_C");
                        checkedListBox1.Items.Add("Group ID_12");
                        checkedListBox1.Items.Add("Unit Class_13");
                        checkedListBox1.Items.Add("Unit Class_13_");
                        checkedListBox1.Items.Add("Unit Class_13_C");
                        checkedListBox1.Items.Add("Group ID_13");
                        checkedListBox1.Items.Add("Unit Class_14");
                        checkedListBox1.Items.Add("Unit Class_14_");
                        checkedListBox1.Items.Add("Unit Class_14_C");
                        checkedListBox1.Items.Add("Group ID_14");
                        checkedListBox1.Items.Add("Unit Class_15");
                        checkedListBox1.Items.Add("Unit Class_15_");
                        checkedListBox1.Items.Add("Unit Class_15_C");
                        checkedListBox1.Items.Add("Group ID_15");
                        checkedListBox1.Items.Add("Regdate");
                    }
                    break;
                case 7:
                    {
                        checkedListBox1.Items.Add("IndexKey");
                        checkedListBox1.Items.Add("Group ID");
                        checkedListBox1.Items.Add("Item ID");
                        checkedListBox1.Items.Add("Item Name");
                        checkedListBox1.Items.Add("Rate");
                        checkedListBox1.Items.Add("Period");
                        checkedListBox1.Items.Add("Quantity");
                        checkedListBox1.Items.Add("Regdate");
                    }
                    break;
                case 8:
                    {
                        checkedListBox1.Items.Add("IndexKey");
                        checkedListBox1.Items.Add("House_ID");
                        checkedListBox1.Items.Add("House_Enum");
                        checkedListBox1.Items.Add("SourceItemID");
                        checkedListBox1.Items.Add("Exchange_Des1");
                        checkedListBox1.Items.Add("Exchange_Num1");
                        checkedListBox1.Items.Add("ExchangeType");
                        checkedListBox1.Items.Add("ExchangeType_Enum");
                        checkedListBox1.Items.Add("ResultItemID");
                        checkedListBox1.Items.Add("Exchange_Des2");
                        checkedListBox1.Items.Add("Exchange_Num2");
                        checkedListBox1.Items.Add("Quantity");
                        checkedListBox1.Items.Add("Rate");
                        checkedListBox1.Items.Add("Regdate");

                    }
                    break;
                case 9:
                    {
                        checkedListBox1.Items.Add("IndexKey");
                        checkedListBox1.Items.Add("Manufacture ID");
                        checkedListBox1.Items.Add("Manufacture Name");
                        checkedListBox1.Items.Add("m_ManufactureType");
                        checkedListBox1.Items.Add("ManufactureType_");
                        checkedListBox1.Items.Add("ManufactureType_C");
                        checkedListBox1.Items.Add("Cost");
                        checkedListBox1.Items.Add("Result Group ID");
                        checkedListBox1.Items.Add("Result Name");
                        checkedListBox1.Items.Add("Result Item Probability");
                        checkedListBox1.Items.Add("Material ID");
                        checkedListBox1.Items.Add("Material Name");
                        checkedListBox1.Items.Add("Material Count");
                        checkedListBox1.Items.Add("IsMaterialBegin");
                        checkedListBox1.Items.Add("Regdate");
                    }
                    break;
                case 10:
                    {
                        checkedListBox1.Items.Add("IndexKey");
                        checkedListBox1.Items.Add("Group ID");
                        checkedListBox1.Items.Add("Item ID");
                        checkedListBox1.Items.Add("Item Name");
                        checkedListBox1.Items.Add("Rate");
                        checkedListBox1.Items.Add("Regdate");
                    }
                    break;
                case 11:
                    {
                        checkedListBox1.Items.Add("IndexKey");
                        checkedListBox1.Items.Add("Manufacture ID");
                        checkedListBox1.Items.Add("Manufacture Name");
                        checkedListBox1.Items.Add("Village ID");
                        checkedListBox1.Items.Add("Village Name");
                        checkedListBox1.Items.Add("Village Enum");
                        checkedListBox1.Items.Add("Regdate");
                    }
                    break;
                case 12:
                    {
                        checkedListBox1.Items.Add("IndexKey");
                        checkedListBox1.Items.Add("ExtraDamageType");
                        checkedListBox1.Items.Add("ExtraDamageType_");
                        checkedListBox1.Items.Add("ExtraDamageType_C");
                        checkedListBox1.Items.Add("IsForNPC");
                        checkedListBox1.Items.Add("Rate");
                        checkedListBox1.Items.Add("Time");
                        checkedListBox1.Items.Add("FirstDamagePercent");
                        checkedListBox1.Items.Add("SecondDamagePercent");
                        checkedListBox1.Items.Add("ThirdDamagePercent");
                        checkedListBox1.Items.Add("FirstSlowPercent");
                        checkedListBox1.Items.Add("SecondSlowPercent");
                        checkedListBox1.Items.Add("ThirdSlowPercent");
                        checkedListBox1.Items.Add("StunTime");
                        checkedListBox1.Items.Add("HPDrainPercent");
                        checkedListBox1.Items.Add("MPDrain");
                        checkedListBox1.Items.Add("FirstDefenceDebuffPercent");
                        checkedListBox1.Items.Add("SecondDefenceDebuffPercent");
                        checkedListBox1.Items.Add("ThirdDefenceDebuffPercent");
                        checkedListBox1.Items.Add("Regdate");
                    }
                    break;
                case 13:
                    {
                        checkedListBox1.Items.Add("IndexKey");
                        checkedListBox1.Items.Add("Enchant Type");
                        checkedListBox1.Items.Add("Enchant Type_");
                        checkedListBox1.Items.Add("Char Lv");
                        checkedListBox1.Items.Add("LOW");
                        checkedListBox1.Items.Add("NORMAL");
                        checkedListBox1.Items.Add("RARE");
                        checkedListBox1.Items.Add("ELITE");
                        checkedListBox1.Items.Add("UNIQUE");
                        checkedListBox1.Items.Add("Regdate");
                    }
                    break;
                case 14:
                    {
                        checkedListBox1.Items.Add("IndexKey");
                        checkedListBox1.Items.Add("NPC_ABILITY_ID");
                        checkedListBox1.Items.Add("NPC_ABILITY_ID_");
                        checkedListBox1.Items.Add("NPC_ABILITY_ID_C");
                        checkedListBox1.Items.Add("Front");
                        checkedListBox1.Items.Add("PhysicAttackUpPercent");
                        checkedListBox1.Items.Add("MagicAttackUpPercent");
                        checkedListBox1.Items.Add("CriticalPercent");
                        checkedListBox1.Items.Add("AllSpeedUpPercent");
                        checkedListBox1.Items.Add("PhysicDefenseUpPercent");
                        checkedListBox1.Items.Add("MagicDefenseUpPercent");
                        checkedListBox1.Items.Add("AlwaysSuperArmor");
                        checkedListBox1.Items.Add("Heavy");
                        checkedListBox1.Items.Add("Regdate");
                    }
                    break;
            }
        }

        void ColumnLoad_QuestTable()
        {
            switch (m_nSelectTab)
            {
                case 0:
                    {
                        checkedListBox1.Items.Add("IndexKey");
                        checkedListBox1.Items.Add("ID");
                        checkedListBox1.Items.Add("Enable Quest");
                        checkedListBox1.Items.Add("Quest Type");
                        checkedListBox1.Items.Add("Start NPC Name");
                        checkedListBox1.Items.Add("End NPC Name");
                        checkedListBox1.Items.Add("Title");
                        checkedListBox1.Items.Add("Fair Level");
                        checkedListBox1.Items.Add("Fair Dungeon ID");
                        checkedListBox1.Items.Add("Quest Repeat");
                        checkedListBox1.Items.Add("Condition Lv");
                        checkedListBox1.Items.Add("Condition Unit Type");
                        checkedListBox1.Items.Add("Condition Unit Class");
                        checkedListBox1.Items.Add("Reward Unit Class");
                        checkedListBox1.Items.Add("Change Job Type");
                        checkedListBox1.Items.Add("Condition Item ID");
                        checkedListBox1.Items.Add("Condition Item Quantity");
                        checkedListBox1.Items.Add("SubQuest1");
                        checkedListBox1.Items.Add("SubQuest2");
                        checkedListBox1.Items.Add("SubQuest3");
                        checkedListBox1.Items.Add("SubQuest4");
                        checkedListBox1.Items.Add("SubQUest5");
                        checkedListBox1.Items.Add("Reward Enable");
                        checkedListBox1.Items.Add("Reward EXP");
                        checkedListBox1.Items.Add("Reward ED");
                        checkedListBox1.Items.Add("Reward Grade");
                        checkedListBox1.Items.Add("Reward Buff");
                        checkedListBox1.Items.Add("Reward SP");
                        checkedListBox1.Items.Add("Reward Item1");
                        checkedListBox1.Items.Add("name1");
                        checkedListBox1.Items.Add("Period 1");
                        checkedListBox1.Items.Add("Reward ItemQuantity1");
                        checkedListBox1.Items.Add("Socket Option1");
                        checkedListBox1.Items.Add("Reward Item2");
                        checkedListBox1.Items.Add("name2");
                        checkedListBox1.Items.Add("Period 2");
                        checkedListBox1.Items.Add("Reward ItemQuantity2");
                        checkedListBox1.Items.Add("Socket Option2");
                        checkedListBox1.Items.Add("Reward Item3");
                        checkedListBox1.Items.Add("name3");
                        checkedListBox1.Items.Add("Period 3");
                        checkedListBox1.Items.Add("Reward ItemQuantity3");
                        checkedListBox1.Items.Add("Socket Option3");
                        checkedListBox1.Items.Add("Select Reward Item1");
                        checkedListBox1.Items.Add("name1_1");
                        checkedListBox1.Items.Add("Select Period1");
                        checkedListBox1.Items.Add("Select Reward ItemQuantity1");
                        checkedListBox1.Items.Add("Select Socket Option1");
                        checkedListBox1.Items.Add("Select Reward Item2");
                        checkedListBox1.Items.Add("name2_1");
                        checkedListBox1.Items.Add("Select Period2");
                        checkedListBox1.Items.Add("Select Reward ItemQuantity2");
                        checkedListBox1.Items.Add("Select Socket Option2");
                        checkedListBox1.Items.Add("Select Reward Item3");
                        checkedListBox1.Items.Add("name3_1");
                        checkedListBox1.Items.Add("Select Period3");
                        checkedListBox1.Items.Add("Select Reward ItemQuantity3");
                        checkedListBox1.Items.Add("Select Socket Option3");
                        checkedListBox1.Items.Add("Select Reward Item4");
                        checkedListBox1.Items.Add("name4_1");
                        checkedListBox1.Items.Add("Select Period4");
                        checkedListBox1.Items.Add("Select Reward ItemQuantity4");
                        checkedListBox1.Items.Add("Select Socket Option4");
                        checkedListBox1.Items.Add("Select Reward Item5");
                        checkedListBox1.Items.Add("name5_1");
                        checkedListBox1.Items.Add("Select Period 5");
                        checkedListBox1.Items.Add("Select Reward ItemQuantity5");
                        checkedListBox1.Items.Add("Select Socket Option5");
                        checkedListBox1.Items.Add("Select Reward ItemCount");
                        checkedListBox1.Items.Add("Main Text");
                        checkedListBox1.Items.Add("Thanks Text");
                        checkedListBox1.Items.Add("Diss Clear Text");
                        checkedListBox1.Items.Add("Condition ClearQuest ID 1");
                        checkedListBox1.Items.Add("Condition ClearQuest ID 2");
                        checkedListBox1.Items.Add("Condition ClearQuest ID 3");
                        checkedListBox1.Items.Add("Condition ClearQuest ID 4");
                        checkedListBox1.Items.Add("Condition ClearQuest ID 5");
                        checkedListBox1.Items.Add("Condition ClearQuest ID 6");
                        checkedListBox1.Items.Add("Condition ClearQuest ID 7");
                        checkedListBox1.Items.Add("Condition ClearQuest ID 8");
                        checkedListBox1.Items.Add("Condition DisClearQuest ID");                        
                        checkedListBox1.Items.Add("Regdate");
                    }
                    break;
                case 1 :
                    {
                        checkedListBox1.Items.Add("IndexKey");
                        checkedListBox1.Items.Add("ID");
                        checkedListBox1.Items.Add("Clear Type");
                        checkedListBox1.Items.Add("Clear Condition Kill NPC ID");
                        checkedListBox1.Items.Add("Clear Condition Kill NPC Quantity");
                        checkedListBox1.Items.Add("Clear Condition Collection Item ID");
                        checkedListBox1.Items.Add("Name");
                        checkedListBox1.Items.Add("Clear Condition Collection Item Quantity");
                        checkedListBox1.Items.Add("Quest Item Drop Rate");
                        checkedListBox1.Items.Add("Clear Condition Dungeon Difficulty");
                        checkedListBox1.Items.Add("Clear Condition Upper Difficulty");
                        checkedListBox1.Items.Add("Clear Condition Dungeon ID");
                        checkedListBox1.Items.Add("Clear Condition Dungeon Damage");
                        checkedListBox1.Items.Add("Clear Condition Dungeon Rank");
                        checkedListBox1.Items.Add("Clear Condition Dungeon Clear Time");
                        checkedListBox1.Items.Add("Clear Condition Dungeon clear count");
                        checkedListBox1.Items.Add("Clear Condition Talk NPC ID");
                        checkedListBox1.Items.Add("Clear Condition PVP_TYPE");
                        checkedListBox1.Items.Add("Clear Condition PVP_PLAY");
                        checkedListBox1.Items.Add("Clear Condition PVP_WIN");
                        checkedListBox1.Items.Add("Clear Condition PVP_KILL");
                        checkedListBox1.Items.Add("AutomaticDescription");
                        checkedListBox1.Items.Add("Description");                        
                        checkedListBox1.Items.Add("Regdate"); 
                    }
                    break;
            }
        }


        void ColumnLoad_TitleTable()
        {
            switch (m_nSelectTab)
            {
                case 0:
                    {
                        checkedListBox1.Items.Add("IndexKey");
                        checkedListBox1.Items.Add("TitleID");
                        checkedListBox1.Items.Add("Title_Name");
                        checkedListBox1.Items.Add("Description");
                        checkedListBox1.Items.Add("Sort_Num");
                        checkedListBox1.Items.Add("Title_Type");
                        checkedListBox1.Items.Add("Title_Type_");
                        checkedListBox1.Items.Add("Particle_Name");
                        checkedListBox1.Items.Add("Condition Unit Type");
                        checkedListBox1.Items.Add("Condition_Unit_Type_");
                        checkedListBox1.Items.Add("Condition Unit Class");
                        checkedListBox1.Items.Add("Condition_Unit_Class_");
                        checkedListBox1.Items.Add("Open_Level");
                        checkedListBox1.Items.Add("Secret_Title");
                        checkedListBox1.Items.Add("Base_HP");
                        checkedListBox1.Items.Add("Atk_Physic");
                        checkedListBox1.Items.Add("Atk_Magic");
                        checkedListBox1.Items.Add("Def_Physic");
                        checkedListBox1.Items.Add("Def_Magic");
                        checkedListBox1.Items.Add("Socket01");
                        checkedListBox1.Items.Add("Socket02");
                        checkedListBox1.Items.Add("Socket03");
                        checkedListBox1.Items.Add("Socket04");
                        checkedListBox1.Items.Add("Socket05");
                        checkedListBox1.Items.Add("Socket06");
                        checkedListBox1.Items.Add("Socket07");
                        checkedListBox1.Items.Add("Socket08");
                        checkedListBox1.Items.Add("Socket09");
                        checkedListBox1.Items.Add("Socket10");
                        checkedListBox1.Items.Add("Visible");                        
                        checkedListBox1.Items.Add("Regdate"); 
                    }
                    break;
                case 1:
                    {
                        checkedListBox1.Items.Add("IndexKey");
                        checkedListBox1.Items.Add("ID");
                        checkedListBox1.Items.Add("Mission Clear Type");
                        checkedListBox1.Items.Add("Mission_Clear_Type_");
                        checkedListBox1.Items.Add("Mission_Name");
                        checkedListBox1.Items.Add("Condition_Lv");
                        checkedListBox1.Items.Add("Condition Unit Type");
                        checkedListBox1.Items.Add("Condition_Unit_Type_");
                        checkedListBox1.Items.Add("Condition Unit Class");
                        checkedListBox1.Items.Add("Condition_Unit_Class_");
                        checkedListBox1.Items.Add("SubMission_1");
                        checkedListBox1.Items.Add("SubMission_2");
                        checkedListBox1.Items.Add("SubMission_3");
                        checkedListBox1.Items.Add("SubMission_4");
                        checkedListBox1.Items.Add("SubMission_5");
                        checkedListBox1.Items.Add("Period");
                        checkedListBox1.Items.Add("Reward_Title_ID");
                        checkedListBox1.Items.Add("Title Name");
                        checkedListBox1.Items.Add("Condition_ClearMission_ID");                        
                        checkedListBox1.Items.Add("Regdate");
                    }
                    break;

                case 2:
                    {
                        checkedListBox1.Items.Add("IndexKey");
                        checkedListBox1.Items.Add("ID");
                        checkedListBox1.Items.Add("Clear_Type_Idx");
                        checkedListBox1.Items.Add("Clear_Type_");
                        checkedListBox1.Items.Add("NPC ID");
                        checkedListBox1.Items.Add("Clear Condition Kill NPC ID");
                        checkedListBox1.Items.Add("Clear Condition Kill NPC Quantity");
                        checkedListBox1.Items.Add("Clear_Condition_Collection_Item_ID");
                        checkedListBox1.Items.Add("Name");
                        checkedListBox1.Items.Add("Clear_Condition_Collection_Item_Quantity");
                        checkedListBox1.Items.Add("Quest_Item_Drop_Rate");
                        checkedListBox1.Items.Add("Clear_Condition_Dungeon_Difficulty");
                        checkedListBox1.Items.Add("Clear_Condition_Dungeon_ID");
                        checkedListBox1.Items.Add("Clear_Condition_Dungeon_Damage");
                        checkedListBox1.Items.Add("Clear_Condition_Dungeon_Rank");
                        checkedListBox1.Items.Add("Clear_Condition_Dungeon_Clear_Time");
                        checkedListBox1.Items.Add("Clear_Condition_Dungeon_clear_count");
                        checkedListBox1.Items.Add("Clear_Condition_Talk_NPC_ID");
                        checkedListBox1.Items.Add("Clear_Condition_PVP_TYPE");
                        checkedListBox1.Items.Add("Clear_Condition_PVP_PLAY");
                        checkedListBox1.Items.Add("Clear_Condition_PVP_WIN");
                        checkedListBox1.Items.Add("Clear_Condition_PVP_KILL");
                        checkedListBox1.Items.Add("Clear_Condition_QUEST");
                        checkedListBox1.Items.Add("Clear_Condition_COLLECT_TITLE");
                        checkedListBox1.Items.Add("Clear_Condition_USE_ITEM");
                        checkedListBox1.Items.Add("AutomaticDescription");
                        checkedListBox1.Items.Add("Description");                        
                        checkedListBox1.Items.Add("Regdate"); 
                    }
                    break;
            }
        }

        void ColumnLoad_SkillTable()
        {
            switch (m_nSelectTab)
            {
                case 0:
                    {
                        checkedListBox1.Items.Add("IndexKey");
                        checkedListBox1.Items.Add("ID");
                        checkedListBox1.Items.Add("ID_ENUM");
                        checkedListBox1.Items.Add("TYPE");
                        checkedListBox1.Items.Add("TYPE_ENUM");
                        checkedListBox1.Items.Add("NAME");
                        checkedListBox1.Items.Add("DESC");
                        checkedListBox1.Items.Add("SKILL_LEVEL");
                        checkedListBox1.Items.Add("POWER_RATE");
                        checkedListBox1.Items.Add("COOL_TIME");
                        checkedListBox1.Items.Add("MP_CONSUMPTION");
                        checkedListBox1.Items.Add("STAT_BASE_HP");
                        checkedListBox1.Items.Add("STAT_ATK_PHYSIC");
                        checkedListBox1.Items.Add("STAT_ATK_MAGIC");
                        checkedListBox1.Items.Add("STAT_DEF_PHYSIC");
                        checkedListBox1.Items.Add("STAT_DEF_MAGIC");
                        checkedListBox1.Items.Add("ICON_TEXTURE_NAME");
                        checkedListBox1.Items.Add("ICON_TEXTURE_PIECE_NAME");
                        checkedListBox1.Items.Add("BORN_SEALED");
                        checkedListBox1.Items.Add("SHOW_LEVEL");
                        checkedListBox1.Items.Add("ACTIVE_SKILL_USE_CONDITION");
                        checkedListBox1.Items.Add("ACTIVE_SKILL_USE_CONDITION_ENUM");
                        checkedListBox1.Items.Add("Description");
                        checkedListBox1.Items.Add("IN_HOUSE_TEST_ONLY");
                        checkedListBox1.Items.Add("MISC_0");
                        checkedListBox1.Items.Add("MISC_1");
                        checkedListBox1.Items.Add("MISC_2");
                        checkedListBox1.Items.Add("MISC_3");
                        checkedListBox1.Items.Add("MISC_4");
                        checkedListBox1.Items.Add("MISC_5");
                        checkedListBox1.Items.Add("Regdate");
                        
                    }
                    break;
                case 16:
                    {
                        checkedListBox1.Items.Add("IndexKey");
                        checkedListBox1.Items.Add("ID Num");
                        checkedListBox1.Items.Add("ID Name");
                        checkedListBox1.Items.Add("Description");
                        checkedListBox1.Items.Add("Regdate");
                        
                    }
                    break;

            }
        }


        private void button1_Click(object sender, EventArgs e)
        {
            for (int i = 0; i < checkedListBox1.Items.Count; i++)
            {
                dgvTemp.Columns[i].Visible = checkedListBox1.GetItemChecked(i);
            }
            this.Hide();
        }

        private void ColumnManager_Load(object sender, EventArgs e)
        {
            if(this.Owner.Name == "BillingTable")
            {
                ColumnLoad_BillingTable();
            }
            else if (this.Owner.Name == "QuestTable")
            {
                ColumnLoad_QuestTable();
            }
            else if (this.Owner.Name == "TitleTable")
            {
                ColumnLoad_TitleTable();
            }
            else if (this.Owner.Name == "SkillTable")
            {
                ColumnLoad_SkillTable();
            }

            

            for (int i = 0; i < checkedListBox1.Items.Count; i++)
            {
                checkedListBox1.SetItemChecked(i, dgvTemp.Columns[i].Visible);
            }            
        }

        private void ColumnManager_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar == (char)Keys.Escape)
            {
                this.Hide();
            }
        }

        private void checkedListBox1_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar == (char)Keys.Escape)
            {
                this.Hide();
            }
        }

        private void ColumnManager_Resize(object sender, EventArgs e)
        {
            checkedListBox1.Width = this.Width - 15;
            checkedListBox1.Height = this.Height - 78;
            
            button1.Left = checkedListBox1.Location.X + checkedListBox1.Width - button1.Width;
            button1.Top = checkedListBox1.Location.Y + checkedListBox1.Height + 4;

            btnLoad.Left = button1.Left - btnLoad.Width -10;
            btnLoad.Top = checkedListBox1.Location.Y + checkedListBox1.Height + 4;

            btnSave.Left = btnLoad.Left - btnSave.Width - 10;
            btnSave.Top = checkedListBox1.Location.Y + checkedListBox1.Height + 4;
            
        }

        private void button2_Click(object sender, EventArgs e)
        {
            for(int i = 0; i < checkedListBox1.Items.Count; i ++)
            {
                checkedListBox1.SetItemChecked(i, true);
            }
        }

        private void button3_Click(object sender, EventArgs e)
        {
            for (int i = 0; i < checkedListBox1.Items.Count; i++)
            {
                checkedListBox1.SetItemChecked(i, false);
            }
        }

        private void btnSave_Click(object sender, EventArgs e)
        {
            FileDialog SaveFile = new SaveFileDialog();


            SaveFile.InitialDirectory = Directory.GetCurrentDirectory();
            SaveFile.Filter = ".Col|*.Col";
            SaveFile.Title = "Save File";
            DialogResult DResult = SaveFile.ShowDialog();       
            if (DResult == DialogResult.OK)
            {
                StreamWriter sw = new StreamWriter(SaveFile.FileName, false, Encoding.Default);

                sw.Write(this.Owner.Name + "\n");


                for (int i = 0; i < checkedListBox1.Items.Count; i++)
                {
                    sw.Write(checkedListBox1.GetItemChecked(i) + "\t");
                }
                sw.Close();
            }

        }

        private void btnLoad_Click(object sender, EventArgs e)
        {
            FileDialog OpenFile = new OpenFileDialog();
            OpenFile.Filter = ".Col|*.Col";
            OpenFile.Title = "Open File";
            DialogResult DResult = OpenFile.ShowDialog();
            if (DResult == DialogResult.OK)
            {
                StreamReader sr = new StreamReader(OpenFile.FileName, Encoding.Default);

                
                string strTableName;
                strTableName = sr.ReadLine();
                strTableName.Replace("\n", "");
                if (this.Owner.Name != strTableName)
                {
                    MessageBox.Show("해당테이블 Column정보가 아닙니다.");
                }

                
                string strColumn = sr.ReadLine();
                string delimstr = "\t";
                char[] delimiter = delimstr.ToCharArray();
                string[] arrData = strColumn.Split(delimiter, strColumn.Length);

                for (int i = 0; i < checkedListBox1.Items.Count; i++)
                {
                    checkedListBox1.SetItemChecked(i, Convert.ToBoolean(arrData[i]));
                } 
                
                sr.Close();
            }
        }
    }
}