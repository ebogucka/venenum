#include "Save.h"


Save::Save(GameContext& context) : context(context) {}


void Save::store()
{
    context.map->save();

    boost::property_tree::ptree savefile;
    savefile.put("game.floor", context.floor);

    context.player->save(savefile);

    savefile.put("game.monsters", "");
    for(std::unordered_map<int, Creature*>::iterator it = context.monsters.begin(); it != context.monsters.end(); ++it)
    {
        it->second->saveState(savefile);
    }
    savefile.put("game.items", "");
    for(std::unordered_map<int, Item*>::iterator it = context.items.begin(); it != context.items.end(); ++it)
    {
        it->second->saveState(savefile);
    }
    boost::property_tree::xml_writer_settings<char> settings('\t', 1);
    boost::property_tree::write_xml("../data/save.dat", savefile, std::locale(), settings);
}


void Save::restore()
{
    using namespace boost::property_tree;
    context.map->load();
    ptree savefile;
    read_xml("../data/save.dat", savefile);

    context.floor = savefile.get<int>("game.floor");
    context.player->load(savefile);

    BOOST_FOREACH(ptree::value_type &value, savefile.get_child("game.monsters"))
    {
        Creature::loadState(value.second, context);  
    }
    BOOST_FOREACH(ptree::value_type &value, savefile.get_child("game.items"))
    {
        Item::loadState(value.second, context);  
    }
}
