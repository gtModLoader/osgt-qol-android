#pragma once
#include "variantdb.hpp"

class App
{
    public:
        virtual ~App();

        Variant* GetVar(std::string varName) { return m_sharedDB.GetVar(varName); }
        uint8_t pad[433];
        bool m_bShowFPS; // 441
        uint8_t pad2[1994];
        Entity* m_pEntityRoot; // 2440
        uint8_t pad3[1104];
        VariantDB m_sharedDB; // 3552
};
// 2440 - entityroot