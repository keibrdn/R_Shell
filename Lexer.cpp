#include "Lexer.h"
#include "SyntaxExcept.h"
#include "Utility.h"

#include <iostream>







pBaseTok Lexer::GetNextToken()
{
    string sTokText;
    
    try
    {
        if( !m_TokenList.empty())
        {
            pBaseTok pToken = m_TokenList.front();
            m_TokenList.pop_front();
            return pToken;
        }
        
        
        while( true)
        {
            if( sTokText.size() == 0)
            {
                switch( CurrentChar())
                {
                    case ' ':
                        switch( m_eState)
                        {
                            case eState::eCmndState:
                            case eState::eArgState:
                            case eState::eCnctState:
                                DiscardChar();
                                continue;

                            case eState::eQtArgState:
                                break;
                    
                        }
                        break;
                    
                    
                    case '#':
                        switch( m_eState)
                        {
                            case eState::eQtArgState:
                                sTokText += CurrentChar();
                                DiscardChar();
                                continue;
                        
                            case eState::eCmndState:
                            case eState::eArgState:
                            case eState::eCnctState:
                                while( (CurrentChar() != '\n') && (CurrentChar() != EOF))
                                    DiscardChar();
                    
                        }
                        //No "break;" here...we want to drop into the next '\n' "case".
                    
                    
                    case '\n':
                        switch( m_eState)
                        {
                            case eState::eCmndState:
                                DiscardChar();
                                continue;
                      
                            case eState::eArgState:
                                m_eState = eState::eCnctState;
                                continue;
                            
                            case eState::eQtArgState:
                                SyntaxExcept::Except( m_nLineCount, m_nCharCount, "New line found while reading \"quoted argument\"");
                                
                            case eState::eCnctState:
                                m_CharList.front() = ';';
                        }
                        break;
                    
                    
                    case '\"':
                        switch( m_eState)
                        {
                            case eState::eCmndState:
                                SyntaxExcept::Except( m_nLineCount, m_nCharCount, "Quotation mark found when looking for \"command\"");

                            case eState::eArgState:
                                m_eState = eState::eQtArgState;
                                DiscardChar();
                                continue;
                                //break;
                    
                            case eState::eQtArgState:
                                SyntaxExcept::Except( m_nLineCount, m_nCharCount, "\"Quoted argument\" is an empty string.");
                    
                            case eState::eCnctState:
                                SyntaxExcept::Except( m_nLineCount, m_nCharCount, "Quotation mark found when looking for \"connector\"");
                        }
                        break;
                    
                    
                    case EOF:
                        //return make_shared<EndTok>(sTokText);
                        switch( m_eState)
                        {
                            case eState::eCmndState:
                                return make_shared<EndCharsTok>(sTokText);
                                
                            case eState::eArgState:
                            case eState::eCnctState:
                                m_CharList.push_front(';');
                                m_eState = eState::eCnctState;
                                break;
                
                            case eState::eQtArgState:
                                SyntaxExcept::Except( m_nLineCount, m_nCharCount, "Looking for ending quotation mark, but found \"exit\".");
                        }
                        break;

                    case '(':
                        switch( m_eState)
                        {
                            case eState::eCmndState:
                                DiscardChar();
                                m_nParenthesisCount++;
                                return make_shared<OpenParenthTok>("(");
                                
                            case eState::eArgState:
                                SyntaxExcept::Except( m_nLineCount, m_nCharCount, "Expected argument, but found \"(\".");
                                
                            case eState::eQtArgState:
                                //SyntaxExcept::Except( m_nLineCount, m_nCharCount, "Expected quoted argument, but found \"(\".");
                                break;
                                
                            case eState::eCnctState:
                                SyntaxExcept::Except( m_nLineCount, m_nCharCount, "Expected connector, but found \"(\".");
                        }
                        break;
                        
                    case ')':

                        switch( m_eState)
                        {
                            case eState::eCmndState:
                                if( m_nParenthesisCount > 0)
                                {
                                    //if( m_bIsPriorCharSemiColon)
                                    //{
                                        //If here, then the parser tree of class SystemDo objects was ended by the prior semicolon. 
                                        m_nParenthesisCount--;
                                        DiscardChar();
                                        return make_shared<CloseParenthTok>(")");
                                    //}
                                    //else
                                    //    SyntaxExcept::Except( m_nLineCount, m_nCharCount, "Expected command, but found \")\".");
                                }
                                else
                                    SyntaxExcept::Except( m_nLineCount, m_nCharCount, "Found a close parenthesis without a matching open parenthesis.");

                            case eState::eArgState:
                                //DiscardChar();
                                m_CharList.push_front(';');
                                m_eState = eState::eCnctState;
                                continue;
                                
                            case eState::eQtArgState:
                                SyntaxExcept::Except( m_nLineCount, m_nCharCount, "Expected quoted argument, but found \")\".");
                                
                            case eState::eCnctState:
                                if(m_bPriorCloseSqrBracket)
                                {
                                    DiscardChar();
                                    return make_shared<CloseParenthTok>(")");
                                }
                                else
                                    SyntaxExcept::Except( m_nLineCount, m_nCharCount, "Expected connector, but found \")\".");

                        }
                        break;
                        
                    case '[':

                        switch( m_eState)
                        {
                            case eState::eCmndState:
                                DiscardChar();
                                m_eState = eState::eArgState;
                                return make_shared<OpenSqrBracketTok>("[");

                            case eState::eArgState:
                                SyntaxExcept::Except( m_nLineCount, m_nCharCount, "Expected argument, but found \"[\".");
                                
                            case eState::eQtArgState:
                                break;
                                
                            case eState::eCnctState:
                                SyntaxExcept::Except( m_nLineCount, m_nCharCount, "Expected connector, but found \"[\".");

                        }
                        break;
                        
                    case ']':

                        switch( m_eState)
                        {
                            case eState::eCmndState:
                                SyntaxExcept::Except( m_nLineCount, m_nCharCount, "Expected command, but found \"]\".");

                            case eState::eArgState:
                                DiscardChar();
                                m_eState = eState::eCnctState;
                                return make_shared<CloseSqrBracketTok>("]");
                                
                            case eState::eQtArgState:
                                break;
                                
                            case eState::eCnctState:
                                SyntaxExcept::Except( m_nLineCount, m_nCharCount, "Expected connector, but found \"]\".");

                        }
                        break;
                }

            }
            else
            {
                switch( CurrentChar())
                {
                    case ' ':
                        switch( m_eState)
                        {
                            case eState::eCmndState:
                                m_eState = eState::eArgState;
                                return make_shared<CmdTok>(sTokText);

                            case eState::eArgState:
                                return make_shared<ArgTok>(sTokText);

                            case eState::eQtArgState:
                                break;
                    
                            case eState::eCnctState:
                                SyntaxExcept::Except( m_nLineCount, m_nCharCount, "Blank space found when looking for \"connector\"");

                        }
                        break;

                    
                    case '#':
                        switch( m_eState)
                        {
                            case eState::eCmndState:
                                return make_shared<CmdTok>(sTokText);

                            case eState::eArgState:
                                m_eState = eState::eCmndState;
                                return make_shared<ArgTok>(sTokText);
                    
                            case eState::eQtArgState:
                                break;
                        
                            case eState::eCnctState:
                                SyntaxExcept::Except( m_nLineCount, m_nCharCount, "Comment found when looking for \"connector\"");
                        }
                        break;


                    case '\n':
                        switch( m_eState)
                        {
                            case eState::eCmndState:
                                m_CharList.front() = ';';
                                m_eState = eState::eCnctState;
                                return make_shared<CmdTok>(sTokText);

                            case eState::eArgState:
                                m_eState = eState::eCnctState;
                                m_CharList.front() = ';';
                                return make_shared<ArgTok>(sTokText);
                      
                            case eState::eQtArgState:
                                SyntaxExcept::Except( m_nLineCount, m_nCharCount, "Found \'new line\" while reading \"quoted argument\".");
                    
                            case eState::eCnctState:
                                SyntaxExcept::Except( m_nLineCount, m_nCharCount, "Newline found when looking for \"connector\"");
                        }

                    
                    case '\"':
                        switch( m_eState)
                        {
                            case eState::eCmndState:
                                DiscardChar();
                                SyntaxExcept::Except( m_nLineCount, m_nCharCount, "Found quotation mark while reading \"command\"");

                            case eState::eArgState:
                                DiscardChar();
                                SyntaxExcept::Except( m_nLineCount, m_nCharCount, "Found quotation mark while reading \'argument\"");
                    
                            case eState::eQtArgState:
                                m_eState = eState::eArgState;
                                //sTokText += CurrentChar();
                                DiscardChar();
                                return make_shared<QtArgTok>(sTokText);
                    
                            case eState::eCnctState:
                                DiscardChar();
                                SyntaxExcept::Except( m_nLineCount, m_nCharCount, "Quotation mark found when looking for \"connector\"");
                        }
                        break;
                    
                    
                
                    case ';':
                        switch( m_eState)
                        {
                            case eState::eCmndState:
                                m_eState = eState::eCnctState;
                                return make_shared<CmdTok>(sTokText);

                            case eState::eArgState:
                                m_eState = eState::eCnctState;
                                return make_shared<ArgTok>(sTokText);
                      
                            case eState::eQtArgState:
                                break;
                    
                            case eState::eCnctState:
                                SyntaxExcept::Except( m_nLineCount, m_nCharCount, "");
                        }
                        break;
                 
                    
                    case EOF:
                        switch( m_eState)
                        {
                            case eState::eCmndState:
                                m_CharList.push_front(';');
                                m_eState = eState::eCnctState;
                                return make_shared<CmdTok>(sTokText);
                            case eState::eArgState:
                                m_CharList.push_front(';');
                                m_eState = eState::eCnctState;
                                return make_shared<ArgTok>(sTokText);
                            case eState::eCnctState:
                                m_CharList.push_front(';');
                                m_eState = eState::eCnctState;
                
                            case eState::eQtArgState:
                                SyntaxExcept::Except( m_nLineCount, m_nCharCount, "Looking for ending quotation mark, but found \"EndOfText\".");
                        }
                        break;

                    case '(':
                        switch( m_eState)
                        {
                            case eState::eCmndState:
                                SyntaxExcept::Except( m_nLineCount, m_nCharCount, "Expected command, but found \"(\".");
                                
                            case eState::eArgState:
                                SyntaxExcept::Except( m_nLineCount, m_nCharCount, "Expected argument, but found \"(\".");
                                
                            case eState::eQtArgState:
                                SyntaxExcept::Except( m_nLineCount, m_nCharCount, "Expected quoted argument, but found \"(\".");
                                
                            case eState::eCnctState:
                                SyntaxExcept::Except( m_nLineCount, m_nCharCount, "Expected connector, but found \"(\".");

                        }
                        break;
                        
                    case ')':
                        switch( m_eState)
                        {
                            case eState::eCmndState:
                                m_CharList.push_front(' ');
                                continue;
                                
                            case eState::eArgState:
                                m_CharList.push_front(' ');
                                continue;
                                
                            case eState::eQtArgState:
                                break;  //The ')' will be added to sTokText.
                                
                            case eState::eCnctState:
                                SyntaxExcept::Except( m_nLineCount, m_nCharCount, "Expected connector, but found \")\".");

                        }
                        break;
                    case '[':

                        switch( m_eState)
                        {
                            case eState::eCmndState:
                                SyntaxExcept::Except( m_nLineCount, m_nCharCount, "Expected comand, but found \"[\".");

                            case eState::eArgState:
                                SyntaxExcept::Except( m_nLineCount, m_nCharCount, "Expected argument, but found \"[\".");
                                
                            case eState::eQtArgState:
                                break;
                                
                            case eState::eCnctState:
                                SyntaxExcept::Except( m_nLineCount, m_nCharCount, "Expected connector, but found \"[\".");

                        }
                        break;
                    case ']':

                        switch( m_eState)
                        {
                            case eState::eCmndState:
                                //SyntaxExcept::Except( m_nLineCount, m_nCharCount, "Expected comand, but found \"]\".");
                                m_CharList.push_front(' ');
                                continue;

                            case eState::eArgState:
                                m_CharList.push_front(' ');
                                continue;
                                //return make_shared<ArgTok>( sTokText);
                                
                            case eState::eQtArgState:
                                break;
                                
                            case eState::eCnctState:
                                SyntaxExcept::Except( m_nLineCount, m_nCharCount, "Expected connector, but found \"]\".");

                        }
                        break;
                }
            }




            
            sTokText += CurrentChar();
            DiscardChar();

        
        

            
            //if( (sTokText == "&&") || (sTokText == "||") || (sTokText == ";"))
            if( (sTokText == LOGICAL_AND_TEXT) || (sTokText == LOGICAL_OR_TEXT) || (sTokText == ";"))
            {
                switch( m_eState)
                {
                    case eState::eCmndState:
                        if( (!m_bPriorCloseParenthesis) || (sTokText == ";"))
                            SyntaxExcept::Except( m_nLineCount, m_nCharCount, "Looking for \"command\", but found \"connector\".");

                    case eState::eArgState:
                    case eState::eCnctState:
                        m_eState = eState::eCmndState;
                        return make_shared<CnctTok>(sTokText);
                
                    case eState::eQtArgState:
                        break;


                }
            }
        
        
        
            if( sTokText == EXIT_TEXT)
            {
                switch( m_eState)
                {
                    case eState::eCmndState:
                        m_eState = eState::eCnctState;
                        return make_shared<XTok>(sTokText);
                        
                    case eState::eArgState:
                    case eState::eCnctState:
                    case eState::eQtArgState:
                        break;
                }
            }
        }
    }
    catch( SyntaxExcept Ex)
    {
        //Discard any partial commands, arguments, or connectors.
        //DiscardResidue();
        
        throw Ex;
    }
    
}








