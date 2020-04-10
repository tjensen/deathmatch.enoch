void SpawnCrate()
{
    vector position = "7332.09 295.65 2667.92";
    ItemBase chest = ItemBase.Cast(GetGame().CreateObject("SeaChest", position, false, false, false));
    Magazine magazine;

    chest.SetPosition(position);
    chest.GetInventory().CreateInInventory("VSS");
    magazine = Magazine.Cast(chest.GetInventory().CreateInInventory("Mag_VSS_10Rnd"));
    magazine.ServerSetAmmoCount(0);
    chest.GetInventory().CreateInInventory("AmmoBox_9x39AP_20Rnd");
    chest.GetInventory().CreateInInventory("AmmoBox_9x39AP_20Rnd");
}

void main()
{
    //INIT WEATHER BEFORE ECONOMY INIT------------------------
    Weather weather = g_Game.GetWeather();

    weather.MissionWeather(false);    // false = use weather controller from Weather.c

    weather.GetOvercast().Set(Math.RandomFloatInclusive(0.02, 0.1), 1, 0);
    weather.GetRain().Set(0, 1, 0);
    weather.GetFog().Set(0, 1, 0);

    //INIT ECONOMY--------------------------------------
    Hive ce = CreateHive();
    if (ce)
        ce.InitOffline();

    //DATE RESET AFTER ECONOMY INIT-------------------------
    int year, month, day, hour, minute;
    int reset_month = 8, reset_day = 10;
    GetGame().GetWorld().GetDate(year, month, day, hour, minute);

    if ((month == reset_month) && (day < reset_day))
    {
        GetGame().GetWorld().SetDate(year, reset_month, reset_day, hour, minute);
    }
    else
    {
        if ((month == reset_month + 1) && (day > reset_day))
        {
            GetGame().GetWorld().SetDate(year, reset_month, reset_day, hour, minute);
        }
        else
        {
            if ((month < reset_month) || (month > reset_month + 1))
            {
                GetGame().GetWorld().SetDate(year, reset_month, reset_day, hour, minute);
            }
        }
    }

    SpawnCrate();
}

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

class CustomMission: MissionServer
{
    ref TStringArray Tops = {
        "BDUJacket", "Blouse_Green", "BomberJacket_Black", "BomberJacket_Brown",
        "BomberJacket_Olive", "DenimJacket", "HikingJacket_Black", "HikingJacket_Green",
        "Hoodie_Black", "Hoodie_Brown", "Hoodie_Green", "HuntingJacket_Brown",
        "HuntingJacket_Summer", "LabCoat", "M65Jacket_Black", "M65Jacket_Olive",
        "ParamedicJacket_Green", "PrisonUniformJacket", "QuiltedJacket_Black",
        "QuiltedJacket_Green", "TacticalShirt_Black", "TacticalShirt_Olive", "WoolCoat_Black"
    };
    ref TStringArray Bottoms = {
        "BDUPants", "CargoPants_Black", "CargoPants_Green", "HunterPants_Brown",
        "HunterPants_Summer", "Jeans_Black", "Jeans_Brown", "Jeans_Green", "ParamedicPants_Green",
        "PrisonUniformPants", "ShortJeans_Black", "ShortJeans_Brown", "ShortJeans_Green"
    };
    ref TStringArray Shoes = {
        "AthleticShoes_Black", "AthleticShoes_Brown", "AthleticShoes_Green", "CombatBoots_Beige",
        "CombatBoots_Black", "CombatBoots_Brown", "CombatBoots_Green", "CombatBoots_Grey",
        "HikingBoots_Black", "HikingBoots_Brown", "JungleBoots_Black", "JungleBoots_Brown",
        "JungleBoots_Green", "JungleBoots_Olive", "MilitaryBoots_Black", "MilitaryBoots_Brown",
        "Sneakers_Black", "Sneakers_Green", "Wellies_Black", "Wellies_Brown", "Wellies_Green",
        "WorkingBoots_Brown", "WorkingBoots_Green"
    };
    ref TStringArray Vests = {
        "PressVest_Blue", "PressVest_LightBlue", "UKAssVest_Black", "UKAssVest_Camo",
        "UKAssVest_Khaki", "UKAssVest_Olive"
    };
    ref TStringArray Belts = {"CivilianBelt", "MilitaryBelt"};
    ref TStringArray Knives = {"CombatKnife", "HuntingKnife", "KitchenKnife", "SteakKnife"};
    autoptr array<ref RangedWeapon> PrimaryWeapons = new array<ref RangedWeapon>();
    autoptr array<ref RangedWeapon> SecondaryWeapons = new array<ref RangedWeapon>();

