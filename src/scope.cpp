#include "pyinter/scope.h"

Scope::Scope() {
    while (!data_table.empty())
        data_table.pop_back();
    data_table.push_back(std::unordered_map<std::string, antlrcpp::Any>());
}

void Scope::VarRegister(const std::string &var_name, antlrcpp::Any var_data, int type) {
    if (type || data_table.back().find(var_name) != data_table.back().end()) {
        data_table.back()[var_name] = var_data;
        return;
    }
    if (data_table.front().find(var_name) != data_table.front().end()) {
        data_table.front()[var_name] = var_data;
        return;
    }
    data_table.back()[var_name] = var_data;
}

QueryResult Scope::VarQuery(const std::string &var_name) const {
    auto it = data_table.back().find(var_name);
    if (it == data_table.back().end()) {
        it = data_table.front().find(var_name);
        if (it == data_table.front().end())
            return QueryResult(false, 0);
    }
    return QueryResult(true, it->second);
}

void Scope::push() { data_table.push_back(std::unordered_map<std::string, antlrcpp::Any>()); }
void Scope::pop() { if (!data_table.size())
        throw "??";
    data_table.pop_back();
}