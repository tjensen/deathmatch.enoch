# DayZ Livonia Deathmatch

This is a DayZ mission for running a deathmatch server on the Livonia DLC map.
No client-side mods are required to play.

* [Installation Instructions](#installation-instructions)
* [Recommended Server Settings](#recommended-server-settings)
* [Optional Server Settings](#optional-server-settings)
  * [Round Duration](#round-duration)
  * [Maximum Rounds](#maximum-rounds)
  * [Infected Spawning](#infected-spawning)
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

## Optional Server Settings

### Round Duration

The duration, in minutes, of each round can be changed by including a
`deathmatchRoundMinutes` setting in the server configuartion file. For example,
the following changes the duration to 45 minutes:

```
deathmatchRoundMinutes=45;
```

The default is 30 minutes.

### Maximum Rounds

The server can be configured to automatically terminate after a desired number
of rounds using the `maxRounds` setting. By default, the server will never
automatically terminate. To stop after every 4 rounds:

```
maxRounds=4;
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

```
infectedChance=50;
```

The number of infected spawned per player can also be customized using the
`infectedPlayerFactor` setting. By default, 5 infected are spawned for each
player. For example:

```
infectedPlayerFactor=10;  // spawn 10 infected per player
```

The minimum and maximum number of infected to spawn is also configurable. The
defaults are 25 and 50, respectively. For example:

```
minimumInfected=10;
maximumInfected=80;
```

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
* ~~No infected~~
  * Configurable in the server settings
* Dropped items and dead bodies despawn very quickly
