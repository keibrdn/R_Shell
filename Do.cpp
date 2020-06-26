#include "Do.h"

#include "SyntaxExcept.h"

#include <sys/wait.h>
#include <unistd.h> 
#include <cstring>
#include <iostream>

#include <sys/types.h>
#include <sys/stat.h>




int SystemDo::Do()
{
    //throw ShellExcept("Attempt to call SystemDo::Do(), which should have been overriden in a derived class.");
    cout << "\n\n\nWARNING!!!!!!!!!!!!!!!!!!\nAttempt to execute a \"Do()\" that is empty!\n\n" << endl;
    
    return 0; //return "success".
}








void CmndDo::NewArrayMemory()
{
    if( m_pArray != nullptr)
        DeleteArrayMemory();


    m_pArray = new char*[ m_vec_pTokens.size() + 1];
    
    for(size_t i = 0; i <= m_vec_pTokens.size(); i++)
        if( i < m_vec_pTokens.size())
        {
            m_pArray[i] = new char[m_vec_pTokens.at(i)->TokenText().size()+1];
            strcpy(m_pArray[i], m_vec_pTokens.at(i)->TokenText().c_str());
        }
        else
            m_pArray[i] = nullptr;

}








void CmndDo::DeleteArrayMemory()
{
    if( m_pArray == nullptr)
        return;
    
    for( size_t i = 0 ; i <= m_vec_pTokens.size(); i++)
        if( i < m_vec_pTokens.size())
            delete[] m_pArray[i];
        else
        {
            delete[] m_pArray;
            m_pArray = nullptr;
        }
}







void CmndDo::Check4ValidCmnd()
{
    if( m_vec_pTokens.size() == 0)
        throw ShellExcept("In CmndDo::Check4ValidCmnd(), err... m_vec_pTokens.size() == 0.");
    
    if( m_vec_pTokens.front()->TokenType() != eTokType::eCmdTok)
        throw ShellExcept("In CmndDo::Check4ValidCmnd(), err... first element of m_vec_pTokens is not of type eCmdTok");
    
    for( size_t i = 1; i< m_vec_pTokens.size(); i++)
        if( (m_vec_pTokens.at(i)->TokenType() != eTokType::eArgTok) && (m_vec_pTokens.at(i)->TokenType() != eTokType::eQtArgTok))
            throw ShellExcept("In CmndDo::Check4ValidCmnd(), err... token not of type \"Arg\" or type \"QuotedArg\".");
}








int CmndDo::DoTest()
{
    string sFlag, sPath;
    struct stat FileInfo;
    int    nResult;
    
    if( (m_vec_pTokens.size() <= 1) || (m_vec_pTokens.size() > 3))
        throw ShellExcept("In CmndDo::DoTest(), wrong number of arguments.");
    
    if( m_vec_pTokens.front()->TokenText() != TEST_TEXT)
        throw ShellExcept( "In CmndDo::DoTest(), attempt to do command \"test\" on a command that is not \"test\".");

    if( m_vec_pTokens.size() == 3)
    {
        sFlag = m_vec_pTokens.at(1)->TokenText();
        sPath = m_vec_pTokens.at(2)->TokenText();
    }
    if( m_vec_pTokens.size() == 2)
    {
        sFlag = "-e";
        sPath = m_vec_pTokens.at(1)->TokenText();
    }
    
    
    
    nResult = stat( sPath.c_str(), &FileInfo);
    
    
    
    //if( nResult != 0)
    //{
    //    string sMesg = "\n\nAn error occured when doing \"stat()\".  perror status was... ";
    //    perror( sMesg.c_str());
    //    exit( EXIT_FAILURE);
    //}

    if( sFlag == "-e")
    {
        if( nResult == 0)
        {
            cout << "(True)\n";
            exit( EXIT_SUCCESS);
        }
        else
        {
            cout << "(False)\n";
            exit( EXIT_FAILURE);
        }
    }
    else if( sFlag == "-f")
    {
        if( (nResult == 0) && (S_ISREG( FileInfo.st_mode)))
        {
            cout << "(True)\n";
            exit( EXIT_SUCCESS);
        }
        else
        {
            cout << "(False)\n";
            exit( EXIT_FAILURE);
        }
    }
    else if( sFlag == "-d")
    {
        if( (nResult == 0) && (S_ISDIR( FileInfo.st_mode)))
        {
            cout << "(True)\n";
            exit( EXIT_SUCCESS);
        }
        else
        {
            cout << "(False)\n";
            exit( EXIT_FAILURE);
        }
    }
    else
    {
        cout << "\n\nIn Cmnd::DoTest(), invalid \"flag\" argument...must be a \"-e\", \"-f\", or \"-d\".\n";
        exit( EXIT_FAILURE);
    }
    
    
    return nResult;
}








CmndDo::~CmndDo()
{
    DeleteArrayMemory();
}








void CmndDo::PushBack(pBaseTok pToken)
{
    DeleteArrayMemory();
    
    if( m_vec_pTokens.size() == 0)
    {
        if( pToken->TokenType() != eTokType::eCmdTok)
            throw SyntaxExcept("In CmndDo::PushBack(), attempt to push_back() first token when that first token is not a command token.");
    }
    else
        if( (pToken->TokenType() != eTokType::eArgTok) && (pToken->TokenType() != eTokType::eQtArgTok))
            throw SyntaxExcept("In CmndDo::PushBack(), attempt to push_back() a token that is not an argument or quoted argument token.");

    m_vec_pTokens.push_back( pToken);
}








