class Webhook : Managed
{
    string type;
    string url;
}

class DeathmatchSettings : Managed
{
    ref Webhook killFeedWebhook = new Webhook();

    void load()
    {
        JsonFileLoader<ref DeathmatchSettings>.JsonLoadFile(
                "$profile:deathmatch-settings.json", this);
    }
}
