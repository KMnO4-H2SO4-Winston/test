#include "term.h"

Term::Term(std::string info) {
    this->fgNoLinearEquationInOne(info);
    this->fgWrongTerm(info);
    this->_info = info;
    std::regex unknownRegex("((?:\\+|-)(?:\\d*?(?:\\.\\d*?)?))([a-zA-Z])");
    std::regex leftParenthesisRegex("((?:\\+|-)(?:\\d*?(?:\\.\\d*?)?))([a-zA-Z]?)\\(");
    std::regex rightParenthesisRegex("((?:\\+|-)(?:\\d*?(?:\\.\\d*?)?))([a-zA-Z]?)\\)");
    std::smatch unknownMatch;
    if (std::regex_match(this->_info, unknownMatch, unknownRegex) ||
       std::regex_match(this->_info, unknownMatch, leftParenthesisRegex) ||
       std::regex_match(this->_info, unknownMatch, rightParenthesisRegex)) {
        if (unknownMatch[1] == "+") {
            this->_coefficient = 1;
        } else if (unknownMatch[1] == "-") {
            this->_coefficient = -1;
        } else {
            this->strStream << unknownMatch[1];
            this->strStream >> this->_coefficient;
        }
        this->_unknown = unknownMatch[2];
    } else {
        this->strStream << this->_info;
        this->strStream >> this->_coefficient;
        this->_unknown = "";
    }
    strStream.clear();
    if (this->_coefficient == 0) {
        this->_unknown = "";
        this->_info = "0";
    }
}

void Term::fgNoLinearEquationInOne(std::string info) {
    std::string msg;
    std::regex TwoUnknownRegex("(?:\\+|-)(?:\\d*?(?:\\.\\d*?)?)([a-zA-Z])([a-zA-Z])");
    std::smatch TwoUnknownMatch;
    if (std::regex_match(info, TwoUnknownMatch, TwoUnknownRegex)) {
        if (TwoUnknownMatch[1] == TwoUnknownMatch[2]) {
            msg = "excepted quadratic equation";
        } else {
            msg = "excepted multivariate equation";
        }
        throw FormatError(msg.c_str());
    }
}

void Term::fgWrongTerm(std::string info) {
    std::string msg;
    size_t unknownIdx = -1;
    for (size_t idx = 0; idx < info.size(); idx++) {
        if (isalpha(info[idx])) {
            unknownIdx = idx;
        }
    }
    if (unknownIdx > 0) {
        if (unknownIdx < info.size() - 1 &&
            info[unknownIdx + 1] != '(' &&
            info[unknownIdx + 1] != ')') {
            msg = "\"";
            if (info[info.size() - 1] == '(' ||
                info[info.size() - 1] == ')') {
                msg += info.substr(0, info.size() - 1);
            } else {
                msg += info.substr(0, info.size());
            }
            msg += "\" is not a RIGHT term";
            throw FormatError(msg.c_str());
        }
    }
}

double Term::getCoefficient() {
    return this->_coefficient;
}

std::string Term::getUnknown() {
    return this->_unknown;
}

void Term::getCoefficientToOne(double num) {
    this->_coefficient /= num;
    this->strStream << this->_coefficient;
    this->strStream >> this->_info;
    this->strStream.clear();
    this->_info += this->_unknown;
    if (this->_unknown != "") {
        switch((int)this->_coefficient) {
            case +1:
                this->_info = this->_unknown;
                break;
            case -1:
                this->_info = "-" + this->_unknown;
                break;
        }
    }
}

std::string* Term::getInfo() {
    return &this->_info;
}
