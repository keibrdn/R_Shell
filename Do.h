#ifndef DO_H
#define DO_H

#include <vector>

#include "Utility.h"
#include "Token.h"

using namespace std;






class SystemDo
{
public:
    virtual     ~SystemDo() {}
    virtual int Do();
    virtual int CmndCount() {return 0;}
};

typedef shared_ptr<SystemDo> pSystemDo;














typedef vector<pBaseTok> vec_pTok;



class CmndDo : public SystemDo
{
    char**       m_pArray = nullptr;
    vec_pTok     m_vec_pTokens;
    
    //const char*  GetCommand();
    //const char** GetArray();
    void         NewArrayMemory();
    void         DeleteArrayMemory();
    void         Check4ValidCmnd();
    int          DoTest();
    
public:
             ~CmndDo();
    int     CmndCount() override       {return 1;}
    void    PushBack(pBaseTok pToken);
    void    Clear()                    {m_vec_pTokens.clear();}
    size_t  VecTokenSize()             {return m_vec_pTokens.size();}
    int     Do() override;
    string  AsString();
    //int     DoTest();
};


typedef shared_ptr<CmndDo> pCmndDo;
















class OperatorDo : public SystemDo
{
protected:
    pSystemDo m_pLeft;
    pSystemDo m_pRight;
    
public:
         OperatorDo()  {}
         ~OperatorDo() {}
    int  CmndCount() override;
    void SetLeftPointer( pSystemDo pLeft);
    void SetRightPointer(pSystemDo pRight);
};

typedef shared_ptr<OperatorDo> pOperDo;














class AndDo : public OperatorDo
{
public:
            AndDo() {}
            ~AndDo(){}
    int     Do() override;
};

typedef shared_ptr<AndDo> pAndDo;
















class OrDo : public OperatorDo
{
public:
            OrDo() {}
            ~OrDo(){}
    int     Do() override;
};

typedef shared_ptr<OrDo> pOrDo;















typedef vector<pSystemDo> vecDoTree;


class ParenDo : public SystemDo 
{
protected:
    vecDoTree m_vecTree;
public:
         ParenDo() {}
         ~ParenDo(){}
    int  Do() override;
    int  CmndCount() override; 
    void AddTree(pSystemDo pSysDo) {m_vecTree.push_back(pSysDo);}
};

typedef shared_ptr<ParenDo> pParenDo;








#endif // DO_H
