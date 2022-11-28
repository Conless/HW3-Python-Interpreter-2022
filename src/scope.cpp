#include "pyinter/scope.h"

void Scope::VarRegister(const std::string &var_name, antlrcpp::Any var_data) { var_table[var_name] = var_data; }

QueryResult Scope::VarQuery(const std::string &var_name) const {
    auto it = var_table.find(var_name);
    if (it == var_table.end())
        return QueryResult(false, 0);
    return QueryResult(true, it->second);
} 