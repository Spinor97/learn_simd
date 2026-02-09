#pragma once

#include "msg.hpp"
#include "fix_parser.hpp"

bool parsed_fix_msg(const char* msg, size_t len, parsedMsg &out,
    vector<int> &commas, vector<int> & nl);