void Lexer::UnGetToken(pBaseTok pToken)
{
    m_TokenList.push_front( pToken);
}








void Lexer::AddChars( string sInput)
{
    string sTemp;
    
    sTemp  = sInput;
    //sTemp += "\n ";
    //sTemp += END_OF_LINE;
    //sTemp += ' ';
    
    for(auto iter = sTemp.begin(); iter != sTemp.end(); iter++)
        m_CharList.push_back( *iter);
    
    m_nCharCount = 0;
    m_nLineCount = 0;
}








void Lexer::TokenizeCin()
{
    string   sInput, sOutput;
    eTokType eType = eTokType::eUnknownTok;
    
    while( eType != eTokType::eExitTok)
    {
        eType = eTokType::eUnknownTok;
        cout << "\n\nEnter text for lexer test.\n" << endl;
        
        sInput = "";
        getline( std::cin, sInput);
/*
        AddChars( sInput);
        
        try
        {
            cout << "\n\n";
            while( (eType != eTokType::eEndCharsTok) && (eType != eTokType::eExitTok))
            {
                pBaseTok pToken = GetNextToken();
                
                eType = pToken->TokenType();
            
                cout << pToken->AsString() << endl;
                cout << "            " << pToken->TokenText() << "\n" << endl;
            }
        }
        catch( SyntaxExcept Ex)
        {
            cout << "\n\nCaught SyntaxExcept...\n" << Ex.what() << endl;
        }
*/

        sOutput = "\n" + TokenizeString( sInput);
        cout << sOutput;
        if( sOutput.find( "\nExit") == 0)
            break;


    }
}








