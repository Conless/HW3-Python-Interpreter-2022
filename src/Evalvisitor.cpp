#include "pyinter/Evalvisitor.h"

#include "pyinter/utils.h"

int dfn = 0;

antlrcpp::Any EvalVisitor::visitFile_input(Python3Parser::File_inputContext *ctx) { return visitChildren(ctx); }

antlrcpp::Any EvalVisitor::visitFuncdef(Python3Parser::FuncdefContext *ctx) {
    // no func def
    return visitChildren(ctx);
}

antlrcpp::Any EvalVisitor::visitParameters(Python3Parser::ParametersContext *ctx) {
    // no func def
    return visitChildren(ctx);
}

antlrcpp::Any EvalVisitor::visitStmt(Python3Parser::StmtContext *ctx) {
    OutputFunction(__func__);
    return visitChildren(ctx);
}

antlrcpp::Any EvalVisitor::visitSimple_stmt(Python3Parser::Simple_stmtContext *ctx) {
    OutputFunction(__func__);
    return visitChildren(ctx);
}

antlrcpp::Any EvalVisitor::visitSmall_stmt(Python3Parser::Small_stmtContext *ctx) {
    OutputFunction(__func__);
    return visitChildren(ctx);
}

antlrcpp::Any EvalVisitor::visitExpr_stmt(Python3Parser::Expr_stmtContext *ctx) {
    OutputFunction(__func__);
    auto testlist_array = ctx->testlist();
    if (ctx->augassign()) {
        std::string opt = ctx->augassign()->getText();
        std::string var_name = testlist_array[0]->getText();
        auto var = scope.VarQuery(var_name);
        auto num = visitTestlist(testlist_array[1]);
        if (!var.exist)
            throw Exception("", UNDEFINED);
        if (opt == "+=") {
            scope.VarRegister(var_name, var.data + num);
        } else if (opt == "-=") {
            scope.VarRegister(var_name, var.data - num);
        } else if (opt == "*=") {
            scope.VarRegister(var_name, var.data * num);
        } else if (opt == "/=") {
            scope.VarRegister(var_name, var.data / num);
        } else if (opt == "//=") {
            // scope.VarRegister(var_name, divide(scope.VarQuery(var_name).second, var_data));
        }
        return scope.VarQuery(var_name).data;
    } else {
        if (testlist_array.size() == 1) {
            return visitTestlist(testlist_array[0]);
        } else if (testlist_array.size() == 2) {
            std::string var_name = testlist_array[0]->getText();
            antlrcpp::Any num = visitTestlist(testlist_array[1]);
            // auto num = scope.VarQuery(num_name);
            // if (!validateVarName(var_name))
            //     throw Exception("", INVALID_VARNAME);
            // TODO
            scope.VarRegister(var_name, num);
        } else {
            // TODO
            throw Exception("", UNIMPLEMENTED);
        }
    }
    return 0;
}

antlrcpp::Any EvalVisitor::visitFlow_stmt(Python3Parser::Flow_stmtContext *ctx) {
    OutputFunction(__func__);
    return visitChildren(ctx);
}

antlrcpp::Any EvalVisitor::visitCompound_stmt(Python3Parser::Compound_stmtContext *ctx) {
    OutputFunction(__func__);
    return visitChildren(ctx);
}

antlrcpp::Any EvalVisitor::visitIf_stmt(Python3Parser::If_stmtContext *ctx) {
    OutputFunction(__func__);
    auto test_array = ctx->test();
    auto suite_array = ctx->suite();
    for (int i = 0; i < test_array.size(); i++) {
        if (visitTest(test_array[i]).as<bool>())
            return visitSuite(suite_array[i]);
    }
    if (test_array.size() < suite_array.size())
        return visitSuite(suite_array.back());
    return {};
}

antlrcpp::Any EvalVisitor::visitWhile_stmt(Python3Parser::While_stmtContext *ctx) {
    OutputFunction(__func__);
    auto test_expr = ctx->test();
    auto suite_expr = ctx->suite();
    while (visitTest(test_expr).as<bool>()) {
        visitSuite(suite_expr);
    }
    return {};
}

antlrcpp::Any EvalVisitor::visitSuite(Python3Parser::SuiteContext *ctx) {
    OutputFunction(__func__);
    return visitChildren(ctx);
}

antlrcpp::Any EvalVisitor::visitTest(Python3Parser::TestContext *ctx) {
    OutputFunction(__func__);
    return visitChildren(ctx);
}

antlrcpp::Any EvalVisitor::visitOr_test(Python3Parser::Or_testContext *ctx) {
    OutputFunction(__func__);
    return visitChildren(ctx);
}

antlrcpp::Any EvalVisitor::visitAnd_test(Python3Parser::And_testContext *ctx) {
    OutputFunction(__func__);
    return visitChildren(ctx);
}

antlrcpp::Any EvalVisitor::visitNot_test(Python3Parser::Not_testContext *ctx) {
    OutputFunction(__func__);
    return visitChildren(ctx);
}

