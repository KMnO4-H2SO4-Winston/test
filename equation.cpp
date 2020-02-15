#include "equation.h"

Equation::Equation(std::string info) {
    this->fgNoLinearEquationInOne(info);
    this->fgWrongEquation(info);
    this->_info = info;
    std::vector<std::string> expressions = split(this->_info, "=");
    this->_leftExpression = new Expression(expressions[0]);
    this->_rightExpression = new Expression(expressions[1]);
}

void Equation::fgWrongEquation(std::string info) {
    std::regex formatRegex(".+?=.+?");
    std::string msg = "\"" + info;
    msg += "\" is not a equation";
    bool hasAlpha = false;
    for (size_t idx = 0; idx < info.size(); idx++) {
        if (isalpha(info[idx])) {
            hasAlpha = true;
        }
    }
    if (!std::regex_match(info, formatRegex) || !hasAlpha) {
        throw FormatError(msg.c_str());
    }
}

void Equation::fgNoLinearEquationInOne(std::string info) {
    std::string msg;
    char unknown = ' ';
    for (size_t idx = 0; idx < info.size(); idx++) {
        if (isalpha(info[idx])) {
            if (unknown == ' ') {
                unknown = info[idx];
            } else {
                if (unknown != info[idx]) {
                    msg = "excepted multivariate equation";
                    throw FormatError(msg.c_str());
                }
            }
        }
    }
}

void Equation::removeParenthesis(){
    this->_leftExpression->removeParenthesis();
    this->_rightExpression->removeParenthesis();
    this->_info = *this->_leftExpression->getInfo() + '=' + *this->_rightExpression->getInfo();
}

void Equation::transpose() {
    this->_leftExpression->transpose(LEFT, this->_rightExpression);
    this->_rightExpression->transpose(RIGHT, this->_leftExpression);
    this->_info = *this->_leftExpression->getInfo() + '=' + *this->_rightExpression->getInfo();
}

void Equation::combineLikeTerms() {
    this->_leftExpression->combineLikeTerms();
    this->_rightExpression->combineLikeTerms();
    this->_info = *this->_leftExpression->getInfo() + '=' + *this->_rightExpression->getInfo();
}

void Equation::getCoefficientToOne() {
    double divisor = this->_leftExpression->getCoefficient();
    bool hasSolution = true;
    if (divisor != 0) {
        this->_leftExpression->getCoefficientToOne(divisor);
        this->_rightExpression->getCoefficientToOne(divisor);
    } else {
        this->_rightExpression->getCoefficientToOne(1);
        hasSolution = false;
    }
    this->_info = *this->_leftExpression->getInfo() + '=' + *this->_rightExpression->getInfo();
    if(!hasSolution) this->_info += "(no solution)";
    delete this->_leftExpression;
    delete this->_rightExpression;
}

std::string* Equation::getInfo() {
    removeParenthesis();
    transpose();
    combineLikeTerms();
    getCoefficientToOne();
    return &this->_info;
}
