#ifndef PYTHON_INTERPRETER_EVALVISITOR_H
#define PYTHON_INTERPRETER_EVALVISITOR_H

#include "Exception.h"
#include "Python3BaseVisitor.h"
#include "Scope.h"
#include "utils.h"

Scope scope;

int toInt(antlrcpp::Any const &a) {
    if (a.is<int>())
        return a.as<int>();
    else if (a.is<double>())
        return int(a.as<double>());
    else if (a.is<std::string>())
        return stringToInt(a.as<std::string>());
    else if (a.is<bool>())
        return a.as<bool>() ? 1 : 0;
    else
        return 0;
}

double toFloat(antlrcpp::Any const &a) {
    if (a.is<double>())
        return a.as<double>();
    else if (a.is<int>())
        return (double)a.as<int>();
    else if (a.is<std::string>())
        return std::stod(a.as<std::string>());
    else if (a.is<bool>())
        return a.as<bool>() ? 1.0 : 0.0;
    else
        return 0.0;
}

bool toBool(antlrcpp::Any const &a) {
    if (a.is<bool>())
        return a.as<bool>();
    else if (a.is<int>())
        return a.as<int>() != 0;
    else if (a.is<double>())
        return a.as<double>() != 0;
    else if (a.is<std::string>())
        return a.as<std::string>().length() > 0;
    else
        return false;
}

std::string to_string(const int &a) {
    int x = a;
    std::string str = "";
    while (x) {
        str.insert(0, 1, x % 10 + '0');
        x /= 10;
    }
    return str;
}

std::string toStr(antlrcpp::Any const &a) {
    if (a.is<std::string>())
        return a;
    else if (a.is<int>())
        return to_string(a.as<int>());
    else if (a.is<double>())
        return std::to_string(a.as<double>());
    else if (a.is<bool>())
        return a.as<bool>() ? "True" : "False";
    else
        return std::string();
}

antlrcpp::Any operator+(antlrcpp::Any const &a, antlrcpp::Any const &b) {
    if (a.is<std::string>() || b.is<std::string>())
        return toStr(a) + toStr(b);
    else if (a.is<double>() || b.is<double>())
        return toFloat(a) + toFloat(b);
    else
        return toInt(a) + toInt(b);
}

antlrcpp::Any operator-(antlrcpp::Any const &a, antlrcpp::Any const &b) {
    if (a.is<double>() || b.is<double>())
        return toFloat(a) - toFloat(b);
    else
        return toInt(a) - toInt(b);
}

antlrcpp::Any operator*(antlrcpp::Any const &a, antlrcpp::Any const &b) {
    if (a.is<std::string>() || b.is<std::string>()) {
        std::string s = a.is<std::string>() ? a.as<std::string>() : b.as<std::string>();
        int times = toInt(a.is<std::string>() ? b : a);
        return s * int(times);
    } else if (a.is<double>() || b.is<double>()) {
        double x = toFloat(a), y = toFloat(b);
        return x * y;
    } else {
        int x = toInt(a), y = toInt(b);
        return x * y;
    }
}

antlrcpp::Any DivInt(antlrcpp::Any const &a, antlrcpp::Any const &b) {
    if (a.is<double>() || b.is<double>()) {
        double x = toFloat(a), y = toFloat(b);
        return (int)(x / y);
    } else {
        return toInt(a) / toInt(b);
    }
}

class EvalVisitor : public Python3BaseVisitor {

    // todo:override all methods of Python3BaseVisitor
  public:
    virtual antlrcpp::Any visitFile_input(Python3Parser::File_inputContext *ctx) override { return visitChildren(ctx); }

    virtual antlrcpp::Any visitFuncdef(Python3Parser::FuncdefContext *ctx) override {
        // no func def
        return visitChildren(ctx);
    }

    virtual antlrcpp::Any visitParameters(Python3Parser::ParametersContext *ctx) override {
        // no func def
        return visitChildren(ctx);
    }

    virtual antlrcpp::Any visitStmt(Python3Parser::StmtContext *ctx) override { return visitChildren(ctx); }

    virtual antlrcpp::Any visitSimple_stmt(Python3Parser::Simple_stmtContext *ctx) override { return visitChildren(ctx); }

    virtual antlrcpp::Any visitSmall_stmt(Python3Parser::Small_stmtContext *ctx) override { return visitChildren(ctx); }

    virtual antlrcpp::Any visitExpr_stmt(Python3Parser::Expr_stmtContext *ctx) override {
        if (ctx->augassign()) {
            throw Exception("", UNIMPLEMENTED);
        }
        auto testlistArray = ctx->testlist();
        if (testlistArray.size() == 1) {
            visitTestlist(testlistArray[0]);
            return 0;
        } else if (testlistArray.size() > 2) {
            throw Exception("", UNIMPLEMENTED);
        }

        std::string varName = testlistArray[0]->getText();
        int varData = visitTestlist(testlistArray[1]);

        if (!validateVarName(varName)) {
            throw Exception("", INVALID_VARNAME);
        }
        scope.varRegister(varName, varData);
        return 0;
    }

    virtual antlrcpp::Any visitFlow_stmt(Python3Parser::Flow_stmtContext *ctx) override { return visitChildren(ctx); }

