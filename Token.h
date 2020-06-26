#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <memory>

using namespace std;







enum class eTokType{ eUnknownTok = 10, eCmdTok = 12, eArgTok = 13, eQtArgTok = 14, eCnctTok = 15, eExitTok = 16,
                      eEndCharsTok = 17, eOpenParenthTok = 18, eCloseParenthTok = 19, eOpenSqrBracket = 20, eCloseSqrBracket = 21, eCompleteTok = 22};




class BaseTok
{
protected:
    eTokType m_eType;
    string   m_sText;
    
public:
             BaseTok()    {m_eType = eTokType::eUnknownTok;}
    string   TokenText()  {return m_sText;}
    string   AsString();
    eTokType TokenType()  {return m_eType;}
};




class CmdTok : public BaseTok
{
public:
    CmdTok(string sText) {m_eType = eTokType::eCmdTok; m_sText = sText;}
};




class ArgTok : public BaseTok
{
public:
    ArgTok(string sText) {m_eType = eTokType::eArgTok; m_sText = sText;}
};




class QtArgTok : public BaseTok
{
public:
    QtArgTok(string sText) {m_eType = eTokType::eQtArgTok; m_sText = sText;}
};




class CnctTok : public BaseTok
{
public:
    CnctTok(string sText) {m_eType = eTokType::eCnctTok; m_sText = sText;}
};




class XTok : public BaseTok
{
public:
    XTok(string sText) {m_eType = eTokType::eExitTok; m_sText = sText;}
};




class EndCharsTok : public BaseTok
{
public:
    EndCharsTok(string sText) {m_eType = eTokType::eEndCharsTok; m_sText = sText;}
};




class OpenParenthTok : public BaseTok
{
public:
    OpenParenthTok(string sText) {m_eType = eTokType::eOpenParenthTok; m_sText = sText;}
};




class CloseParenthTok : public BaseTok
{
public:
    CloseParenthTok(string sText) {m_eType = eTokType::eCloseParenthTok; m_sText = sText;}
};




class CompleteTok : public BaseTok
{
public:
    CompleteTok(string sText) {m_eType = eTokType::eCompleteTok; m_sText = sText;}
};




class OpenSqrBracketTok : public BaseTok
{
public:
    OpenSqrBracketTok(string sText) {m_eType = eTokType::eOpenSqrBracket; m_sText = sText;}
};




class CloseSqrBracketTok : public BaseTok
{
public:
    CloseSqrBracketTok(string sText) {m_eType = eTokType::eCloseSqrBracket; m_sText = sText;}
};





typedef std::shared_ptr<BaseTok>     pBaseTok;
typedef std::shared_ptr<CmdTok>      pCmdTok;
typedef std::shared_ptr<ArgTok>      pArgTok;
typedef std::shared_ptr<QtArgTok>    pQtArgTok;
typedef std::shared_ptr<CnctTok>     pCnctTok;
typedef std::shared_ptr<XTok>        pXTok;
typedef std::shared_ptr<EndCharsTok> pEndTok;












#endif // TOKEN_H
