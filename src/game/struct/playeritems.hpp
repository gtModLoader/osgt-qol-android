#pragma once

// Incomplete type
#include <list>
#include <stdint.h>

// Namings matched.
struct InventoryItem
{
    short m_itemID;
    uint8_t m_count;
    uint8_t m_flags;
};

// Namings matched.
class PlayerItems
{
  public:
    virtual ~PlayerItems();

    uint8_t m_version;
    short m_clothingCache[9];
    std::list<InventoryItem> m_items;
    short m_quickSlots[4];
    int m_backpackSize;
    std::list<InventoryItem> m_FilterItems;
    bool m_IsDirty;
    int m_DungeonItemCount;
};