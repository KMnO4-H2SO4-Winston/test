#include "expression.h"

Expression::Expression(std::string info) {
    this->_info = info;
    this->_coefficient = 0;
    this->_unknown = "";
    if (this->_info[0] != '+' && this->_info[0] != '-') this->_info.insert(0, 1, '+');
    size_t idx = 0;
    while (idx < this->_info.size()) {
        if (this->_info[idx] == '(') {
            if (this->_info[idx + 1] != '+' && this->_info[idx + 1] != '-') {
                this->_info.insert(idx + 1, 1, '+');
                idx++;
            }
        }
        idx++;
    }
    std::string infoTmp = this->_info;
    idx = 0;
    while (idx < infoTmp.size()) {
        if (idx > 0) {
            if (infoTmp[idx] == '+' || infoTmp[idx] == '-') {
                infoTmp.insert(idx, 1, '~');
                idx++;
            }
        }
        idx++;
    }
    std::vector<std::string> strTerms = split(infoTmp, "~");
    for (idx = 0; idx < strTerms.size(); idx++) {
        Term* termTmp = new Term(strTerms[idx]);
        this->_terms.push_back(termTmp);
    }
}

double Expression::getCoefficient() {
    double coefficientTmp = 0;
    for (size_t idx = 0; idx < this->_terms.size(); idx++) {
        coefficientTmp += this->_terms[idx]->getCoefficient();
    }
    return coefficientTmp;
}

std::string Expression::getUnknown() {
    std::string unknownTmp = "";
    for (size_t idx = 0; idx < this->_terms.size(); idx++) {
        if((unknownTmp = this->_terms[idx]->getUnknown()) == "") break;
    }
    return unknownTmp;
}

std::vector<Term*>* Expression::getTerms() {
    return &this->_terms;
}

void Expression::removeParenthesis() {
    std::regex leftParenthesisRegex("(?:\\+|-)(?:\\d*?(?:\\.\\d*?)?)(?:[a-zA-Z]?)\\(");
    std::regex rightParenthesisRegex("(?:\\+|-)(?:\\d*?(?:\\.\\d*?)?)(?:[a-zA-Z]?)\\)");
    size_t idx = this->_terms.size() - 1;
    size_t leftParenthesisIdx = 0;
    size_t rightParenthesisIdx = 0;
    double multiplierCoefficient;
    std::string multiplierUnknown;
    std::vector<Term*>::reverse_iterator iter = this->_terms.rbegin();
    std::vector<Term*>::iterator iterTmp;
    while (iter != this->_terms.rend() && idx >= 0) {
        if (std::regex_match(*(*iter)->getInfo(), leftParenthesisRegex)) {
            leftParenthesisIdx = idx;
            multiplierCoefficient = (*iter)->getCoefficient();
            multiplierUnknown = (*iter)->getUnknown();
            iterTmp = this->_terms.erase((++iter).base());
            iter = std::vector<Term*>::reverse_iterator(iterTmp);
            std::vector<Term*>::iterator iterInside;
            size_t idxInside = 0;
            for (iterInside = this->_terms.begin(); iterInside != this->_terms.end(); iterInside++) {
                if (std::regex_match(*(*iterInside)->getInfo(), rightParenthesisRegex) ) {
                    rightParenthesisIdx = idxInside;
                    break;
                }
                idxInside++;
            }
            idxInside = leftParenthesisIdx;
            iterInside = this->_terms.begin() + leftParenthesisIdx;
            while (iterInside != this->_terms.begin() + rightParenthesisIdx + 1 &&
                  idxInside < this->_terms.size()) {
                std::string changeTermInfo;
                double changeTermCoefficient = (*iterInside)->getCoefficient();
                changeTermCoefficient *= multiplierCoefficient;
                this->strStream << changeTermCoefficient;
                this->strStream >> changeTermInfo;
                this->strStream.clear();
                changeTermInfo += (*iterInside)->getUnknown();
                changeTermInfo += multiplierUnknown;
                if (changeTermInfo[0] != '+' && changeTermInfo[0] != '-') changeTermInfo.insert(0, 1, '+');
                std::regex TwoUnknownRegex("(?:\\+|-)(?:\\d*?(?:\\.\\d*?)?)([a-zA-Z])([a-zA-Z])");
                std::smatch TwoUnknownMatch;
                std::string msg;
                if (std::regex_match(changeTermInfo, TwoUnknownMatch, TwoUnknownRegex)) {
                    if (TwoUnknownMatch[1] == TwoUnknownMatch[2]) {
                        msg = "excepted quadratic equation";
                    } else {
                        msg = "excepted multivariate equation";
                    }
                    throw FormatError(msg.c_str());
                }
                Term* changeTerm = new Term(changeTermInfo);
                this->_terms[idxInside] = changeTerm;
                iterInside++;
                idxInside++;
            }
        } else {
            iter++;
            idx--;
        }
    }
    this->_info = "";
    for (idx = 0; idx < this->_terms.size(); idx++) {
        this->_info += *this->_terms[idx]->getInfo();
    }
}

void Expression::transpose(side atSide, Expression* anotherExpression) {
    std::regex unknownRegex("((?:\\+|-)(?:\\d*?(?:\\.\\d*?)?))([a-zA-Z])");
    std::vector<Term*>::iterator iter = this->_terms.begin();
    while (iter != this->_terms.end()) {
        std::string tmp = *(*iter)->getInfo();
        if((!std::regex_match(*(*iter)->getInfo(), unknownRegex) && atSide == LEFT) ||
            (std::regex_match(*(*iter)->getInfo(), unknownRegex) && atSide == RIGHT)) {
            std::string moveTermInfo = *(*iter)->getInfo();
            iter = this->_terms.erase(iter);
            char symbol = moveTermInfo[0];
            if (symbol == '-') {
                symbol = '+';
            } else {
                symbol = '-';
            }
            moveTermInfo = symbol + moveTermInfo.substr(1);
            Term* moveTerm = new Term(moveTermInfo);
            anotherExpression->getTerms()->push_back(moveTerm);
        } else {
            iter++;
        }
    }
    this->_info = "";
    if (this->_terms.size() == 0) {
        this->_info = "0";
        Term* termTmp = new Term(this->_info);
        this->_terms.push_back(termTmp);
    } else {
        for (size_t idx = 0; idx < _terms.size(); idx++) {
            _info += *(_terms[idx]->getInfo());
        }
    }
}

void Expression::combineLikeTerms() {
    this->_coefficient = getCoefficient();
    this->_unknown = getUnknown();
    std::string termAfterCombiningInfo;
    this->strStream << this->_coefficient;
    this->strStream >> termAfterCombiningInfo;
    this->strStream.clear();
    termAfterCombiningInfo += this->_unknown;
    if (termAfterCombiningInfo[0] != '+' && termAfterCombiningInfo[0] != '-')
        termAfterCombiningInfo.insert(0, 1, '+');
    Term* termAfterCombining = new Term(termAfterCombiningInfo);
    this->_terms.clear();
    this->_terms.push_back(termAfterCombining);
    this->_info = *this->_terms[0]->getInfo();
}

void Expression::getCoefficientToOne(double num) {
    this->_terms[0]->getCoefficientToOne(num);
    this->_info = *this->_terms[0]->getInfo();
}

std::string* Expression::getInfo() {
    return &this->_info;
}
