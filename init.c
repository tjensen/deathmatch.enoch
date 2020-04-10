#include "$CurrentDir:\\mpmissions\\deathmatch.enoch\\clothes.c"
#include "$CurrentDir:\\mpmissions\\deathmatch.enoch\\crates.c"
#include "$CurrentDir:\\mpmissions\\deathmatch.enoch\\weapons.c"


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

    Crates.SpawnCrates();
}

class CustomMission: MissionServer
{
    autoptr Clothes clothes = new Clothes();
    autoptr Weapons weapons = new Weapons();

    override PlayerBase CreateCharacter(PlayerIdentity identity, vector pos, ParamsReadContext ctx, string characterName)
    {
        Entity playerEnt;
        playerEnt = GetGame().CreatePlayer(identity, characterName, pos, 0, "NONE");//Creates random player
        Class.CastTo(m_player, playerEnt);

        GetGame().SelectPlayer(identity, m_player);

        return m_player;
    }

    void EquipPlayerForSurvival(PlayerBase player)
    {
        HumanInventory inventory = player.GetHumanInventory();
        EntityAI bandage = inventory.CreateInInventory("BandageDressing");
        inventory.CreateInInventory("TacticalBaconCan_Opened");
        inventory.CreateInInventory("WaterBottle");

        player.SetQuickBarEntityShortcut(bandage, 3);
    }

    void StartFedAndWatered(PlayerBase player)
    {
        player.GetStatWater().Set(player.GetStatWater().GetMax());
        player.GetStatEnergy().Set(player.GetStatEnergy().GetMax());
    }

    override void StartingEquipSetup(PlayerBase player, bool clothesChosen)
    {
        player.RemoveAllItems();

        EntityAI sheath = clothes.EquipPlayerClothes(player);
        EquipPlayerForSurvival(player);
        weapons.EquipPlayerWeapons(player, sheath);
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
