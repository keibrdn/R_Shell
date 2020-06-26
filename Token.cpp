#include "Token.h"







string BaseTok::AsString()
{
    string sReturn;
    
    switch( m_eType)
    {
        case eTokType::eUnknownTok      : sReturn = "Unknown        "; break;
        case eTokType::eCmdTok          : sReturn = "Command        "; break;
        case eTokType::eArgTok          : sReturn = "Argument       "; break;
        case eTokType::eQtArgTok        : sReturn = "Quoted Argument"; break;
        case eTokType::eCnctTok         : sReturn = "Connect        "; break;
        case eTokType::eExitTok         : sReturn = "Exit           "; break;
        case eTokType::eEndCharsTok     : sReturn = "EndOfChars     "; break;
        case eTokType::eOpenParenthTok  : sReturn = "OpenParenth    "; break;
        case eTokType::eCloseParenthTok : sReturn = "CloseParenth   "; break;
        case eTokType::eCompleteTok     : sReturn = "CompleteCommand"; break;
        case eTokType::eOpenSqrBracket  : sReturn = "OpenSqrBracket "; break;
        case eTokType::eCloseSqrBracket : sReturn = "CloseSqrBracket"; break;
    }
    
    size_t nColumn2 = 20 - sReturn.size();
    for(size_t i = 0; i< nColumn2; i++)
        sReturn += ' ';
    
    sReturn += TokenText();
    
    return sReturn;
}