string Lexer::TokenizeString( string sInput)
{
    shared_ptr<BaseTok> pToken;
    string sReturn;
    
    AddChars( sInput);
    
    try
    {
        while( (pToken = GetNextToken())->TokenType() != eTokType::eEndCharsTok)
        {
            sReturn += pToken->AsString();
            sReturn += '\n';
        }
    }
    catch( SyntaxExcept Ex)
    {
        //cout << "\n\nCaught SyntaxExcept...\n" << Ex.what() << endl;
        sReturn  = "\n\nCaught SyntaxExcept...  ";
        sReturn += Ex.what();
        sReturn += '\n';
    }
    
    return sReturn;
}








void Lexer::Reset()
{
    m_eState = eState::eCmndState;
    //m_bIsPriorCharSemiColon = false;
    m_bPriorCloseParenthesis = false;
    m_nCharCount = 0;
    m_nLineCount = 0;
    m_nParenthesisCount = 0;
    m_CharList.clear();
    m_TokenList.clear();
}








void Lexer::DiscardResidue()
{
    //Discard any partial commands, arguments, or connectors.
    while( (CurrentChar() != ';') && (CurrentChar() != '\n') && (m_CharList.size() > 0)) 
        DiscardChar();

    //Discard the ';' or '\n' that remains. ( DiscardChar() does nothing if m_CharList.size() == 0 ).
    DiscardChar();

    m_eState = eState::eCmndState;
}















