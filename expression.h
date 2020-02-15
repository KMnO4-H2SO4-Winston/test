#ifndef SOLVEEQUATIONS_EXPRESSION_H
#define SOLVEEQUATIONS_EXPRESSION_H
#include "main.h"
#include "term.h"

class Expression {
public:
    Expression(std::string info);
    double getCoefficient();
    std::string getUnknown();
    std::vector<Term*>* getTerms();
    void removeParenthesis();
    void transpose(side atSide, Expression* anotherExpression);
    void combineLikeTerms();
    void getCoefficientToOne(double num);
    std::string* getInfo();
private:
    std::string _info;
    std::string _unknown;
    double _coefficient;
    std::vector<Term*> _terms;
    std::stringstream strStream;
};

#endif //SOLVEEQUATIONS_EXPRESSION_H
