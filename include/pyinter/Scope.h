#ifndef APPLE_PIE_SCOPE_H
#define APPLE_PIE_SCOPE_H

#include <map>
#include <string>

class Scope {

    private:
        std::map<std::string, int> varTable;

    public:
        Scope(): varTable() {}
        void varRegister(const std::string& varName, int varData) {
            varTable[varName] = varData;
        }    

        std::pair<bool, int> varQuery(const std::string& varName) const {
            auto it = varTable.find(varName);
            if (it == varTable.end()) return std::make_pair(false, 0);
            return std::make_pair(true, it->second);
        }
};

#endif //APPLE_PIE_SCOPE_H