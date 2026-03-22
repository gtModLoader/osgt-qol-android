#pragma once
#include "variantdb.hpp"

class Entity;

class EntityComponent
{
  public:
    EntityComponent();
    virtual ~EntityComponent() {};

    void SetName(std::string name) { *m_name = name; }
    std::string GetName() { return *m_name; }

    virtual void OnAdd(Entity* pEnt) {};
    virtual void OnRemove() {};

    Entity* GetParent() { return m_parent; }
    VariantDB* GetShared() { return &m_sharedDB; }
    Variant* GetVar(std::string const& varName) { return m_sharedDB.GetVar(varName); }
    Variant* GetVarWithDefault(const std::string& varName, const Variant& var)
    {
        return m_sharedDB.GetVarWithDefault(varName, var);
    }

  private:
    void* trackable;
    void* smth_else;
    std::string* m_name;
    Entity* m_parent;
    VariantDB m_sharedDB;
};
static_assert(sizeof(EntityComponent) == 168, "EntityComponent class size mismatch.");