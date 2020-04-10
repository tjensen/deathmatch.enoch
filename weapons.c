class RangedWeapon
{
    EntityAI CreateInInventory(HumanInventory inventory)
    {
        EntityAI weapon = inventory.CreateInInventory(GetName());
        return Equip(inventory, weapon);
    }

    EntityAI CreateInHands(HumanInventory inventory)
    {
        EntityAI weapon = inventory.CreateInHands(GetName());
        return Equip(inventory, weapon);
    }

    private EntityAI Equip(HumanInventory inventory, EntityAI weapon)
    {
        GameInventory weaponInventory = weapon.GetInventory();

        EntityAI optic = CreateAttachment(weaponInventory, GetOptic());
        if (optic != null)
        {
            optic.GetInventory().CreateAttachment("Battery9V");
        }

        CreateAttachment(weaponInventory, GetHandguard());
        CreateAttachment(weaponInventory, GetButtstock());

        string ammunition = GetAmmunition();
        inventory.CreateInInventory(ammunition);
        inventory.CreateInInventory(ammunition);
        inventory.CreateInInventory(ammunition);

        return weapon;
    }

    private EntityAI CreateAttachment(GameInventory inventory, string attachment)
    {
        if (attachment == "") return null;

        return inventory.CreateAttachment(attachment);
    }

    string GetName() {return "";}
    string GetAmmunition() {return "";}
    string GetOptic() {return "";}
    string GetHandguard() {return "";}
    string GetButtstock() {return "";}
}


class FixedRangedWeapon extends RangedWeapon
{
    string m_name;
    string m_ammunition;
    string m_optic;
    string m_handguard;
    string m_buttstock;

    void Init(string name, string ammunition, string optic = "", string handguard = "", string buttstock = "")
    {
        m_name = name;
        m_ammunition = ammunition;
        m_optic = optic;
        m_handguard = handguard;
        m_buttstock = buttstock;
    }

    string GetName()
    {
        return m_name;
    }

    string GetAmmunition()
    {
        return m_ammunition;
    }

    string GetOptic()
    {
        return m_optic;
    }

    string GetHandguard()
    {
        return m_handguard;
    }

    string GetButtstock()
    {
        return m_buttstock;
    }
}


class RandomRangedWeapon extends RangedWeapon
{
    static ref TStringArray EMPTY = new TStringArray();

    private TStringArray m_names;
    private string m_ammunition;
    private TStringArray m_optics;
    private TStringArray m_handguards;
    private TStringArray m_buttstocks;

    void Init(
        TStringArray names, string ammunition, TStringArray optics = EMPTY,
        TStringArray handguards = EMPTY, TStringArray buttstocks = EMPTY)
    {
        m_names = names;
        m_optics = optics;
        m_handguards = handguards;
        m_buttstocks = buttstocks;
        m_ammunition = ammunition;
    }

    string GetName()
    {
        return GetRandomElement(m_names);
    }

    string GetAmmunition()
    {
        return m_ammunition;
    }

    string GetOptic()
    {
        return GetRandomElement(m_optics);
    }

    string GetHandguard()
    {
        return GetRandomElement(m_handguards);
    }

    string GetButtstock()
    {
        return GetRandomElement(m_buttstocks);
    }

    private string GetRandomElement(TStringArray elements)
    {
        if (elements == null) return "";
        if (elements.Count() == 0) return "";

        return elements.GetRandomElement();
    }
}


class Winchester extends FixedRangedWeapon
{
    void Winchester()
    {
        Init("Winchester70", "Ammo_308Win", "HuntingOptic");
    }
}


class Blaze extends FixedRangedWeapon
{
    void Blaze()
    {
        Init("B95", "Ammo_308Win", "HuntingOptic");
    }
}


class Mosin extends FixedRangedWeapon
{
    void Mosin()
    {
        Init("Mosin9130", "Ammo_762x54", "PUScopeOptic");
    }
}


class SKS extends FixedRangedWeapon
{
    void SKS()
    {
        Init("SKS", "Ammo_762x39", "PUScopeOptic");
    }
}


class MP5 extends RandomRangedWeapon
{
    static ref TStringArray Names = {"MP5K"};
    static ref TStringArray Optics = {"M68Optic", "M4_T3NRDSOptic", "ReflexOptic"};
    static ref TStringArray Handguards = {"MP5_PlasticHndgrd", "MP5_RailHndgrd"};
    static ref TStringArray Buttstocks = {"MP5k_StockBttstck"};

    void MP5()
    {
        Init(Names, "Mag_MP5_30Rnd", Optics, Handguards, Buttstocks);
    }
}


class UMP extends RandomRangedWeapon
{
    static ref TStringArray Names = {"UMP45"};
    static ref TStringArray Optics = {"M68Optic", "M4_T3NRDSOptic", "ReflexOptic"};

    void UMP()
    {
        Init(Names, "Mag_UMP_25Rnd", Optics);
    }
}


class M4A1 extends RandomRangedWeapon
{
    static ref TStringArray Names = {"M4A1"};
    static ref TStringArray Optics = {"M68Optic", "M4_T3NRDSOptic", "ReflexOptic"};
    static ref TStringArray Handguards = {"M4_MPHndgrd", "M4_PlasticHndgrd", "M4_RISHndgrd"};
    static ref TStringArray Buttstocks = {"M4_CQBBttstck", "M4_MPBttstck", "M4_OEBttstck"};

    void M4A1()
    {
        Init(Names, "Mag_STANAG_30Rnd", Optics, Handguards, Buttstocks);
    }
}


class Shotgun extends RandomRangedWeapon
{
    static ref TStringArray Names = {"Izh43Shotgun", "Mp133Shotgun"};

    void Shotgun()
    {
        Init(Names, "Ammo_12gaPellets");
    }
}


class Vaiga extends FixedRangedWeapon
{
    void Vaiga()
    {
        Init("Saiga", "Mag_Saiga_8Rnd", "", "", "Saiga_Bttstck");
    }
}
