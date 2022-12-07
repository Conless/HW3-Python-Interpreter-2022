#include "pyinter/Evalvisitor.h"

#include "int2048/int2048.h"
#include "pyinter/exception.h"
#include "pyinter/utils.h"
#include "pyinter/convert.h"

int stk = 0;
int show_status;

Scope var_table, func_table;

enum FlowType { kFlowContinue, kFlowBreak, kFlowReturn, kPosition };

std::unordered_map<int, Scope> scope_func;

void JudgeInput(int argc, char *argv[]) {
    if (argc >= 2) {
        if (!strcmp(argv[1], "--show-status=0"))
            show_status = 0;
        else if (!strcmp(argv[1], "--show-status=1"))
            show_status = 1;
        else if (!strcmp(argv[1], "--show-status=2"))
            show_status = 2;
    }
}

void OutputFunction(const char *s) {
    if (show_status == 2)
        printf("Into function %s\n", s);
    return;
}

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
        if (Convert<bool>()(visitTest(test_array[i])))
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
    while (Convert<bool>()(visitTest(test_expr))) {
        antlrcpp::Any ret = visitSuite(suite_expr);
        if (ret.is<FlowType>() && ret.as<FlowType>() == kFlowBreak)
            break;
        if (ret.is<std::pair<FlowType, antlrcpp::Any>>() && ret.as<std::pair<FlowType, antlrcpp::Any>>().first == kFlowReturn)
            return ret;
    }
    return {};
}