antlrcpp::Any EvalVisitor::visitComparison(Python3Parser::ComparisonContext *ctx) {
    OutputFunction(__func__);
    auto term_array = ctx->arith_expr();
    auto opt_array = ctx->comp_op();
    std::vector<antlrcpp::Any> num_array;
    for (int i = 0; i < term_array.size(); i++)
        num_array.push_back(visitArith_expr(term_array[i]));
    if (num_array.size() == 1)
        return num_array[0];
    if (num_array.size() == 2) {
        std::string opt = opt_array[0]->getText();
        if (opt == "<")
            return num_array[0] < num_array[1];
        if (opt == ">")
            return num_array[0] > num_array[1];
        if (opt == "==")
            return num_array[0] == num_array[1];
        if (opt == ">=")
            return num_array[0] >= num_array[1];
        if (opt == "<=")
            return num_array[0] <= num_array[1];
        if (opt == "!=")
            return num_array[0] != num_array[1];
        throw Exception("", UNIMPLEMENTED);
    }
    return true;
    // return visitChildren(ctx);
}

antlrcpp::Any EvalVisitor::visitArith_expr(Python3Parser::Arith_exprContext *ctx) {
    OutputFunction(__func__);
    auto term_array = ctx->term();
    auto opt_array = ctx->addorsub_op();
    if (term_array.size() == 1)
        return visitTerm(term_array[0]);
    else {
        std::vector<antlrcpp::Any> num_array;
        for (int i = 0; i < term_array.size(); i++)
            num_array.push_back(visitTerm(term_array[i]));
        antlrcpp::Any num_res = num_array[0];
        for (int i = 1; i < term_array.size(); i++) {
            std::string opt = opt_array[i - 1]->getText();
            if (opt == "+")
                num_res = num_res + num_array[i];
            else if (opt == "-")
                num_res = num_res - num_array[i];
            else
                throw Exception("", UNIMPLEMENTED);
        }
        return num_res;
    }
}

antlrcpp::Any EvalVisitor::visitTerm(Python3Parser::TermContext *ctx) {
    OutputFunction(__func__);
    auto factorArray = ctx->factor();
    if (factorArray.size() == 1) {
        return visitFactor(factorArray[0]);
    }
    auto opArray = ctx->muldivmod_op();
    int ret = visitFactor(factorArray[0]).as<int>();
    for (int i = 1; i < factorArray.size(); ++i) {
        std::string tmpOp = opArray[i - 1]->getText();
        if (tmpOp == "*")
            ret = ret * visitFactor(factorArray[i]).as<int>();
        else if (tmpOp == "/")
            ret = ret / visitFactor(factorArray[i]).as<int>();
        else
            throw Exception("", UNIMPLEMENTED);
    }
    return ret;
}

antlrcpp::Any EvalVisitor::visitFactor(Python3Parser::FactorContext *ctx) {
    OutputFunction(__func__);
    antlrcpp::Any ret = visitChildren(ctx);
    // TODO
    return std::move(ret);
}

antlrcpp::Any EvalVisitor::visitAtom_expr(Python3Parser::Atom_exprContext *ctx) {
    OutputFunction(__func__);
    if (!ctx->trailer())
        return visitAtom(ctx->atom());
    auto functionName = ctx->atom()->getText();
    // std::cout << functionName << '\n';
    auto argsArray = visitTrailer(ctx->trailer()).as<std::vector<antlrcpp::Any>>();
    if (argsArray.size() != 1) {
        throw Exception(functionName, INVALID_FUNC_CALL);
    }
    if (functionName == "print") {
        std::cout << argsArray[0] << '\n';
        return 0;
    }
    throw Exception("", UNIMPLEMENTED);
}

antlrcpp::Any EvalVisitor::visitTrailer(Python3Parser::TrailerContext *ctx) {
    OutputFunction(__func__);
    if (ctx->arglist())
        return visitArglist(ctx->arglist());
    return std::move(std::vector<antlrcpp::Any>());
}

antlrcpp::Any EvalVisitor::visitAtom(Python3Parser::AtomContext *ctx) {
    OutputFunction(__func__);
    if (ctx->NUMBER()) {
        return TypeConverter::stringToNum(ctx->NUMBER()->getText());
    } else if (ctx->NAME()) {
        auto result = scope.VarQuery(ctx->NAME()->getText());
        if (result.exist) {
            return result.data;
        } else
            throw Exception(ctx->NAME()->getText(), UNDEFINED);
    } else if (ctx->TRUE()) {
        return true;
    } else if (ctx->FALSE()) {
        return false;
    } else if (ctx->NONE()) {
        return {};
    } else if (ctx->test()) {
        antlrcpp::Any res = visitTest(ctx->test());
        return res;
    } else {
        auto string_array = ctx->STRING();
        std::string str = string_array[0]->getText();
        str = str.substr(1, str.size() - 2);
        return std::move(str);
    }
    throw Exception("", UNIMPLEMENTED);
}

antlrcpp::Any EvalVisitor::visitTestlist(Python3Parser::TestlistContext *ctx) { return visitChildren(ctx); }

antlrcpp::Any EvalVisitor::visitArglist(Python3Parser::ArglistContext *ctx) {
    auto argumentArray = ctx->argument();
    std::vector<antlrcpp::Any> retArray;
    for (auto ctx : argumentArray) {
        if (ctx->test().size() > 1)
            throw Exception("", UNIMPLEMENTED);
        retArray.push_back(visitTest(ctx->test()[0]));
    }
    return retArray;
}

antlrcpp::Any EvalVisitor::visitArgument(Python3Parser::ArgumentContext *ctx) { return visitChildren(ctx); }