int CmndDo::Do()
{
    string ErrMesg;
    int    nStatus = 100;                              // = 100 so that with a debugger I can see when nStatus has changed.
    pid_t  nPid;
    
    Check4ValidCmnd();
    
    
    
    
    if( m_vec_pTokens.front()->TokenText() == EXIT_TEXT)
    {
        cout << "\n\nAdios Amigos!!\n\n";
        exit( EXIT_SUCCESS);
    }
    
    
    
    
    nPid = fork();
    
    if( nPid < 0)
    {
        ErrMesg  = "\n\n\"Fork\" failed when running the command \"";
        ErrMesg += AsString();
        ErrMesg += "\".  \"perror\" status was... ";
        
        perror( ErrMesg.c_str());
    }
    
    
    if( nPid == 0)                                   //This is the child process.
    {
        if( m_vec_pTokens.front()->TokenText() == TEST_TEXT)
            return DoTest();  //DoTest should not return..it should "exit()" which will terminate this child process.

    
        NewArrayMemory();

        //cout << "\n\n";
        //for( size_t i = 0; i < m_vec_pTokens.size(); i++)
        //{
        //    string sTemp = m_pArray[i];
        //    cout << sTemp << "  " << endl;
        //}
//        if( RSHELL_DEBUG)
//            cout << "\nChild: About to execvp()." << endl;
        
        execvp( *m_pArray, m_pArray);
        
//        if( RSHELL_DEBUG)
//            cout << "\nChild: Done with execvp()." << endl;
    
        DeleteArrayMemory();
        
        //execvp() should have taken control of this process, 
        //so we shouldn't get here.
        //But if we do get here, 
        //then an error has occured.
        ErrMesg  = "\n\n\"execvp\" failed when running the command \"";
        ErrMesg += AsString();
        ErrMesg += "\".  \"perror\" status was... ";
        
        perror( ErrMesg.c_str());
        exit( EXIT_FAILURE);

    }

//    if( RSHELL_DEBUG)
//        cout << "\nParent: About to wait, nStatus = " << nStatus << endl;

    //waitpid( (pid_t)-1, &nStatus, 0);                //If we get here, then this is the parent process. Wait here for child process to finish.
    //waitpid( nPid, &nStatus, 0);                //If we get here, then this is the parent process. Wait here for child process to finish.
    while( (waitpid( nPid, &nStatus, 0)) != nPid)                //If we get here, then this is the parent process. Wait here for child process to finish.
        ;
    
    
    
//    if( RSHELL_DEBUG)
//        cout << "\nParent: Done waiting, nStatus = " << nStatus << endl;
    
    
/*    
    //if( (nStatus != 0) || (nPid < 0))                //(nPid < 0) indicates that the fork() failed.
    if( nStatus != 0)
    {
        string ErrMesg;
        
        ErrMesg  = "\n\nError running command  \"";
        ErrMesg += AsString();
        ErrMesg += "\"\nError was due to... ";
        
        perror( ErrMesg.c_str());
        
        //return -1;
        return -1;
    }
*/
    return nStatus;
    //return 0;
}








string CmndDo::AsString()
{
    string sText;
    
    for( size_t i = 0; i < m_vec_pTokens.size(); i++)
    {
        sText += m_vec_pTokens.at(i)->TokenText();
        if( m_vec_pTokens.size() > (i + 1))
            sText += "  ";
    }
    
    return sText;
}







/*
int CmndDo::DoTest()
{
    int nResult = -1;
    
    Check4ValidCmnd();
    
    NewArrayMemory();
    
    cout << "\n\n";
    for( size_t i = 0; i < m_vec_pTokens.size(); i++)
    {
        string sTemp = m_pArray[i];
        cout << sTemp << "  " << endl;
    }
        
    
    DeleteArrayMemory();
    
    return nResult;
}
*/
















int OperatorDo::CmndCount()
{
    int nCount = 0;
    
    if( m_pLeft)
        nCount += m_pLeft->CmndCount();
    
    if( m_pRight)
        nCount += m_pRight->CmndCount();
    
    return nCount;
}









void OperatorDo::SetLeftPointer( pSystemDo pLeft)
{
    if( m_pLeft)
        throw ShellExcept("In OperatorDo::SetLeftPointer(), attempt to set a non null left pointer.");
    else
        m_pLeft = pLeft;
}









void OperatorDo::SetRightPointer(pSystemDo pRight)
{
    if( m_pRight)
        throw ShellExcept("In OperatorDo::SetRightPointer(), attempt to set a non null right pointer.");
    else
        m_pRight = pRight;
}
















int AndDo::Do()
{
    int nResult = m_pLeft->Do();
    if( nResult == 0)
        nResult = m_pRight->Do();
    
    return nResult;
}
















int OrDo::Do()
{
    int nResult = m_pLeft->Do();
    if( nResult != 0)
        nResult = m_pRight->Do();
    
    return nResult;
}
















int ParenDo::Do()
{
    int nResult = false;
    
    if( m_vecTree.size() == 0)
        return nResult;
    
    for( auto iter = m_vecTree.begin(); iter != m_vecTree.end(); iter++)
        nResult = (*iter)->Do();
    
    return nResult;
}








int ParenDo::CmndCount()
{
    int nTotal = 0;
    
    if( m_vecTree.size() == 0)
        return 0;
    
    for( auto iter = m_vecTree.begin(); iter != m_vecTree.end(); iter++)
        nTotal += (*iter)->CmndCount();
    
    return nTotal;
}














