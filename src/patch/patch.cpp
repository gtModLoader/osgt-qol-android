#include "patch.hpp"
#include "utils/utils.hpp"
#include <stdexcept>
#include <string>
#include <vector>

namespace patch
{

PatchManager& PatchManager::get()
{
    static PatchManager instance;
    return instance;
}

void PatchManager::applyPatchesFromFile(const std::string& fileName)
{
    // First apply all core patches.
    for (auto patch : patchMap)
    {
        if (patch.second.type == PatchType::Core)
            applyPatch(patch.first);
    }
    for (auto patch : patchMap)
    {
        if (patch.second.type == PatchType::User)
            applyPatch(patch.first);
    }
    // Deal with loading from a patches.txt later, need to do some SAF access to a more neutral folder.
    return;
}

void PatchManager::applyPatch(const std::string& name)
{
    if (patchMap.find(name) == patchMap.end())
        throw std::runtime_error("Patch " + name + " does not exist.");
    if (patchMap[name].applied)
        return;
    // Try to apply the patch.
    const char* type = patchMap[name].type == PatchType::Core ? "core" : "user";
    try
    {
        patchMap[name].instance->apply();
        patchMap[name].applied = true;
        LOG_DEBUG("Applied %s patch %s\n", type, name.c_str());
    }
    catch (const std::exception& e)
    {
        LOG_ERROR("Failed to apply patch %s: %s", name.c_str(), e.what());
        throw std::runtime_error("Failed to apply patch " + name + ": " + e.what());
    }
}

std::vector<std::string> PatchManager::getAppliedUserPatchList() const
{
    std::vector<std::string> patches;
    for (auto patch : patchMap)
    {
        if (patch.second.type == PatchType::User && patch.second.applied)
            patches.push_back(patch.first);
    }
    return patches;
}

bool PatchManager::isPatchApplied(const std::string& name)
{
    if (patchMap.find(name) == patchMap.end())
        return false;
    return patchMap[name].applied;
}

} // namespace patch