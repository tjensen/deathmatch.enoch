class Crates
{
    static void SpawnCrates()
    {
        CGame game = GetGame();

        SpawnWeaponCrate(game);
    }

    static private void SpawnWeaponCrate(CGame game)
    {
        vector position = "7332.09 295.65 2667.92";
        ItemBase chest = ItemBase.Cast(
            game.CreateObject("SeaChest", position, false, false, false));
        chest.SetPosition(position);

        GameInventory chestInventory = chest.GetInventory();
        chestInventory.CreateInInventory("VSS");

        Magazine magazine = Magazine.Cast(chestInventory.CreateInInventory("Mag_VSS_10Rnd"));
        magazine.ServerSetAmmoCount(0);

        chestInventory.CreateInInventory("AmmoBox_9x39AP_20Rnd");
        chestInventory.CreateInInventory("AmmoBox_9x39AP_20Rnd");
    }
}
