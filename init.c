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
}

class CustomMission: MissionServer
{
    ref array<string> Tops = {
        "BDUJacket", "Blouse_Green", "BomberJacket_Black", "BomberJacket_Brown",
        "BomberJacket_Olive", "DenimJacket", "HikingJacket_Black", "HikingJacket_Green",
        "Hoodie_Black", "Hoodie_Brown", "Hoodie_Green", "HuntingJacket_Brown",
        "HuntingJacket_Summer", "LabCoat", "M65Jacket_Black", "M65Jacket_Olive",
        "ParamedicJacket_Green", "PrisonUniformJacket", "QuiltedJacket_Black",
        "QuiltedJacket_Green", "TacticalShirt_Black", "TacticalShirt_Olive", "WoolCoat_Black"
    };
    ref array<string> Bottoms = {
        "BDUPants", "CargoPants_Black", "CargoPants_Green", "HunterPants_Brown",
        "HunterPants_Summer", "Jeans_Black", "Jeans_Brown", "Jeans_Green", "ParamedicPants_Green",
        "PrisonUniformPants", "ShortJeans_Black", "ShortJeans_Brown", "ShortJeans_Green"
    };
    ref array<string> Shoes = {
        "AthleticShoes_Black", "AthleticShoes_Brown", "AthleticShoes_Green", "CombatBoots_Beige",
        "CombatBoots_Black", "CombatBoots_Brown", "CombatBoots_Green", "CombatBoots_Grey",
        "HikingBoots_Black", "HikingBoots_Brown", "JungleBoots_Black", "JungleBoots_Brown",
        "JungleBoots_Green", "JungleBoots_Olive", "MilitaryBoots_Black", "MilitaryBoots_Brown",
        "Sneakers_Black", "Sneakers_Green", "Wellies_Black", "Wellies_Brown", "Wellies_Green",
        "WorkingBoots_Brown", "WorkingBoots_Green"
    };
    ref array<string> Vests = {
        "PressVest_Blue", "PressVest_LightBlue", "UKAssVest_Black", "UKAssVest_Camo",
        "UKAssVest_Khaki", "UKAssVest_Olive"
    };
    ref array<string> Belts = {"CivilianBelt", "MilitaryBelt"};
    ref array<string> MP5Handguards = {"MP5_PlasticHndgrd", "MP5_RailHndgrd"};
    ref array<string> M4Handguards = {"M4_MPHndgrd", "M4_PlasticHndgrd", "M4_RISHndgrd"};
    ref array<string> M4Buttstocks = {"M4_CQBBttstck", "M4_MPBttstck", "M4_OEBttstck"};
    ref array<string> Knives = {"CombatKnife", "HuntingKnife", "KitchenKnife", "SteakKnife"};

    override PlayerBase CreateCharacter(PlayerIdentity identity, vector pos, ParamsReadContext ctx, string characterName)
    {
        Entity playerEnt;
        playerEnt = GetGame().CreatePlayer(identity, characterName, pos, 0, "NONE");//Creates random player
        Class.CastTo(m_player, playerEnt);

        GetGame().SelectPlayer(identity, m_player);

        return m_player;
    }

    void EquipPlayerClothes(PlayerBase player)
    {
        HumanInventory inventory = player.GetHumanInventory();
        inventory.CreateInInventory(Tops.GetRandomElement());
        inventory.CreateInInventory(Bottoms.GetRandomElement());
        inventory.CreateInInventory(Shoes.GetRandomElement());
        inventory.CreateInInventory(Vests.GetRandomElement());
        inventory.CreateInInventory(Belts.GetRandomElement());
        inventory.CreateInInventory("NylonKnifeSheath");
        // TODO: Put knife in sheath and put sheath in belt
    }

    void EquipPlayerForSurvival(PlayerBase player)
    {
        HumanInventory inventory = player.GetHumanInventory();
        EntityAI bandage = inventory.CreateInInventory("BandageDressing");
        inventory.CreateInInventory("TacticalBaconCan_Opened");
        inventory.CreateInInventory("WaterBottle");

        player.SetQuickBarEntityShortcut(bandage, 3);
    }