    virtual antlrcpp::Any visitCompound_stmt(Python3Parser::Compound_stmtContext *ctx) override { return visitChildren(ctx); }

    virtual antlrcpp::Any visitIf_stmt(Python3Parser::If_stmtContext *ctx) override { return visitChildren(ctx); }

    virtual antlrcpp::Any visitWhile_stmt(Python3Parser::While_stmtContext *ctx) override { return visitChildren(ctx); }

    virtual antlrcpp::Any visitSuite(Python3Parser::SuiteContext *ctx) override { return visitChildren(ctx); }

    virtual antlrcpp::Any visitTest(Python3Parser::TestContext *ctx) override { return visitChildren(ctx); }

    virtual antlrcpp::Any visitOr_test(Python3Parser::Or_testContext *ctx) override { return visitChildren(ctx); }

    virtual antlrcpp::Any visitAnd_test(Python3Parser::And_testContext *ctx) override { return visitChildren(ctx); }

    virtual antlrcpp::Any visitNot_test(Python3Parser::Not_testContext *ctx) override { return visitChildren(ctx); }

    virtual antlrcpp::Any visitComparison(Python3Parser::ComparisonContext *ctx) override { return visitChildren(ctx); }

    virtual antlrcpp::Any visitArith_expr(Python3Parser::Arith_exprContext *ctx) override {
        auto termArray = ctx->term();
        if (termArray.size() == 1)
            return visitTerm(termArray[0]).as<int>();
        auto opArray = ctx->addorsub_op();
        int ret = visitTerm(termArray[0]).as<int>();
        for (int i = 1; i < termArray.size(); ++i) {
            std::string tmpOp = opArray[i - 1]->getText();
            if (tmpOp == "+")
                ret += visitTerm(termArray[i]).as<int>();
            else if (tmpOp == "-")
                ret -= visitTerm(termArray[i]).as<int>();
            else
                throw Exception("", UNIMPLEMENTED);
        }
        return ret;
    }

    virtual antlrcpp::Any visitTerm(Python3Parser::TermContext *ctx) override {
        auto factorArray = ctx->factor();
        if (factorArray.size() == 1)
            return visitFactor(factorArray[0]).as<int>();
        auto opArray = ctx->muldivmod_op();
        antlrcpp::Any ret = visitFactor(factorArray[0]).as<int>();
        for (int i = 1; i < factorArray.size(); ++i) {
            std::string tmpOp = opArray[i - 1]->getText();
            if (tmpOp == "*")
                ret = ret * (antlrcpp::Any)visitFactor(factorArray[i]).as<int>();
            else if (tmpOp == "/")
                ret = DivInt(ret, visitFactor(factorArray[i]).as<int>());
            else
                throw Exception("", UNIMPLEMENTED);
        }
        return ret;
    }

    virtual antlrcpp::Any visitFactor(Python3Parser::FactorContext *ctx) override { return visitChildren(ctx); }

    virtual antlrcpp::Any visitAtom_expr(Python3Parser::Atom_exprContext *ctx) override {
        if (!ctx->trailer()) {
            // std::cout << visitAtom(ctx->atom()).as<int>() << '\n';
            return visitAtom(ctx->atom()).as<int>();
        }
        auto functionName = ctx->atom()->getText();
        // std::cout << functionName << '\n';
        auto argsArray = visitTrailer(ctx->trailer()).as<std::vector<int>>();
        if (argsArray.size() != 1) {
            throw Exception(functionName, INVALID_FUNC_CALL);
        }
        if (functionName == "print") {
            std::cout << argsArray[0] << std::endl;
            return 0;
        }
        throw Exception("", UNIMPLEMENTED);
    }

    virtual antlrcpp::Any visitTrailer(Python3Parser::TrailerContext *ctx) override {
        if (ctx->arglist())
            return visitArglist(ctx->arglist());
        return std::vector<int>();
    }

    virtual antlrcpp::Any visitAtom(Python3Parser::AtomContext *ctx) override {
        if (ctx->NUMBER()) {
            return stringToNum(ctx->NUMBER()->getText());
        } else if (ctx->NAME()) {
            auto result = scope.varQuery(ctx->NAME()->getText());
            if (result.first) {
                return result.second;
            } else
                throw Exception(ctx->NAME()->getText(), UNDEFINED);
        } else if (ctx->test())
            return visitTest(ctx->test()).as<int>();
        throw Exception("", UNIMPLEMENTED);
    }

    virtual antlrcpp::Any visitTestlist(Python3Parser::TestlistContext *ctx) override { return visitChildren(ctx); }

    virtual antlrcpp::Any visitArglist(Python3Parser::ArglistContext *ctx) override {
        auto argumentArray = ctx->argument();
        std::vector<int> retArray;
        for (auto ctx : argumentArray) {
            if (ctx->test().size() > 1)
                throw Exception("", UNIMPLEMENTED);
            retArray.push_back(visitTest(ctx->test()[0]));
        }
        return retArray;
    }

    virtual antlrcpp::Any visitArgument(Python3Parser::ArgumentContext *ctx) override { return visitChildren(ctx); }
};

#endif // PYTHON_INTERPRETER_EVALVISITOR_H
