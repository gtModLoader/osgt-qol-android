#pragma once
#include "game/struct/vec.hpp"
#include <map>
#include <string>
#include <vector>

struct InventoryMenuInfo
{
    float m_baseSizePerTool;
    float m_scale;
    float m_rectPaddingX;
    float m_rectPaddingY;
    float m_paddingBetweenTools;
    float m_scaledSizeOfTool;
    float m_quickToolbgWidth;
    float m_quickToolbgHeight;
    CL_Vec2f m_vQuickToolPos;
    CL_Vec2f m_vQuickToolSize;
    float m_inventorySpacerY;
    CL_Vec2f m_inventorySize;
    CL_Vec2f m_inventoryScrollSize;
    float m_grabBarHeight;
    float m_totalHeight;
    float m_inventoryRectSpacerY;
    CL_Vec2f m_menuButtonSize;
    CL_Vec2f m_menuRectPos;
    CL_Vec2f m_menuButtonSpacer;
    CL_Vec2f m_menuRectSize;
    CL_Vec2f m_inventoryAreaSavePos;
    CL_Vec2f m_logHandleSavePos;
    float m_snapOffsetFromBorder;
    float m_consoleLogMaxY;
    float m_consoleLogMinY;
    float m_quickToolBarPaddingX;
    float m_quickToolBarPaddingY;
    float m_disconnectedLogHeight;
    float m_defaultLogConnectedLogHeight;
    float m_chooseWorldLogHeight;
    float m_logGameWidth;
    float m_arrowButtonSizeX;
    float m_JumpButtonSizeX;
    float m_finalInventoryY;
    float m_finalLogY;
    std::vector<std::string> m_DefaultTabKeys;
    std::map<std::string, CL_Vec2i> m_TabTexCoordMap;
    std::map<std::string, unsigned int> m_TabBorderColorMap;
    std::vector<std::string> m_ActiveTabKeys;
    std::string m_CurrentActiveTabKey;
    std::string m_CurrentSearchKey;
    bool m_CurrentActiveSearch;
    float m_CurrentPreserveTabHeight;
    float m_CurrentPreserveSearchHeight;
    float m_LogConsoleWasPosY;
    float m_ItemsParentWasPosY;
    float m_TabPreserveWidth;
    bool m_WasGoHigh;
};