antlrcpp::Any EvalVisitor::visitSuite(Python3Parser::SuiteContext *ctx) {
    OutputFunction(__func__);
    if (ctx->simple_stmt())
        return visit(ctx->simple_stmt());
    auto stmt_array = ctx->stmt();
    for (int i = 0; i < stmt_array.size(); i++) {
        antlrcpp::Any ret_value = visitStmt(stmt_array[i]);
        if (ret_value.is<FlowType>()) {
            if (ret_value.as<FlowType>() == kFlowBreak)
                return kFlowBreak;
            if (ret_value.as<FlowType>() == kFlowContinue)
                return kFlowContinue;
        }
        if (ret_value.is<std::pair<FlowType, antlrcpp::Any>>() &&
            ret_value.as<std::pair<FlowType, antlrcpp::Any>>().first == kFlowReturn) {
            return ret_value;
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
        if (Convert<bool>()(visitAnd_test(or_array[i])))
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
        if (!Convert<bool>()(visitNot_test(not_array[i])))
            return false;
    }
    return true;
}

antlrcpp::Any EvalVisitor::visitNot_test(Python3Parser::Not_testContext *ctx) {
    OutputFunction(__func__);
    if (ctx->NOT()) {
        return !(Convert<bool>()(visitNot_test(ctx->not_test())));
    } else {
        return visitComparison(ctx->comparison());
    }
}

antlrcpp::Any EvalVisitor::visitComparison(Python3Parser::ComparisonContext *ctx) {
    OutputFunction(__func__);
    auto term_array = ctx->arith_expr();
    auto opt_array = ctx->comp_op();
    auto las_num = visitArith_expr(term_array[0]);
    if (term_array.size() == 1)
        return las_num;
    for (int i = 1; i < term_array.size(); i++) {
        auto now_num = visitArith_expr(term_array[i]);
        std::string opt = opt_array[i - 1]->getText();
        bool res;
        if (opt == "<")
            res = las_num < now_num;
        else if (opt == ">")
            res = las_num > now_num;
        else if (opt == "==")
            res = las_num == now_num;
        else if (opt == ">=")
            res = las_num >= now_num;
        else if (opt == "<=")
            res = las_num <= now_num;
        else if (opt == "!=")
            res = las_num != now_num;
        else
            throw Exception("", UNIMPLEMENTED);
        if (!res)
            return false;
        las_num = now_num;
    }
    return true;
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
    auto factor_array = ctx->factor();
    if (factor_array.size() == 1) {
        return visitFactor(factor_array[0]);
    }
    auto opt_array = ctx->muldivmod_op();
    auto ret = visitFactor(factor_array[0]);
    for (int i = 1; i < factor_array.size(); i++) {
        std::string opt = opt_array[i - 1]->getText();
        auto right_value = visitFactor(factor_array[i]);
        if (opt == "*")
            ret = ret * right_value;
        else if (opt == "/")
            ret = ret / right_value;
        else if (opt == "%")
            ret = ret % right_value;
        else if (opt == "//")
            ret = divide(ret, right_value);
        else
            throw Exception("", UNIMPLEMENTED);
    }
    return ret;
}

antlrcpp::Any EvalVisitor::visitFactor(Python3Parser::FactorContext *ctx) {
    OutputFunction(__func__);
    antlrcpp::Any ret_value = visitChildren(ctx);
    if (ctx->MINUS()) {
        if (ret_value.is<ll>())
            ret_value = -ret_value.as<ll>();
        else if (ret_value.is<double>())
            ret_value = -ret_value.as<double>();
        else
            throw Exception("", UNIMPLEMENTED);
    }
    return ret_value;
}

void FunctionParaReg(Function func_now, std::vector<antlrcpp::Any> arg_array) {
    var_table.push();
    for (int i = 0; i < func_now.para_array.size(); i++)
        var_table.VarRegister(func_now.para_array[i].first, func_now.para_array[i].second, 1);
    stk++;
    for (int i = 0; i < arg_array.size(); i++) {
        if (arg_array[i].is<std::pair<FlowType, std::string>>()) {
            std::string var_name = arg_array[i].as<std::pair<ll, std::string>>().second;
            QueryResult qres = scope_func[stk].VarQuery(var_name);
            var_table.VarRegister(var_name, qres.data, 1);
        } else {
            var_table.VarRegister(func_now.para_array[i].first, arg_array[i], 1);
        }
    }
}

antlrcpp::Any EvalVisitor::visitAtom_expr(Python3Parser::Atom_exprContext *ctx) {
    OutputFunction(__func__);
    if (!ctx->trailer()) {
        antlrcpp::Any ret_value = visitAtom(ctx->atom());
        return ret_value;
    }
    auto func_name = ctx->atom()->getText();
    stk++;
    auto arg_array = visitTrailer(ctx->trailer()).as<std::vector<antlrcpp::Any>>();
    stk--;
    if (func_name == "print") {
        for (auto ctx_now : arg_array)
            std::cout << ctx_now << ' ';
        std::cout << '\n';
        return {};
    } else if (func_name == "int") {
        return Convert<ll>()(arg_array[0]);
    } else if (func_name == "float") {
        return Convert<double>()(arg_array[0]);
    } else if (func_name == "str") {
        return Convert<std::string>()(arg_array[0]);
    } else if (func_name == "bool") {
        return Convert<bool>()(arg_array[0]);
    }
    QueryResult func_query = func_table.VarQuery(func_name);
    if (func_query.exist) {
        if (show_status) {
            std::cout << func_name << "(" << stk << ")"
                      << " ";
        }
        Function func_now = func_query.data;
        FunctionParaReg(func_now, arg_array);
        if (show_status) {
            for (int i = 0; i < func_now.para_array.size(); i++)
                std::cout << var_table.VarQuery(func_now.para_array[i].first).data << ' ';
            std::cout << "\n";
        }
        antlrcpp::Any ret_value = visitSuite(func_now.suite_array);
        stk--;
        var_table.pop();
        if (ret_value.is<std::pair<FlowType, antlrcpp::Any>>())
            ret_value = ret_value.as<std::pair<FlowType, antlrcpp::Any>>().second;
        if (show_status) {
            printf("End function %s(%d) with return value ", func_name.c_str(), stk);
            std::cout << ret_value << '\n';
        }
        return ret_value;
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
        std::string num = ctx->NUMBER()->getText();
        if (num.find('.') == std::string::npos)
            return Convert<ll>()(num);
        else return Convert<double>()(num);
    } else if (ctx->NAME()) {
        std::string var_name = ctx->NAME()->getText();
        auto result = var_table.VarQuery(var_name);
        if (result.exist) {
            return result.data;
        } else
            return std::make_pair(kPosition, var_name);
    } else if (ctx->TRUE()) {
        return true;
    } else if (ctx->FALSE()) {
        return false;
    } else if (ctx->NONE()) {
        return {};
    } else if (ctx->test()) {
        return visitTest(ctx->test());
    } else {
        auto str_array = ctx->STRING();
        std::string str = str_array[0]->getText();
        str = str.substr(1, str.size() - 2);
        return str;
    }
}

antlrcpp::Any EvalVisitor::visitTestlist(Python3Parser::TestlistContext *ctx) {
    auto test_array = ctx->test();
    if (test_array.size() == 1)
        return visitTest(test_array[0]);
    std::vector<antlrcpp::Any> ret;
    for (int i = 0; i < test_array.size(); i++)
        ret.push_back(visitTest(test_array[i]));
    return ret;
}

antlrcpp::Any EvalVisitor::visitArglist(Python3Parser::ArglistContext *ctx) {
    auto argument_array = ctx->argument();
    std::vector<antlrcpp::Any> ret_array;
    for (int i = 0; i < argument_array.size(); i++) {
        antlrcpp::Any ret = visitArgument(argument_array[i]);
        ret_array.push_back(ret);
    }
    return ret_array;
}

antlrcpp::Any EvalVisitor::visitArgument(Python3Parser::ArgumentContext *ctx) {
    auto test_array = ctx->test();
    if (!ctx->ASSIGN()) {
        return visitTest(test_array[0]);
    } else {
        std::string var_name = test_array[0]->getText();
        auto var_data = visitTest(test_array[1]);
        scope_func[stk].VarRegister(var_name, var_data);
        return std::make_pair(kPosition, var_name);
    }
}
