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

    autoptr TStringStringMap m_Identities = new TStringStringMap();

    autoptr Clothes clothes = new Clothes();
    autoptr Weapons weapons = new Weapons();

    int m_max_rounds = 0;
    int m_num_rounds = 0;

    int m_round_duration;
    bool m_round_ending = false;

    autoptr map<PlayerIdentity, int> m_player_kills = new map<PlayerIdentity, int>();
    autoptr map<PlayerIdentity, int> m_player_deaths = new map<PlayerIdentity, int>();

    void CustomMission()
    {
        CGame game = GetGame();

        m_max_rounds = game.ServerConfigGetInt("maxRounds");
        Print("Max Rounds: " + m_max_rounds);

        m_round_duration = game.ServerConfigGetInt("deathmatchRoundMinutes");
        if (m_round_duration < 1) m_round_duration = DEFAULT_ROUND_DURATION;
        Print("Round duration: " + m_round_duration);

        game.GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(this.CheckPlayerPositions, 10000, true);

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

    private void NotifyPlayer(PlayerIdentity identity, string message, string details = "")
    {
        string now = GetGame().GetTime().ToString();
        string name = "ALL";
        if (identity)
        {
            name = identity.GetName();
        }

        // This should use chat messaging but, because chat is broken in v1.07, we're using
        // NotificationSystem, instead.
        NotificationSystem.SendNotificationToPlayerIdentityExtended(
                identity, 5.0, message, details);

        Print(now + " | NOTIFY | " + name + " (" + identity + ") | " + message + " | " + details);
    }

    private void NotifyAllPlayers(string message, string details = "")
    {
        this.NotifyPlayer(null, message, details);
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
            this.NotifyAllPlayers("Round ends in " + timeLeft + " seconds");
            GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(
                    this.EndRoundCountdown, 5000, false, duration - 5000);
        }
    }

    private void StartRound()
    {
        m_num_rounds++;

        Print("Starting round " + m_num_rounds);

        Print("Players:");
        for (int i = 0; i < m_Identities.Count(); i++)
        {
            Print("  | " + i + " | " + m_Identities.GetKey(i) + " | " + m_Identities.GetElement(i) + " |");
        }

        m_round_ending = false;

        m_player_kills.Clear();
        m_player_deaths.Clear();

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

    private string ReportPlayerStats()
    {
        array<PlayerIdentity> identities = new array<PlayerIdentity>();
        GetGame().GetPlayerIndentities(identities);
        string bestPlayer;
        int bestKills = -1;
        int bestDeaths = -1;

        for (int i = 0; i < identities.Count(); i++)
        {
            PlayerIdentity identity = identities.Get(i);
            int kills = m_player_kills.Get(identity);
            int deaths = m_player_deaths.Get(identity);
            this.NotifyPlayer(identity, "Your K:D was " + kills + ":" + deaths);

            if (kills > bestKills || ((kills == bestKills) && (deaths < bestDeaths)))
            {
                bestPlayer = identity.GetName();
                bestKills = kills;
                bestDeaths = deaths;
            }
        }

        if (bestPlayer != "")
        {
            return ("The top K:D was " + bestKills + ":" + bestDeaths + " by " + bestPlayer);
        }

        return "";
    }

    private void EndRound()
    {
        Print("Ending round");

        this.m_round_ending = true;

        string bestInfo = this.ReportPlayerStats();

        this.NotifyAllPlayers("The round has ended!", bestInfo);

        if (m_max_rounds > 0 && m_num_rounds >= m_max_rounds)
        {
            Print("Max rounds reached -- requesting restart");
            GetGame().RequestRestart(1);  // non-0 code might might encourage GSP to restart?
        }
        else
        {
            this.KillAllPlayers();

            ScriptCallQueue queue = GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY);
            queue.CallLater(this.CleanupObjects, 200, false);
        }

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

    override void InvokeOnConnect(PlayerBase player, PlayerIdentity identity)
    {
        Print("InvokeOnConnect :: " + player + " :: " + identity);

        string uid = identity.GetId();

        // Unfortunately, InvokeOnConnect gets called when players respawn, so we have to keep
        // track of connects and disconnects in order to know if this is being called for a
        // player's initial spawn.
        if (!m_Identities.Contains(uid))
        {
            string name = identity.GetName();

            m_Identities.Set(uid, name);

            this.NotifyAllPlayers(name + " has entered the arena");
        }

        Print("m_Identities.Count() == " + m_Identities.Count());

        super.InvokeOnConnect(player, identity);
    }

    override void PlayerDisconnected(PlayerBase player, PlayerIdentity identity, string uid)
    {
        Print("PlayerDisconnected :: " + player + " :: " + identity + " :: " + uid);

        string name;
        if (m_Identities.Find(uid, name))
        {
            m_Identities.Remove(uid);

            this.NotifyAllPlayers(name + " has left the arena");
        }

        Print("m_Identities.Count() == " + m_Identities.Count());

        super.PlayerDisconnected(player, identity, uid);
    }

    override void HandleBody(PlayerBase player)
    {
        player.DropAllItems();
        // Kill character so that players start fresh every time they connect
        player.SetHealth("", "", 0.0);
    }

    void DeglitchItemInHands(PlayerBase player, int remainingTries)
    {
        Print("DeglitchItemInHands | " + player + " | " + remainingTries);

        ItemBase item = player.GetItemInHands();
        if (item)
        {
            player.LocalReplaceItemInHandsWithNewElsewhere(
                    new DestroyItemInCorpsesHandsAndCreateNewOnGndLambda(
                        item, item.GetType(), player, false));

            remainingTries--;
            if (remainingTries > 0)
            {
                GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(
                        this.DeglitchItemInHands, 500, false, player, remainingTries);
            }
        }
    }

    void OnPlayerDeath(Man player)
    {
        // Drop item in hands to prevent it from getting glitched -- based on work-around documented
        // at https://feedback.bistudio.com/T145046
        PlayerBase playerBase = PlayerBase.Cast(player);
        if (playerBase)
        {
            GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(
                    this.DeglitchItemInHands, 500, false, playerBase, 3);
        }

        PlayerIdentity identity = player.GetIdentity();
        if (identity)
        {
            m_player_deaths.Set(identity, m_player_deaths.Get(identity) + 1);

            string name = identity.GetName();

            KillerData data = player.m_KillerData;
            if (data)
            {
                Man killerMan = Man.Cast(data.m_Killer);
                PlayerIdentity killerIdentity;
                if (killerMan) killerIdentity = killerMan.GetIdentity();
                if (killerIdentity)
                {
                    m_player_kills.Set(killerIdentity, m_player_kills.Get(killerIdentity) + 1);

                    string killer = killerIdentity.GetName();
                    this.NotifyAllPlayers(killer + " killed " + name);
                }
                else
                {
                    this.NotifyAllPlayers(name + " was killed");
                }
            }
            else
            {
                this.NotifyAllPlayers(name + " has died");
            }
        }
    }

    override bool InsertCorpse(Man player)
    {
        Print("InsertCorpse :: " + player);

        if (!m_round_ending)
        {
            this.OnPlayerDeath(player);
        }

        return super.InsertCorpse(player);
    }
};

Mission CreateCustomMission(string path)
{
    return new CustomMission();
}

// vim:ft=cs
