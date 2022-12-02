#include "pyinter/Evalvisitor.h"

#include "int2048/int2048.h"
#include "pyinter/utils.h"

int stk = 0;

Scope var_table, func_table;

typedef sjtu::int2048 ll;
const ll kFlowContinue = 85234095823904;
const ll kFlowBreak = 584085390455439;
const ll kFlowReturn = 35890234859034;

antlrcpp::Any EvalVisitor::visitFile_input(Python3Parser::File_inputContext *ctx) { return visitChildren(ctx); }

antlrcpp::Any EvalVisitor::visitFuncdef(Python3Parser::FuncdefContext *ctx) {
    OutputFunction(__func__);
    std::string func_name = ctx->NAME()->getText();
    Function func_data(
        {visitParameters(ctx->parameters()).as<std::vector<std::pair<std::string, antlrcpp::Any>>>(), ctx->suite()});
    func_table.VarRegister(func_name, func_data);
    return {};
}

antlrcpp::Any EvalVisitor::visitParameters(Python3Parser::ParametersContext *ctx) {
    std::vector<std::pair<std::string, antlrcpp::Any>> para_array_init;
    if (!ctx->typedargslist())
        return para_array_init;
    auto tfpdef_array = ctx->typedargslist()->tfpdef();
    auto test_array = ctx->typedargslist()->test();
    int fir_init = tfpdef_array.size() - test_array.size();
    for (int i = 0; i < tfpdef_array.size(); i++) {
        std::string var_name = tfpdef_array[i]->NAME()->getText();
        antlrcpp::Any var_data;
        if (i < fir_init)
            var_data = {};
        else
            var_data = visitTest(test_array[i - fir_init]);
        para_array_init.push_back(make_pair(var_name, var_data));
    }
    return para_array_init;
}

antlrcpp::Any EvalVisitor::visitStmt(Python3Parser::StmtContext *ctx) {
    OutputFunction(__func__);
    return visitChildren(ctx);
}

antlrcpp::Any EvalVisitor::visitSimple_stmt(Python3Parser::Simple_stmtContext *ctx) {
    OutputFunction(__func__);
    return visitSmall_stmt(ctx->small_stmt());
}

antlrcpp::Any EvalVisitor::visitSmall_stmt(Python3Parser::Small_stmtContext *ctx) {
    OutputFunction(__func__);
    return visitChildren(ctx);
}

std::vector<std::string> cutstring(std::string ctx) {
    int pos = 0, len = ctx.size();
    std::vector<std::string> ret;
    while (pos < len) {
        for (int j = pos; j <= len; j++) {
            if (j == len || ctx[j] == ',') {
                ret.push_back(ctx.substr(pos, j - pos));
                pos = j + 1;
                break;
            }
        }
    }
    return ret;
}

antlrcpp::Any EvalVisitor::visitExpr_stmt(Python3Parser::Expr_stmtContext *ctx) {
    OutputFunction(__func__);
    auto testlist_array = ctx->testlist();
    if (ctx->augassign()) {
        std::string opt = ctx->augassign()->getText();
        std::string var_name = testlist_array[0]->getText();
        auto var = var_table.VarQuery(var_name);
        auto num = visitTestlist(testlist_array[1]);
        if (!var.exist)
            throw Exception("", UNDEFINED);
        // std::cout << opt << std::endl;
        if (opt == "+=") {
            var_table.VarRegister(var_name, var.data + num);
        } else if (opt == "-=") {
            var_table.VarRegister(var_name, var.data - num);
        } else if (opt == "*=") {
            var_table.VarRegister(var_name, var.data * num);
        } else if (opt == "/=") {
            var_table.VarRegister(var_name, var.data / num);
        } else if (opt == "//=") {
            var_table.VarRegister(var_name, divide(var.data, num));
        } else if (opt == "%=") {
            var_table.VarRegister(var_name, var.data % num);
        }
        return var_table.VarQuery(var_name).data;
    } else {
        if (testlist_array.size() == 1) {
            return visitTestlist(testlist_array[0]);
            // } else if (testlist_array.size() == 2) {
            //     std::string var_name = testlist_array[0]->getText();
            //     antlrcpp::Any num = visitTestlist(testlist_array[1]);
            //     // auto num = var_table.VarQuery(num_name);
            //     // if (!validateVarName(var_name))
            //     //     throw Exception("", INVALID_VARNAME);
            //     // TODO
            //     var_table.VarRegister(var_name, num);
            //     // if (num.is<int>())
            //     //     std::cout << 1;
            //     return num;
        } else {
            int siz = testlist_array.size();
            for (int i = siz - 2; i >= 0; i--) {
                antlrcpp::Any ret = visitTestlist(testlist_array[i + 1]);
                if (ret.is<std::vector<antlrcpp::Any>>()) {
                    int cnt = ret.as<std::vector<antlrcpp::Any>>().size();
                    auto var_name = cutstring(testlist_array[i]->getText());
                    for (int j = 0; j < cnt; j++) {
                        var_table.VarRegister(var_name[j], ret.as<std::vector<antlrcpp::Any>>()[j]);
                    }
                } else {
                    auto var_name = testlist_array[i]->getText();
                    var_table.VarRegister(var_name, ret);
                }
            }
        }
        return {};
    }
    return 0;
}

