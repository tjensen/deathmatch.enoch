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
}

class CustomMission extends MissionServer
{
    static private int ROUND_DURATION = (20 * 60 * 1000);
    static private int COUNTDOWN_DURATION = (10 * 1000);

    autoptr Clothes clothes = new Clothes();
    autoptr Weapons weapons = new Weapons();

    void CustomMission()
    {
        this.StartRound();
    }

    private void EndRoundCountdown(int duration)
    {
        if (duration <= 0)
        {
            this.RestartRound();
        }
        else
        {
            int timeLeft = duration / 1000;
            Print("Ending round in " + timeLeft + " " + GetGame().GetTime());
            GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(
                    this.EndRoundCountdown, 1000, false, duration - 1000);
        }
    }

    private void StartRound()
    {
        Print("Starting round");

        CGame game = GetGame();

        int delay = ROUND_DURATION - COUNTDOWN_DURATION;
        Print("Starting countdown in " + delay);
        game.GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(
                this.EndRoundCountdown,
                delay,
                false,
                COUNTDOWN_DURATION);

        Crates.SpawnCrates(game);

        Print("Done");
    }

    private void EndRound()
    {
        Print("Ending round");

        this.KillAllPlayers();

        ScriptCallQueue queue = GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY);
        queue.CallLater(this.CleanupObjects, 0, false);

        Print("Done");
    }

    private void RestartRound()
    {
        this.EndRound();
    }

    private void KillAllPlayers()
    {
        array<Man> men = new array<Man>();
        GetGame().GetPlayers(men);
        foreach (Man man : men)
        {
            Print("Checking man " + man);
            PlayerBase playerBase = PlayerBase.Cast(man);
            if (playerBase != null)
            {
                Print("Killing player " + playerBase);
                playerBase.ClearInventory();
                playerBase.RemoveAllItems();
                playerBase.SetHealth("", "", 0.0);
            }
        }
    }

    private void CleanupObjects()
    {
        Print("Cleaning up objects");
        vector pos = "8000 0 8000";
        array<Object> objects = new array<Object>();
        array<CargoBase> cargos = new array<CargoBase>();
        Print("Finding objects");
        int start = GetGame().GetTime();
        GetGame().GetObjectsAtPosition(pos, 11400, objects, cargos);
        int end = GetGame().GetTime();
        int delta = end - start;
        Print("Done finding objects");
        Print(" Start: " + start);
        Print(" End: " + end);
        Print(" Delta: " + delta);

        foreach (Object obj : objects)
        {
            ItemBase itemBase = ItemBase.Cast(obj);
            if (itemBase != null && itemBase.GetHierarchyParent() == null)
            {
                Print("Cleaning up object " + itemBase);
                itemBase.Delete();
            }

            Man man = Man.Cast(obj);
            if (man != null && man.GetIdentity() != null)
            {
                Print("Cleaning up corpse " + man);
                man.Delete();
            }
        }
        Print("Done cleaning up objects");

        this.StartRound();
    }

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
        this.EquipPlayerForSurvival(player);
        weapons.EquipPlayerWeapons(player, sheath);
        this.StartFedAndWatered(player);
    }

    override void HandleBody(PlayerBase player)
    {
        player.DropAllItems();
        // Kill character so that players start fresh every time they connect
        player.SetHealth("", "", 0.0);
    }
};

Mission CreateCustomMission(string path)
{
    return new CustomMission();
}

// vim:ft=cs