    EntityAI EquipMP5InHands(HumanInventory inventory)
    {
        EntityAI mp5 = inventory.CreateInHands("MP5K");
        GameInventory mp5Inv = mp5.GetInventory();
        mp5Inv.CreateAttachment("ACOGOptic");
        mp5Inv.CreateAttachment(MP5Handguards.GetRandomElement());
        mp5Inv.CreateAttachment("MP5k_StockBttstck");

        inventory.CreateInInventory("Mag_MP5_30Rnd");
        inventory.CreateInInventory("Mag_MP5_30Rnd");
        inventory.CreateInInventory("Mag_MP5_30Rnd");

        return mp5;
    }

    EntityAI EquipUMPInHands(HumanInventory inventory)
    {
        EntityAI ump = inventory.CreateInHands("UMP45");
        GameInventory umpInv = ump.GetInventory();
        umpInv.CreateAttachment("ACOGOptic");

        inventory.CreateInInventory("Mag_UMP_25Rnd");
        inventory.CreateInInventory("Mag_UMP_25Rnd");
        inventory.CreateInInventory("Mag_UMP_25Rnd");

        return ump;
    }

    EntityAI EquipM4A1InHands(HumanInventory inventory)
    {
        EntityAI m4a1 = inventory.CreateInHands("M4A1");
        GameInventory m4a1Inv = m4a1.GetInventory();
        m4a1Inv.CreateAttachment("ACOGOptic");
        m4a1Inv.CreateAttachment(M4Handguards.GetRandomElement());
        m4a1Inv.CreateAttachment(M4Buttstocks.GetRandomElement());

        inventory.CreateInInventory("Mag_STANAG_30Rnd");
        inventory.CreateInInventory("Mag_STANAG_30Rnd");
        inventory.CreateInInventory("Mag_STANAG_30Rnd");

        return m4a1;
    }

    EntityAI EquipPrimaryWeaponInHands(HumanInventory inventory)
    {
        EntityAI weapon;

        switch (Math.RandomInt(0, 3)) {
            case 0:
                weapon = EquipMP5InHands(inventory);
                break;
            case 1:
                weapon = EquipUMPInHands(inventory);
                break;
            case 2:
                weapon = EquipM4A1InHands(inventory);
                break;
        }

        return weapon;
    }

    EntityAI EquipRifle(HumanInventory inventory, string rifleName, string opticName, string ammoName)
    {
        EntityAI rifle = inventory.CreateInInventory(rifleName);
        rifle.GetInventory().CreateAttachment(opticName);

        inventory.CreateInInventory(ammoName);
        inventory.CreateInInventory(ammoName);
        inventory.CreateInInventory(ammoName);

        return rifle;
    }

    EntityAI EquipSecondaryWeapon(HumanInventory inventory)
    {
        EntityAI weapon;

        switch (Math.RandomInt(0, 4)) {
            case 0:
                weapon = EquipRifle(inventory, "Winchester70", "HuntingOptic", "Ammo_308Win");
                break;
            case 1:
                weapon = EquipRifle(inventory, "B95", "HuntingOptic", "Ammo_308Win");
                break;
            case 2:
                weapon = EquipRifle(inventory, "Mosin9130", "PUScopeOptic", "Ammo_762x54");
                break;
            case 3:
                weapon = EquipRifle(inventory, "SKS", "PUScopeOptic", "Ammo_762x39");
                break;
        }

        return weapon;
    }

    EntityAI EquipKnife(HumanInventory inventory)
    {
        return inventory.CreateInInventory(Knives.GetRandomElement());
    }

    void EquipPlayerWeapons(PlayerBase player)
    {
        HumanInventory inventory = player.GetHumanInventory();
        EntityAI primary = EquipPrimaryWeaponInHands(inventory);
        EntityAI secondary = EquipSecondaryWeapon(inventory);
        EntityAI melee = EquipKnife(inventory);

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

        EquipPlayerClothes(player);
        EquipPlayerForSurvival(player);
        EquipPlayerWeapons(player);
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