int Lexer::CurrentChar()
{
    if( m_CharList.size() == 0)
        return EOF;
    
    return m_CharList.front();
}








void Lexer::DiscardChar()
{
    if( m_CharList.size() > 0)
    {
        m_nCharCount++;
        if( m_CharList.front() == '\n')
        {
            m_nLineCount++;
            m_nCharCount = 0;
        }
/*        
        switch( m_CharList.front())
        {
            case ')':
                m_bPriorCloseParenthesis = true;
                break;
                
            case ']':
                m_bPriorCloseSqrBracket = true;
                
            case ' ':
            case '\t':
            case '|':
            case '&':
                break;
                
            default :
                m_bPriorCloseParenthesis = false;
        }
*/

        if( m_CharList.front() == ')')
        {
            m_bPriorCloseParenthesis = true;
            m_bPriorCloseSqrBracket  = false;
        }
        else if( m_CharList.front() == ']')
        {
            m_bPriorCloseParenthesis = false;
            m_bPriorCloseSqrBracket  = true;
        }
        else if( !(    m_CharList.front() == ' '
                    || m_CharList.front() == '\t'
                    || m_CharList.front() == '|'
                    || m_CharList.front() == '&'  ))
        {
            m_bPriorCloseParenthesis = false;
            m_bPriorCloseSqrBracket  = false;
        }

        m_CharList.pop_front();
    }

    

}









