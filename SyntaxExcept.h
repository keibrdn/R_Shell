#ifndef SYNTAXEXCEPT_H
#define SYNTAXEXCEPT_H

#include <exception>
#include <stdexcept>


#include "Utility.h"

using namespace std;








class SyntaxExcept : public std::runtime_error
{
public:
                SyntaxExcept(std::string sText) : std::runtime_error(sText){};
    static void Except( UnLong nLineCount, UnLong nCharCount, string sMesg);
};
















class ShellExcept : public std::runtime_error
{
public:
    ShellExcept(std::string sText) : std::runtime_error(sText){};
};









#endif // SYNTAXEXCEPT_H
