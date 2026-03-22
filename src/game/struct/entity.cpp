#include "game/struct/entity.hpp"
#include "game/signatures.hpp"

EntityComponent* Entity::AddComponent(EntityComponent* myComp)
{
    std::unique_ptr<EntityComponent> compPtr(myComp);
    return real::EntityAddComponent(this, std::move(compPtr));
}

bool Entity::RemoveComponentByName(std::string const& key)
{
    return real::EntityRemoveComponentByName(this, key);
}