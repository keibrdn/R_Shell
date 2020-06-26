#include "rshell.h"

#include <iostream>






void rshell::RunShell()
{
    Parser ShellParser;
    //int    nCount;
    
    cout << "\n\n\n\nStarting RSHELL...\n\n";
    
    while(true)
    {
        pSystemDo pSysDo;
        string    sInput;
        
        ShellParser.Reset();
        cout << "\n\n" << RSHELL_PROMPT << std::flush;
        sInput = "";
        getline( std::cin, sInput);
        ShellParser.AddChars(sInput);
        
        
        pSysDo = ShellParser.GetNextParenthesis();
        pSysDo->Do();
/*
        do
        {
            if( RSHELL_DEBUG)
                cout << "\n******** Doing the root Do() now ********\n";
            
            pSysDo->Do();
            pSysDo = ShellParser.GetNextDo();
            nCount = pSysDo->CmndCount();
        }
        while( nCount > 0);
*/
    }
}
















