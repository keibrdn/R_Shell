#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <list>
#include <memory>

#include "Do.h"

using namespace std;














enum class eState{ eCmndState = 101, eArgState = 102, eQtArgState = 103, eCnctState = 104};





class Lexer
{
public:
    pBaseTok  GetNextToken();
    void      UnGetToken( pBaseTok pToken);
    void      AddChars( string sInput);
    void      TokenizeCin();
    string    TokenizeString(string sInput);
    void      DiscardResidue();
    void      Reset();

protected: 
    int       CurrentChar();  
    void      DiscardChar();
    
    eState         m_eState = eState::eCmndState;
    //bool           m_bIsPriorCharSemiColon = false;
    bool           m_bPriorCloseParenthesis = false;
    bool           m_bPriorCloseSqrBracket  = false;
    UnLong         m_nCharCount = 0;
    UnLong         m_nLineCount = 0;
    UnLong         m_nParenthesisCount = 0;
    list<int>      m_CharList;
    list<pBaseTok> m_TokenList;

};









#endif // LEXER_H
