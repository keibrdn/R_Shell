#include "Parser.h"

#include <iostream>

#include "SyntaxExcept.h"








void Parser::AddChars(string sInput)
{
    m_Lexer.AddChars(sInput);
}








pSystemDo Parser::GetNextParenthesis()
{
    pParenDo pParenthesisDo;
    bool     bLooping = true;
    
    pParenthesisDo = make_shared<ParenDo>();
    
    if( RSHELL_DEBUG)
        cout << "\n\n\n\n\n\n";
    
    while(bLooping)
    {
        pairDoTok pairSysTok;
        
        try
        {
            pairSysTok = GetNextTree();
            
            if( RSHELL_DEBUG)
            {
                cout << "\nGot next Tree...";
                cout << "\n    Token   = " << pairSysTok.second->AsString();
                cout << "\n\n" << std::flush;
            }
            
            switch( pairSysTok.second->TokenType())
            {
                case eTokType::eCnctTok:
                    if( pairSysTok.second->TokenText() == SEMICOLON_TEXT)
                        pParenthesisDo->AddTree( pairSysTok.first);
                    else
                        throw SyntaxExcept("In Parser::GetNextParenthesis(), connect token text was not a \";\"");
                    break;
                    
                case eTokType::eCloseParenthTok:
                case eTokType::eEndCharsTok:
                    bLooping = false;
                    if( pairSysTok.first->CmndCount() > 0)
                        pParenthesisDo->AddTree( pairSysTok.first);
                    break;
                    
                default:
                    throw ShellExcept("In Parser::GetNextParenthesis(), unhandled \"case\".");
            }
        }
        catch( SyntaxExcept Ex)
        {
            cout << "\n\nCaught SyntaxExcept...\n" << Ex.what() << endl;
        
            m_Lexer.DiscardResidue();
            //pRootDo.reset();
        }
    }
    
    return pParenthesisDo;
}








