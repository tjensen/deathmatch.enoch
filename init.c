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
    ref array<string> MP5Handguards = {"MP5_PlasticHndgrd", "MP5_RailHndgrd"};
    ref array<string> Knives = {"CombatKnife", "HuntingKnife", "KitchenKnife", "SteakKnife"};

    override PlayerBase CreateCharacter(PlayerIdentity identity, vector pos, ParamsReadContext ctx, string characterName)
    {
        Entity playerEnt;
        playerEnt = GetGame().CreatePlayer(identity, characterName, pos, 0, "NONE");//Creates random player
        Class.CastTo(m_player, playerEnt);

        GetGame().SelectPlayer(identity, m_player);

        return m_player;
    }

    void StartFedAndWatered(PlayerBase player)
    {
        player.GetStatWater().Set(player.GetStatWater().GetMax());
        player.GetStatEnergy().Set(player.GetStatEnergy().GetMax());
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

    EntityAI EquipWinchester(HumanInventory inventory)
    {
        EntityAI winchester = inventory.CreateInInventory("Winchester70");
        winchester.GetInventory().CreateAttachment("HuntingOptic");

        inventory.CreateInInventory("Ammo_308Win");
        inventory.CreateInInventory("Ammo_308Win");
        inventory.CreateInInventory("Ammo_308Win");

        return winchester;
    }

    EntityAI EquipKnife(HumanInventory inventory)
    {
        return inventory.CreateInInventory(Knives.GetRandomElement());
    }

    void EquipPlayerWeapons(PlayerBase player)
    {
        HumanInventory inventory = player.GetHumanInventory();
        EntityAI primary = EquipMP5InHands(inventory);
        EntityAI secondary = EquipWinchester(inventory);
        EntityAI melee = EquipKnife(inventory);

        player.SetQuickBarEntityShortcut(primary, 0);
        player.SetQuickBarEntityShortcut(secondary, 1);
        player.SetQuickBarEntityShortcut(melee, 2);
    }

    override void StartingEquipSetup(PlayerBase player, bool clothesChosen)
    {
        EntityAI itemTop;
        EntityAI itemEnt;
        ItemBase itemBs;

        itemTop = player.FindAttachmentBySlotName("Body");

        if (itemTop)
        {
            itemEnt = itemTop.GetInventory().CreateInInventory("Rag");
            if (Class.CastTo(itemBs, itemEnt))
                itemBs.SetQuantity(4);

            itemTop.GetInventory().CreateInInventory("TacticalBaconCan_Opened");
            itemTop.GetInventory().CreateInInventory("WaterBottle");
        }

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
