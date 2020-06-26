#include "SyntaxExcept.h"

#include <string>








void SyntaxExcept::Except( UnLong nLineCount, UnLong nCharCount, string sMesg)
{
    string sText;
    
    sText  = "Syntax error on or about line number ";
    sText += to_string( nLineCount);
    sText += " and character number ";
    sText += to_string( nCharCount);
    sText += " ... ";
    sText += sMesg;
    
    throw SyntaxExcept( sText);
}
