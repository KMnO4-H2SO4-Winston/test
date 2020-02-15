#include "main.h"
#include "equation.h"

void inputEquation(std::string& equationInfo, bool& isWrong) {
    std::string msg;
    isWrong = false;
    std::cout << "Please enter an equation you want to solve:";
    getline(std::cin, equationInfo);
    if (equationInfo.size() == 0) {
        msg = "empty input.";
        isWrong = true;
        throw InputError(msg.c_str());
    } else {
        for (size_t idx = 0; idx < equationInfo.size(); idx++) {
            char character = equationInfo[idx];
            if (isIllegal(character)) {
                msg = "excepted illegal character \"";
                msg += character;
                msg += "\".";
                isWrong = true;
                throw InputError(msg.c_str());
            }
        }
    }
}

std::vector<std::string> split(std::string str, std::string replaceStr) {
    std::string::size_type pos;
    std::vector<std::string> result;
    str += replaceStr;
    for (size_t idx = 0; idx < str.size(); idx++) {
        pos = str.find(replaceStr, idx);
        if (pos < str.size()) {
            std::string strTmp = str.substr(idx, pos - idx);
            result.push_back(strTmp);
            idx = pos + replaceStr.size() - 1;
        }
    }
    return result;
}

bool isIllegal(char character) {
    std::string illegals = "!@#$%^&*{}[]|/\\;:'\",<>?`~_ ";
    for (size_t idx = 0; idx < illegals.size(); idx++) {
        if (character == illegals[idx]) {
            return true;
        }
    }
    return false;
}

int main() {
    std::string equationInfo;
    bool isWrong;
    INPUT_LOOP:do {
        try {
            inputEquation(equationInfo, isWrong);
        } catch (InputError& error) {
            std::cerr << error.what() << std::endl;
        }
    } while (isWrong);
    try {
        Equation *equation = new Equation(equationInfo);
        std::cout << "The solution of this equation:" << *equation->getInfo() << std::endl;
        delete equation;
        std::cout << "Press \"Enter\" to exit.";
        std::cin.get();
        return 0;
    } catch (FormatError& error) {
        std::cerr << error.what() << std::endl;
        goto INPUT_LOOP;
    }
}
