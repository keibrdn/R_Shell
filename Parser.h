#ifndef PARSER_H
#define PARSER_H

#include "Lexer.h"






typedef pair<pSystemDo, pBaseTok> pairDoTok;
typedef pair<pCmndDo,   pBaseTok> pairCmndTok;
enum class eParseState { eOperand, eOperator};



class Parser
{
public:
    void        AddChars( string sInput);
    void        Reset()                   {m_Lexer.Reset();}
    pSystemDo   GetNextParenthesis();
protected:
    pairDoTok   GetNextTree();
    pairCmndTok GetNextDo();
    
    Lexer       m_Lexer;
    //eParseState m_eParseState = eParseState::eOperand;
};









#endif // PARSER_H
