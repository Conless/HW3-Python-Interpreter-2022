#ifndef APPLE_PIE_SCOPE_H
#define APPLE_PIE_SCOPE_H

#include <cstring>
#include <string>
#include <unordered_map>

#include "Python3BaseVisitor.h"

struct QueryResult {
    bool exist;
    antlrcpp::Any data;

    QueryResult(bool exist, antlrcpp::Any data) : exist(exist), data(data) { }
};

class Scope {
  private:
    std::unordered_map<std::string, antlrcpp::Any> var_table;

  public:
    Scope() { var_table.clear(); }
    void VarRegister(const std::string &var_name, antlrcpp::Any var_data);
    QueryResult VarQuery(const std::string &var_name) const;
};

#endif // APPLE_PIE_SCOPE_H