pairDoTok Parser::GetNextTree()
{
    eParseState ParsState = eParseState::eOperand;
    pairDoTok   pairSysTok;
    pairCmndTok pairCT;
    pSystemDo   pRootDo;
    pOperDo     pOpDo;
    bool        bLooping = true;
    
    //try
    //{
        while( bLooping)
        {
           pairCT = GetNextDo();
           
           if( RSHELL_DEBUG)
           {
               cout << "\nGot next Do()...";
               cout << "\n    Command = " << pairCT.first->AsString();
               cout << "\n    Token   = " << pairCT.second->AsString();
               cout << "\n\n" << std::flush;
           }
           
           switch( ParsState)
           {
                case eParseState::eOperand:
                    switch( pairCT.second->TokenType())
                    {
                        case eTokType::eCompleteTok:
                            if( pRootDo)                                  //If the tree with root at pRootDo is not empty. (If pRootDo != nullptr)
                                pOpDo->SetRightPointer( pairCT.first);    
                            else
                                pRootDo = pairCT.first;                   //If here, then this is the first element (command) to be added to the tree with root at pRootDo.
                                
                            ParsState = eParseState::eOperator;
                            break;
                            
                        case eTokType::eCnctTok:
                            throw SyntaxExcept("In Parser::GetNextTree(), Syntax err... expected a command but found a connector.");
                            break;
                            
                        case eTokType::eEndCharsTok:
                            if( pRootDo)                                  //If the tree with root at pRootDo is not empty. (If pRootDo != nullptr)
                                throw SyntaxExcept("In Parser::GetNextTree(), Syntax err... expected a command but found \"End Of Chars\"...the tree is incomplete");
                            bLooping = false;
                            break;  
                            
                        case eTokType::eExitTok:
                            //An eExitTok Token should have been changed to a command (eCompleteTok) by GetNextPair() before arriving here.
                            throw ShellExcept( "In Parser::GetNextTree(), found a token of type eExitTok.");
                            
                        case eTokType::eOpenParenthTok:
                            if( pRootDo)                                  //If the tree with root at pRootDo is not empty. (If pRootDo != nullptr)
                                pOpDo->SetRightPointer( GetNextParenthesis());    
                            else
                                pRootDo = GetNextParenthesis();           //If here, then this is the first element (command) to be added to the tree with root at pRootDo.
                                
                            ParsState = eParseState::eOperator;
                            break;

                        case eTokType::eCloseParenthTok:
                            bLooping = false;
                            break;
                            
                        default:
                            throw ShellExcept("In Parser::GetNextTree(), unhandled \"case\".");
                    }
                    break;
                    
                    
                case eParseState::eOperator:
                    switch( pairCT.second->TokenType())
                    {
                        case eTokType::eCompleteTok:
                            throw SyntaxExcept("In Parser::GetNextTree(), Syntax err... expected an operator, but found a completed command.");
                            
                        case eTokType::eCnctTok:
                            if(      pairCT.second->TokenText() == LOGICAL_OR_TEXT)
                            {
                                pOrDo pDo = make_shared<OrDo>();
                                pDo->SetLeftPointer(pRootDo);
                                pRootDo = pOpDo = pDo;
                                ParsState = eParseState::eOperand;
                            }
                            else if( pairCT.second->TokenText() == LOGICAL_AND_TEXT)
                            {
                                pAndDo pDo = make_shared<AndDo>();
                                pDo->SetLeftPointer(pRootDo);
                                pRootDo = pOpDo = pDo;
                                ParsState = eParseState::eOperand;
                            }
                            else if( pairCT.second->TokenText() == SEMICOLON_TEXT)
                                bLooping = false;
                            break;
                            
                        case eTokType::eEndCharsTok:
                            if( pRootDo)                               //If the tree with root at pRootDo is not empty. (If pRootDo != nullptr)
                            {
                                m_Lexer.UnGetToken( pairCT.second);    //Save token for next call to GetNextDo().
                                bLooping = false;                      //Quit the "while" loop so that the current tree will be returned and processed.
                            }
                            break;
                            
                        case eTokType::eExitTok:
                            //An eExitTok Token should have been changed to a command (eCompleteTok) by GetNextPair() before arriving here.
                            throw ShellExcept( "In Parser::GetNextTree(), found a token of type eExitTok.");

                        case eTokType::eCloseParenthTok:
                            bLooping = false;
                            break;
                            

                        default:
                            throw ShellExcept("In Parser::GetNextTree(), unhandled \"case\".");
                    }
                    break;
            }
        }
    //}
    //catch( SyntaxExcept Ex)
    //{
    //    cout << "\n\nCaught SyntaxExcept...\n" << Ex.what() << endl;
    //    
    //    m_Lexer.DiscardResidue();
    //    pRootDo.reset();
    //}
    
    if( !pRootDo)                           //If the tree with root at pRootDo is empty...
        pRootDo = make_shared<SystemDo>();  //Add base class element to the tree. 
        
    pairSysTok.first = pRootDo;
    pairSysTok.second = pairCT.second;
    
    return pairSysTok;
}








pairCmndTok Parser::GetNextDo()
{
    pBaseTok    pToken;
    bool        bBracketed = false;
    pairCmndTok pairCT;
    pCmndDo     pDo = make_shared<CmndDo>();
    
    while(true)
    {
        pToken = m_Lexer.GetNextToken();
        
        if(   pToken->TokenType() == eTokType::eCmdTok
           || pToken->TokenType() == eTokType::eOpenSqrBracket
           || pToken->TokenType() == eTokType::eArgTok
           || pToken->TokenType() == eTokType::eQtArgTok  )
        {
            if( (pToken->TokenType() == eTokType::eOpenSqrBracket) && (pDo->VecTokenSize() == 0))
            {
                bBracketed = true;
                pToken = make_shared<CmdTok>(TEST_TEXT);
            }
            pDo->PushBack(pToken);
        }
        else
        {
            if( pDo->VecTokenSize() > 0)
            {
                if( !((bBracketed) && (pToken->TokenType() == eTokType::eCloseSqrBracket)))
                    m_Lexer.UnGetToken( pToken);
                
                pToken = make_shared<CompleteTok>("");
            }
            else if( pToken->TokenType() == eTokType::eExitTok)
            {
                pDo->PushBack( make_shared<CmdTok>( EXIT_TEXT));
                pToken = make_shared<CompleteTok>("");
            }
            
            break;
        }
    }
    
    pairCT.first  = pDo;
    pairCT.second = pToken;
    
    return pairCT;
}