antlrcpp::Any EvalVisitor::visitFlow_stmt(Python3Parser::Flow_stmtContext *ctx) {
    OutputFunction(__func__);
    if (ctx->continue_stmt())
        return kFlowContinue;
    if (ctx->break_stmt())
        return kFlowBreak;
    if (ctx->return_stmt()) {
        antlrcpp::Any ret_data = {};
        auto test_list = ctx->return_stmt()->testlist();
        if (test_list)
            ret_data = visitTestlist(test_list);
        return std::make_pair(kFlowReturn, antlrcpp::Any(ret_data));
    }
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
        if (TypeConverter::toBool(visitTest(test_array[i])))
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
    while (TypeConverter::toBool(visitTest(test_expr))) {
        antlrcpp::Any ret = visitSuite(suite_expr);
        if (ret.is<ll>() && ret.as<ll>() == kFlowBreak)
            break;
        if (ret.is<std::pair<ll, antlrcpp::Any>>() && ret.as<std::pair<ll, antlrcpp::Any>>().first == kFlowReturn)
            return ret;
    }
    return {};
}

antlrcpp::Any EvalVisitor::visitSuite(Python3Parser::SuiteContext *ctx) {
    OutputFunction(__func__);
    if (ctx->simple_stmt())
        return visit(ctx->simple_stmt());
    auto Array = ctx->stmt();
    for (int i = 0; i < Array.size(); ++i) {
        antlrcpp::Any ret = visitStmt(Array[i]);
        if (ret.is<ll>()) {
            if (ret.as<ll>() == kFlowBreak)
                return kFlowBreak;
            if (ret.as<ll>() == kFlowContinue)
                return kFlowContinue;
        }
        if (ret.is<std::pair<ll, antlrcpp::Any>>() && ret.as<std::pair<ll, antlrcpp::Any>>().first == kFlowReturn) {
            return ret;
        }
    }
    return {};
}

antlrcpp::Any EvalVisitor::visitTest(Python3Parser::TestContext *ctx) {
    OutputFunction(__func__);
    return visitChildren(ctx);
}

antlrcpp::Any EvalVisitor::visitOr_test(Python3Parser::Or_testContext *ctx) {
    OutputFunction(__func__);
    auto or_array = ctx->and_test();
    if (or_array.size() == 1)
        return visitAnd_test(or_array[0]);
    for (int i = 0; i < or_array.size(); i++) {
        if (TypeConverter::toBool(visitAnd_test(or_array[i])))
            return true;
    }
    return false;
}

antlrcpp::Any EvalVisitor::visitAnd_test(Python3Parser::And_testContext *ctx) {
    OutputFunction(__func__);
    auto not_array = ctx->not_test();
    if (not_array.size() == 1)
        return visitNot_test(not_array[0]);
    for (int i = 0; i < not_array.size(); i++) {
        if (!TypeConverter::toBool(visitNot_test(not_array[i])))
            return false;
    }
    return true;
}

antlrcpp::Any EvalVisitor::visitNot_test(Python3Parser::Not_testContext *ctx) {
    OutputFunction(__func__);
    if (ctx->NOT()) {
        return !(TypeConverter::toBool(visitNot_test(ctx->not_test())));
    } else {
        return visitComparison(ctx->comparison());
    }
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
    auto ret = visitFactor(factorArray[0]);
    for (int i = 1; i < factorArray.size(); ++i) {
        std::string tmpOp = opArray[i - 1]->getText();
        auto right_value = visitFactor(factorArray[i]);
        if (tmpOp == "*")
            ret = ret * right_value;
        else if (tmpOp == "/")
            ret = ret / right_value;
        else if (tmpOp == "%")
            ret = ret % right_value;
        else if (tmpOp == "//")
            ret = divide(ret, right_value);
        else
            throw Exception("", UNIMPLEMENTED);
    }
    return ret;
}

