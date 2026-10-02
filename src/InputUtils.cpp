#include "InputUtils.h"
#include <sstream>

bool parseInteger (const std::string& text, int& value) {
    std::istringstream input (text);
    int parsed_value = 0;

    if (!(input >> parsed_value))
        return false;

    input >> std::ws;
    if (input.eof() == false)
        return false;

    value = parsed_value;
    return true;
}