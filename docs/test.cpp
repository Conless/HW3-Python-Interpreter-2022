std::string func_name = ctx->NAME()->getText();
Function func_data({visitParameters(ctx->parameters()).as<std::vector<std::pair<std::string, antlrcpp::Any>>>(), ctx->suite()});
func_table.VarRegister(func_name, func_data);
return {};