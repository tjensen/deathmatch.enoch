# DayZ Livonia Deathmatch

This is a DayZ mission for running a deathmatch server on the Livonia DLC map.
No client-side mods are required to play.

* [Installation Instructions](#installation-instructions)
* [Recommended Server Settings](#recommended-server-settings)
* [Optional Server Settings](#optional-server-settings)
  * [Round Duration](#round-duration)
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

## Design Requirements

* Players do not need to install any mods
* Livonia DLC is required
* Players spawn with full energy, hydration, blood, and health
* Players spawn with a random selection of weapons and clothing
* All players are equal; nobody gets extra privileges or special treatment
* Three loot crates spawn at static locations at the start of the game, containing:
  1. Highest-tier weapons
  2. Armor and gloves
  3. Grenades and traps
* No random loot spawns in the world
* No infected
  * Perhaps this should be optional?
* Dropped items and dead bodies despawn very quickly
