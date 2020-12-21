# DayZ Livonia Deathmatch

This is a DayZ mission for running a deathmatch server on the Livonia DLC map.
No client-side mods are required to play.

* [Installation Instructions](#installation-instructions)
* [Recommended Server Settings](#recommended-server-settings)
* [Optional Server Settings](#optional-server-settings)
  * [Round Duration](#round-duration)
  * [Maximum Rounds](#maximum-rounds)
  * [Infected Spawning](#infected-spawning)
  * [Cowboy Rounds](#cowboy-rounds)
  * [Christmas](#christmas)
  * [Kill Feed Webhook](#kill-feed-webhook)
* [Design Requirements](#design-requirements)

## Installation Instructions

Install the `deathmatch.enoch` folder (the contents of this Git repository) in
your `DayZServer\mpmissions` folder. If DayZ server was installed using the
Steam Windows application, the `mpmissions` folder is typically located at:

```
C:\Program Files (x86)\Steam\steamapps\common\DayZServer\mpmissions
```

Edit the DayZ server configuration file (typically `serverDZ.cfg`) to load the
`deathmatch.enoch` mission:

```
class Missions
{
    class DayZ
    {
        template="deathmatch.enoch";
    };
};
```

## Recommended Server Settings

Players do not spawn with light sources or night vision, so setting the server
time is recommended to avoid frustrating players with darkness. For example:

```
serverTime="2020/4/1/08/00";
serverTimeAcceleration=0;
serverTimePersistent=0;
```

Player characters are randomized, by design, so disabling the respawn dialog
for selecting a custom character is also recommended to avoid confusing
players:

```
disableRespawnDialog=1;
```

## Optional Server Settings

**Important: Optional server settings are no longer read from the server
config file (e.g. `serverDZ.cfg`)**

Optional deathmatch settings can be configured by creating a JSON file, named
`deathmatch-settings.json`, in your server's profile directory.

The following is an example file with all optional settings specified:

```json
{
  "roundMinutes": 45,
  "maxRounds": 4,
  "infectedChance": 50,
  "forceInfectedPlayerLimit": 2,
  "infectedPlayerFactor": 10,
  "minimumInfected": 10,
  "maximumInfected": 80,
  "cowboyRoundChance": 33,
  "christmas": 1,
  "killFeedWebhook": {
    "type": "discord",
    "url": "https://discordapp.com/api/webhooks/XXXXXXXX/YYYYYYYYYYYYYYYY"
  }
}
```

Note that it is only necessary to specify the settings whose defaults you want
to override.

### Round Duration

The duration, in minutes, of each round can be changed by including a
`roundMinutes` setting in the `deathmatch-settings.json` file. For example,
the following changes the duration to 45 minutes:

```json
{
  "roundMinutes": 45
}
```

The default is 30 minutes.

### Maximum Rounds

The server can be configured to automatically terminate after a desired number
of rounds using the `maxRounds` setting. By default, the server will never
automatically terminate. To stop after every 4 rounds:

```json
{
  "maxRounds": 4
}
```

This setting is intended to be used in conjunction with a script that
automatically restarts the server after it exits. Game Server Providers usually
handle this for you but self-hosting may require a batch file or PowerShell
script.

### Infected Spawning

Infected can be configured to spawn as a percentage chance at the beginning of
each round using the `infectedChance` setting. By default, infected have a 0%
chance of spawning. To give infected a 50% chance of spawning at the beginning
of each round:

```json
{
  "infectedChance": 50
}
```

Servers can be configured to override the `infectedChance` setting to force
infected to spawn when the number of players on the server is below the
`forceInfectedPlayerLimit` setting. By default, the `forceInfectedPlayerLimit`
is 0, meaning `infectedChance` controls whether infected spawn, regardless of
how many players are on the server. To force the server to spawn infected
whenever the server has less than 2 players:

```json
{
  "forceInfectedPlayerLimit": 2
}
```

The number of infected spawned per player can also be customized using the
`infectedPlayerFactor` setting. By default, 5 infected are spawned for each
player. For example, to spawn 10 infected per player:

```json
{
  "infectedPlayerFactor": 10
}
```

The minimum and maximum number of infected to spawn is also configurable. The
defaults are 25 and 50, respectively. For example:

```json
{
  "minimumInfected": 10,
  "maximumInfected": 80
}
```

Note that the minimum and maximum infected counts will override the infected
player factor. For example, if `maximumInfected` is 10, `infectedPlayerFactor`
is 5, and there are 20 players on the server, no more than 10 infected will
spawn.

Also note that if `infectedChance` is 0, which is the default value, then
infected will never spawn, regardless of how the other settings are configured.

### Cowboy Rounds

Cowboy Rounds are special game rounds where all players spawn with revolvers,
repeaters, and cowboy attire. By default, cowboy rounds are disabled but can be
enabled through a percent chance using the `cowboyRoundChance` setting. For
example, to give cowboy rounds a 33% chance of happening:

```json
{
  "cowboyRoundChance": 33
}
```

### Christmas

Enabling the Christmas event causes a large Christmas tree to spawn near the
center of the arena, surrounded by 4 gift boxes containing festive loot. It
also causes players to spawn with Santa hats, Santa beards, and clothing with
holiday colors. Infected will also spawn with Santa hats.

Setting `christmas` to `1` will enable the Christmas event. The default is `0`,
which disables the event.

```json
{
  "christmas": 1
}
```

### Kill Feed Webhook

It is possible to configure the server to send kill feed messages to a Discord
channel using a webhook. Use your Discord server's settings to create a webhook
URL for the channel you want kill feed messages to go to.

A typical webhook configuration will look something like this:

```json
{
  "killFeedWebhook": {
    "type": "discord",
    "url": "https://discordapp.com/api/webhooks/XXXXXXXX/YYYYYYYYYYYYYYYY"
  }
}
```

Note that you **must** set the `type` to `discord` in order for the webhook
to work. Other webhook types may be added in the future.

## Design Requirements

* Players do not need to install any mods
* Livonia DLC is required
* Players spawn with full energy, hydration, blood, and health
* Players spawn with a random selection of weapons and clothing
* All players are equal; nobody gets extra privileges or special treatment
* Three loot crates spawn at ~~static~~ random locations at the start of the game, containing:
  1. Highest-tier weapons
  2. Armor and gloves
  3. Grenades and traps
* No random loot spawns in the world
* ~~No~~ Optional infected spawning
  * Configurable in the server settings
* Dropped items and dead bodies despawn very quickly
