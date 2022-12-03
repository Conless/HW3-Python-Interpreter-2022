#ifndef APPLE_PIE_SCOPE_H
#define APPLE_PIE_SCOPE_H

#include <cstring>
#include <string>
#include <stack>
#include <unordered_map>

#include "Python3BaseVisitor.h"

struct Function {
    std::vector<std::pair<std::string, antlrcpp::Any>> para_array;
    Python3Parser::SuiteContext *suite_array;
};

struct QueryResult {
    bool exist;
    antlrcpp::Any data;

    QueryResult(bool exist, antlrcpp::Any data) : exist(exist), data(data) {}
};

class Scope {
  private:
    std::vector<std::unordered_map<std::string, antlrcpp::Any>> data_table;

  public:
    Scope();
    void VarRegister(const std::string &var_name, antlrcpp::Any var_data, int type = 0);
    QueryResult VarQuery(const std::string &var_name) const;
    void push();
    void pop();
};

#endif // APPLE_PIE_SCOPE_H