antlrcpp::Any EvalVisitor::visitFactor(Python3Parser::FactorContext *ctx) {
    OutputFunction(__func__);
    antlrcpp::Any ret = visitChildren(ctx);
    // TODO
    if (ctx->MINUS()) {
        if (ret.is<ll>())
            ret = -ret.as<ll>();
        if (ret.is<double>())
            ret = -ret.is<double>();
    }
    return ret;
}

antlrcpp::Any EvalVisitor::visitAtom_expr(Python3Parser::Atom_exprContext *ctx) {
    OutputFunction(__func__);
    if (!ctx->trailer()) {
        antlrcpp::Any ret = visitAtom(ctx->atom());
        // if (ret.is<int>())
        //     std::cout << 1;
        return ret;
        // return visitAtom(ctx->atom());
    }
    auto functionName = ctx->atom()->getText();
    // std::cout << functionName << '\n';
    auto argsArray = visitTrailer(ctx->trailer()).as<std::vector<antlrcpp::Any>>();
    // TODO
    if (functionName == "print") {
        if (argsArray.size())
            std::cout << argsArray[0];
        std::cout << '\n';
        return 0;
    }
    else if (functionName == "int") {
        if (argsArray[0].is<double>())
            return (ll)((int)argsArray[0].as<double>());
        if (argsArray[0].is<ll>())
            return argsArray[0].as<ll>();
        if (argsArray[0].is<std::string>())
            return ll(argsArray[0].as<std::string>());
        throw Exception("", UNIMPLEMENTED);
    } else if (functionName == "float") {
        if (argsArray[0].is<double>())
            return argsArray[0].as<double>();
        if (argsArray[0].is<ll>())
            return double(argsArray[0].as<ll>());
        if (argsArray[0].is<std::string>())
            return TypeConverter::stringToNum(argsArray[0].as<std::string>());
        throw Exception("", UNIMPLEMENTED);
    } else if (functionName == "str") {
        return TypeConverter::toString(argsArray[0]);
    } else if (functionName == "bool") {
        if (argsArray[0].is<double>())
            return argsArray[0].as<double>() != 0;
        if (argsArray[0].is<ll>())
            return argsArray[0].as<ll>() != 0;
        if (argsArray[0].is<bool>())
            return argsArray[0].as<bool>();
        throw Exception("", UNIMPLEMENTED);
    }
    QueryResult func_query = func_table.VarQuery(functionName);
    if (func_query.exist) {
        // printf("\nCustom Function\n");
        // OutputFunction(functionName.c_str());
        ++stk;
        // std::cout << functionName << "(" << stk << ")"
                //   << " ";
        Function func_now = func_query.data;
        var_table.push();
        for (int i = 0; i < func_now.para_array.size(); i++)
            var_table.VarRegister(func_now.para_array[i].first, func_now.para_array[i].second);
        // TODO
        for (int i = 0; i < argsArray.size(); i++) {
            var_table.VarRegister(func_now.para_array[i].first, argsArray[i]);
            // std::cout << argsArray[i] << " ";
        }
        // std::cout << "\n";
        antlrcpp::Any tmp = visitSuite(func_now.suite_array);
        var_table.pop();
        // printf("End function %s(%d)\n", functionName.c_str(), stk);
        stk--;
        if (tmp.is<std::pair<ll, antlrcpp::Any>>())
            return tmp.as<std::pair<ll, antlrcpp::Any>>().second;
        return tmp;
    }
    throw Exception("", UNIMPLEMENTED);
}

antlrcpp::Any EvalVisitor::visitTrailer(Python3Parser::TrailerContext *ctx) {
    OutputFunction(__func__);
    if (ctx->arglist())
        return visitArglist(ctx->arglist());
    return std::vector<antlrcpp::Any>();
}

antlrcpp::Any EvalVisitor::visitAtom(Python3Parser::AtomContext *ctx) {
    OutputFunction(__func__);
    if (ctx->NUMBER()) {
        antlrcpp::Any ret = TypeConverter::stringToNum(ctx->NUMBER()->getText());
        // if (ret.is<int>())
        //     std::cout << 1;
        return ret;
    } else if (ctx->NAME()) {
        auto result = var_table.VarQuery(ctx->NAME()->getText());
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

antlrcpp::Any EvalVisitor::visitTestlist(Python3Parser::TestlistContext *ctx) {
    auto testArray = ctx->test();
    if (testArray.size() == 1)
        return visitTest(testArray[0]);
    std::vector<antlrcpp::Any> ret;
    for (int i = 0; i < testArray.size(); ++i)
        ret.push_back(visitTest(testArray[i]));
    return ret;
}

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
