#ifndef SOLVEEQUATIONS_MAIN_H
#define SOLVEEQUATIONS_MAIN_H
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <regex>
#include <cctype>
#include "error.h"

void inputEquation(std::string& equationInfo, bool& isWrong);
std::vector<std::string> split(std::string string, std::string replaceStr);
bool isIllegal(char character);

typedef bool side;
#define LEFT true
#define RIGHT false

#endif //SOLVEEQUATIONS_MAIN_H
