#include "pyinter/scope.h"

void Scope::VarRegister(const std::string &var_name, antlrcpp::Any var_data) { data_table[var_name] = var_data; }

QueryResult Scope::VarQuery(const std::string &var_name) const {
    auto it = data_table.find(var_name);
    if (it == data_table.end())
        return QueryResult(false, 0);
    return QueryResult(true, it->second);
} 