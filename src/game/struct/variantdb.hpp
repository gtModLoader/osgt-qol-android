#pragma once
#include "variant.hpp"
#include <unordered_map>

class FunctionObject
{
  public:
    boost::signals2::signal<void(VariantList*)> sig_function;
};

class VariantDB
{
  public:
    virtual ~VariantDB() { DeleteAll(); }
    void Print();
    FunctionObject* GetFunction(const std::string& key);
    FunctionObject* GetFunctionIfExists(const std::string& key);
    void CallFunctionIfExists(const std::string& key, VariantList* pVList);
    Variant* GetVarIfExists(const std::string& key);
    Variant* GetVarWithDefault(const std::string& key, const Variant& vDefault);
    Variant* GetVar(const std::string& key);
    void DeleteVar(const std::string& key);
    void DeleteAll();

  private:
    std::unordered_map<std::string, std::shared_ptr<Variant>> m_data;
    std::unordered_map<std::string, std::shared_ptr<FunctionObject>> m_functionData;
    std::unordered_map<std::string, std::shared_ptr<Variant>>::iterator m_nextItor;
};
