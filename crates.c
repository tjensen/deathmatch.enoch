class Crates
{
    static private const vector WEAPON_CRATE_POS = "7332.09 295.65 2667.92";
    static private const vector ARMOR_CRATE_POS = "7304.11 300.059 2850.14";
    static private const vector TRAP_CRATE_POS = "7373.78 296.85 2570.04";

    static private ref TStringArray WEAPONS = {"VSS", "FAL", "SVD"};

    static private ref TStringArray FAL_BUTTSTOCKS = {
        "Fal_FoldingBttstck", "Fal_OeBttstck"
    };

    static private ref TStringArray VESTS = {
        "PlateCarrierVest", "PressVest_Blue", "PressVest_LightBlue",
        "PoliceVest"
    };
    static private ref TStringArray HELMETS = {
        "BallisticHelmet_Black", "BallisticHelmet_Green", "BallisticHelmet_UN",
        "GorkaHelmet", "Mich2001Helmet", "Ssh68Helmet", "ZSh3PilotHelmet"
    };
    static private ref TStringArray GLOVES = {
        "TacticalGloves_Beige", "TacticalGloves_Black", "TacticalGloves_Green",
        "SurgicalGloves_Blue", "SurgicalGloves_Green",
        "SurgicalGloves_LightBlue", "SurgicalGloves_White",
        "NBCGlovesGray", "OMNOGloves_Brown", "OMNOGloves_Gray",
        "WorkingGloves_Beige", "WorkingGloves_Black", "WorkingGloves_Brown",
        "WorkingGloves_Yellow"
    };

    static private ref TStringArray TRAPS = {"LandMineTrap", "BearTrap"};
    static private ref TStringArray GRENADES = {
        "FlashGrenade", "M67Grenade", "RGD5Grenade", "M18SmokeGrenade_Purple",
        "M18SmokeGrenade_Green", "M18SmokeGrenade_Red",
        "M18SmokeGrenade_White", "M18SmokeGrenade_Yellow",
        "RDG2SmokeGrenade_Black", "RDG2SmokeGrenade_White"
    };

    static void SpawnCrates(CGame game)
    {
        Crates.SpawnWeaponCrate(game, WEAPONS.GetRandomElement());
        Crates.SpawnArmorCrate(game);
        Crates.SpawnTrapCrate(game);
    }

    static private GameInventory SpawnCrate(CGame game, vector position)
    {
        Print("Spawning crate at " + position);
        ItemBase chest = ItemBase.Cast(
            game.CreateObject("SeaChest", position, false, false, false));
        // CreateObject doesn't always position the object correctly
        chest.SetPosition(position);

        return chest.GetInventory();
    }

    static private void SpawnEmptyMagazine(
            GameInventory inventory, string name)
    {
        Magazine magazine = Magazine.Cast(inventory.CreateInInventory(name));
        magazine.ServerSetAmmoCount(0);
    }

    static private void SpawnVSS(GameInventory inventory)
    {
        inventory.CreateInInventory("VSS");

        inventory.CreateInInventory("PSO1Optic");
        inventory.CreateInInventory("Battery9V");

        Crates.SpawnEmptyMagazine(inventory, "Mag_VSS_10Rnd");
        Crates.SpawnEmptyMagazine(inventory, "Mag_VSS_10Rnd");
        Crates.SpawnEmptyMagazine(inventory, "Mag_VSS_10Rnd");

        // Despite its name, the 20-round box actually contains 10 rounds
        inventory.CreateInInventory("AmmoBox_9x39AP_20Rnd");
        inventory.CreateInInventory("AmmoBox_9x39AP_20Rnd");
        inventory.CreateInInventory("AmmoBox_9x39AP_20Rnd");
        inventory.CreateInInventory("AmmoBox_9x39AP_20Rnd");
        inventory.CreateInInventory("AmmoBox_9x39AP_20Rnd");
        inventory.CreateInInventory("AmmoBox_9x39AP_20Rnd");
    }

    static private void SpawnFAL(GameInventory inventory)
    {
        inventory.CreateInInventory("FAL");

        inventory.CreateInInventory(FAL_BUTTSTOCKS.GetRandomElement());

        inventory.CreateInInventory("ACOGOptic");

        // The FAL currently only accepts an improved suppressor, which doesn't
        // last very long, so offer a few of them.
        inventory.CreateInInventory("ImprovisedSuppressor");
        inventory.CreateInInventory("ImprovisedSuppressor");
        inventory.CreateInInventory("ImprovisedSuppressor");
        inventory.CreateInInventory("ImprovisedSuppressor");
        inventory.CreateInInventory("ImprovisedSuppressor");

        Crates.SpawnEmptyMagazine(inventory, "Mag_FAL_20Rnd");
        Crates.SpawnEmptyMagazine(inventory, "Mag_FAL_20Rnd");
        Crates.SpawnEmptyMagazine(inventory, "Mag_FAL_20Rnd");

        inventory.CreateInInventory("AmmoBox_308Win_20Rnd");
        inventory.CreateInInventory("AmmoBox_308Win_20Rnd");
        inventory.CreateInInventory("AmmoBox_308Win_20Rnd");
        inventory.CreateInInventory("AmmoBox_308Win_20Rnd");
        inventory.CreateInInventory("AmmoBox_308Win_20Rnd");
        inventory.CreateInInventory("AmmoBox_308Win_20Rnd");
    }

    static private void SpawnSVD(GameInventory inventory)
    {
        inventory.CreateInInventory("SVD");

        inventory.CreateInInventory("PSO1Optic");
        inventory.CreateInInventory("Battery9V");

        inventory.CreateInInventory("AK_Suppressor");

        Crates.SpawnEmptyMagazine(inventory, "Mag_SVD_10Rnd");
        Crates.SpawnEmptyMagazine(inventory, "Mag_SVD_10Rnd");
        Crates.SpawnEmptyMagazine(inventory, "Mag_SVD_10Rnd");

        inventory.CreateInInventory("AmmoBox_762x54_20Rnd");
        inventory.CreateInInventory("AmmoBox_762x54_20Rnd");
        inventory.CreateInInventory("AmmoBox_762x54_20Rnd");
    }

    static private void SpawnWeaponCrate(CGame game, string name)
    {
        GameInventory inventory = Crates.SpawnCrate(game, WEAPON_CRATE_POS);

        if (name == "VSS")
        {
            SpawnVSS(inventory);
        }
        else if (name == "FAL")
        {
            SpawnFAL(inventory);
        }
        else if (name == "SVD")
        {
            SpawnSVD(inventory);
        }
    }

    static private void SpawnArmorCrate(CGame game)
    {
        GameInventory inventory = Crates.SpawnCrate(game, ARMOR_CRATE_POS);

        inventory.CreateInInventory(VESTS.GetRandomElement());

        inventory.CreateInInventory(HELMETS.GetRandomElement());

        inventory.CreateInInventory(GLOVES.GetRandomElement());
    }

    static private void SpawnTrapCrate(CGame game)
    {
        GameInventory inventory = Crates.SpawnCrate(game, TRAP_CRATE_POS);

        inventory.CreateInInventory(TRAPS.GetRandomElement());
        inventory.CreateInInventory(TRAPS.GetRandomElement());

        inventory.CreateInInventory(GRENADES.GetRandomElement());
        inventory.CreateInInventory(GRENADES.GetRandomElement());
        inventory.CreateInInventory(GRENADES.GetRandomElement());
    }
}

// vim:ft=cs
