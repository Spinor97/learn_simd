#pragma once

#include "msg.hpp"
#include "fix_parser.cpp"

bool parsed_fix_msg(const char* msg, size_t len, parsedMsg &out,
    vector<int> &commas, vector<int> & nl) {
        commas = get_pos_char(msg, len, ',');
        nl = get_pos_char(msg, len, '\n');

        if (commas.size() < 2 && nl.size() < 1) {
            
            return false;
        }

        int c0 = commas[0], c1 = commas[1], c2 = nl[0];

        out.sym = msg;
        out.symLen = commas[0];

        out.px = msg + c0 + 1;
        out.pxLen = c1 - c0 - 1;

        out.qty = msg + c1 + 1;
        out.qtyLen = c2 - c1 - 1;

        return true;
}