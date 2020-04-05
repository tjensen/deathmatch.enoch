# DayZ Livonia Deathmatch

This is a DayZ mission for running a deathmatch server on the Livonia DLC map.
No client-side mods are required to play.

## Installation Instructions

Install the `deathMatch.enoch` folder (the contents of this Git repository) in
your `DayZServer\mpmissions` folder. If DayZ server was installed using the
Steam Windows application, the `mpmissions` folder is typically located at:

```
C:\Program Files (x86)\Steam\steamapps\common\DayZServer\mpmissions
```

Edit the DayZ server configuration file (typically `serverDZ.cfg`) to load the `deathMatch.enoch` mission:

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