    void CustomMission()
    {
        PrimaryWeapons.Insert(new MP5());
        PrimaryWeapons.Insert(new UMP());
        PrimaryWeapons.Insert(new M4A1());
        PrimaryWeapons.Insert(new Shotgun());
        PrimaryWeapons.Insert(new Vaiga());

        SecondaryWeapons.Insert(new Winchester());
        SecondaryWeapons.Insert(new Blaze());
        SecondaryWeapons.Insert(new Mosin());
        SecondaryWeapons.Insert(new SKS());
    }

    override PlayerBase CreateCharacter(PlayerIdentity identity, vector pos, ParamsReadContext ctx, string characterName)
    {
        Entity playerEnt;
        playerEnt = GetGame().CreatePlayer(identity, characterName, pos, 0, "NONE");//Creates random player
        Class.CastTo(m_player, playerEnt);

        GetGame().SelectPlayer(identity, m_player);

        return m_player;
    }

    EntityAI EquipPlayerClothes(PlayerBase player)
    {
        HumanInventory inventory = player.GetHumanInventory();
        inventory.CreateInInventory(Tops.GetRandomElement());
        inventory.CreateInInventory(Bottoms.GetRandomElement());
        inventory.CreateInInventory(Shoes.GetRandomElement());
        inventory.CreateInInventory(Vests.GetRandomElement());
        EntityAI belt = inventory.CreateInInventory(Belts.GetRandomElement());
        return belt.GetInventory().CreateAttachment("NylonKnifeSheath");
    }

    void EquipPlayerForSurvival(PlayerBase player)
    {
        HumanInventory inventory = player.GetHumanInventory();
        EntityAI bandage = inventory.CreateInInventory("BandageDressing");
        inventory.CreateInInventory("TacticalBaconCan_Opened");
        inventory.CreateInInventory("WaterBottle");

        player.SetQuickBarEntityShortcut(bandage, 3);
    }

    EntityAI EquipPrimaryWeaponInHands(HumanInventory inventory)
    {
        return PrimaryWeapons.GetRandomElement().CreateInHands(inventory);
    }

    EntityAI EquipSecondaryWeapon(HumanInventory inventory)
    {
        return SecondaryWeapons.GetRandomElement().CreateInInventory(inventory);
    }

    EntityAI EquipKnifeInSheath(EntityAI sheath)
    {
        return sheath.GetInventory().CreateAttachment(Knives.GetRandomElement());
    }

    void EquipPlayerWeapons(PlayerBase player, EntityAI sheath)
    {
        HumanInventory inventory = player.GetHumanInventory();
        EntityAI primary = EquipPrimaryWeaponInHands(inventory);
        EntityAI secondary = EquipSecondaryWeapon(inventory);
        EntityAI melee = EquipKnifeInSheath(sheath);

        player.SetQuickBarEntityShortcut(primary, 0);
        player.SetQuickBarEntityShortcut(secondary, 1);
        player.SetQuickBarEntityShortcut(melee, 2);
    }

    void StartFedAndWatered(PlayerBase player)
    {
        player.GetStatWater().Set(player.GetStatWater().GetMax());
        player.GetStatEnergy().Set(player.GetStatEnergy().GetMax());
    }

    override void StartingEquipSetup(PlayerBase player, bool clothesChosen)
    {
        player.RemoveAllItems();

        EntityAI sheath = EquipPlayerClothes(player);
        EquipPlayerForSurvival(player);
        EquipPlayerWeapons(player, sheath);
        StartFedAndWatered(player);
    }

    override void HandleBody(PlayerBase player)
    {
        // Kill character so that players start fresh every time they connect
        player.SetHealth("", "", 0.0);
    }
};

Mission CreateCustomMission(string path)
{
    return new CustomMission();
}
