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
    static private int DEFAULT_ROUND_DURATION = 30;
    static private int COUNTDOWN_DURATION_MS = 10000;

    static private const vector LIMBO_POSITON = "7270.39 293.398 2923.94";
    static private const vector PLAYAREA_CENTER = "7451 0 2732";
    static private const int CLEANUP_RADIUS = 600;
    static private const int KILL_RADIUS = 570;
    static private const int PLAYAREA_RADIUS = 500;

    autoptr Clothes clothes = new Clothes();
    autoptr Weapons weapons = new Weapons();

    int m_round_duration;

    void CustomMission()
    {
        m_round_duration = GetGame().ServerConfigGetInt(
                "deathmatchRoundMinutes");
        if (m_round_duration < 1) m_round_duration = DEFAULT_ROUND_DURATION;
        Print("Round duration: " + m_round_duration);

        GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(
                this.CheckPlayerPositions, 10000, true);

        this.StartRound();
    }

    private float DistanceFromCenter(vector pos)
    {
        vector adjusted = Vector(pos[0], 0, pos[2]);

        return vector.Distance(PLAYAREA_CENTER, adjusted);
    }

    private void CheckPlayerPositions()
    {
        array<Man> players = new array<Man>();
        GetGame().GetPlayers(players);
        foreach (Man player : players)
        {
            if (player.IsAlive())
            {
                float distance = this.DistanceFromCenter(player.GetPosition());

                if (distance > PLAYAREA_RADIUS)
                {
                    Print("Player " + player.GetIdentity().GetName() + " is too far away (" + distance + ")");
                    NotificationSystem.SendNotificationToPlayerExtended(
                            player, 5.0, "You are outside the zone!",
                            "You will continue to lose health until you return to the zone.");
                    if (distance > KILL_RADIUS)
                    {
                        player.SetHealth("", "", 0.0);
                    }
                    else
                    {
                        player.SetHealth("", "", player.GetHealth() - 33);
                    }
                }
            }
        }
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
            NotificationSystem.SendNotificationToPlayerIdentityExtended(
                    null, 1.0, "Round ends in " + timeLeft + " seconds");
            Print("Ending round in " + timeLeft + " " + GetGame().GetTime());
            GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(
                    this.EndRoundCountdown, 5000, false, duration - 5000);
        }
    }

    private void StartRound()
    {
        Print("Starting round");

        CGame game = GetGame();

        int delay = (m_round_duration * 60000) - COUNTDOWN_DURATION_MS;
        game.GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(
                this.EndRoundCountdown,
                delay,
                false,
                COUNTDOWN_DURATION_MS);

        Crates.SpawnCrates(game);

        Print("Done");
    }

    private void EndRound()
    {
        Print("Ending round");

        NotificationSystem.SendNotificationToPlayerIdentityExtended(
                null, 1.0, "The round has ended!");

        this.KillAllPlayers();

        ScriptCallQueue queue = GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY);
        queue.CallLater(this.CleanupObjects, 200, false);

        Print("Done");
    }

    private void RestartRound()
    {
        this.EndRound();
    }

    private void PutInLimbo(EntityAI entity)
    {
        entity.SetPosition(LIMBO_POSITON);
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
                this.PutInLimbo(playerBase);
            }
        }
    }

    private void CleanupObjects()
    {
        Print("Cleaning up objects");

        CGame game = GetGame();
        array<Object> objects = new array<Object>();
        array<CargoBase> cargos = new array<CargoBase>();
        Print("Finding objects");
        int start = game.GetTime();
        game.GetObjectsAtPosition(
                PLAYAREA_CENTER, CLEANUP_RADIUS, objects, cargos);
        int end = game.GetTime();
        int delta = end - start;
        Print("Done finding objects");
        Print(" Start: " + start);
        Print(" End: " + end);
        Print(" Delta: " + delta);

        Print(" Objects to check: " + objects.Count());

        foreach (Object obj : objects)
        {
            ItemBase itemBase = ItemBase.Cast(obj);
            if (itemBase != null && itemBase.GetHierarchyParent() == null)
            {
                Print("Cleaning up object " + itemBase);
                itemBase.Delete();
            }

            PlayerBase player = PlayerBase.Cast(obj);
            if (player != null)
            {
                if (player.GetIdentity() == null)
                {
                    Print("Cleaning up corpse " + player);
                    player.Delete();
                }
                else
                {
                    Print("Stripping corpse " + player);
                    player.ClearInventory();
                    player.RemoveAllItems();
                    this.PutInLimbo(player);
                }
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
        inventory.CreateInInventory("SalineBagIV");
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
