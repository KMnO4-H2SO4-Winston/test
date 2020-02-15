#ifndef SOLVEEQUATIONS_TERM_H
#define SOLVEEQUATIONS_TERM_H
#include "main.h"

class Term {
public:
    Term(std::string info);
    double getCoefficient();
    std::string getUnknown();
    void getCoefficientToOne(double num);
    std::string* getInfo();
private:
    void fgNoLinearEquationInOne(std::string info);
    void fgWrongTerm(std::string info);
    std::string _info;
    double _coefficient;
    std::string _unknown;
    std::stringstream strStream;
};

#endif //SOLVEEQUATIONS_TERM_H
