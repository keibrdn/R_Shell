

#include "rshell.h"

#include "gtest/gtest.h"





TEST(TokenTest, Cmnd) 
{
    Lexer Lex;
    
    string sInput = "Cmd1\n";
    EXPECT_EQ("Command             Cmd1\nConnect             ;\n", Lex.TokenizeString( sInput));
}




TEST(TokenTest, SpaceCmndSpace) 
{
    Lexer Lex;
    
    string sInput = " Cmd1 \n";
    EXPECT_EQ("Command             Cmd1\nConnect             ;\n", Lex.TokenizeString( sInput));
}




TEST(TokenTest, CmndSemi) 
{
    Lexer Lex;
    
    string sInput = "Cmd1;\n";
    EXPECT_EQ("Command             Cmd1\nConnect             ;\n", Lex.TokenizeString( sInput));
}




TEST(TokenTest, CmndArg) 
{
    Lexer Lex;
    
    string sInput = "Cmd1 Arg1\n";
    EXPECT_EQ("Command             Cmd1\nArgument            Arg1\nConnect             ;\n", Lex.TokenizeString( sInput));
}




TEST(TokenTest, CmndArgSemi) 
{
    Lexer Lex;
    
    string sInput = "Cmd1 Arg1;\n";
    EXPECT_EQ("Command             Cmd1\nArgument            Arg1\nConnect             ;\n", Lex.TokenizeString( sInput));
}




TEST(TokenTest, CmndOrCmnd) 
{
    Lexer Lex;
    
    string sInput = "Cmd1 || Cmd2\n";
    EXPECT_EQ("Command             Cmd1\nConnect             ||\nCommand             Cmd2\nConnect             ;\n", Lex.TokenizeString( sInput));
}




TEST(TokenTest, CmndAndCmnd) 
{
    Lexer Lex;
    
    string sInput = "Cmd1 && Cmd2\n";
    EXPECT_EQ("Command             Cmd1\nConnect             &&\nCommand             Cmd2\nConnect             ;\n", Lex.TokenizeString( sInput));
}




TEST(TokenTest, CmndQuote) 
{
    Lexer Lex;
    
    string sInput = "Cmd1 \"Arg1 in quotes\"\n";
    EXPECT_EQ("Command             Cmd1\nQuoted Argument     Arg1 in quotes\nConnect             ;\n", Lex.TokenizeString( sInput));
}




TEST(TokenTest, CmndQuoteSemi) 
{
    Lexer Lex;
    
    string sInput = "Cmd1 \"A r g 1\";\n";
    EXPECT_EQ("Command             Cmd1\nQuoted Argument     A r g 1\nConnect             ;\n", Lex.TokenizeString( sInput));
}




TEST(TokenTest, CmndQuoteSpaceSemiSpace) 
{
    Lexer Lex;
    
    string sInput = "Cmd1 \"A r g 1\" ; \n";
    EXPECT_EQ("Command             Cmd1\nQuoted Argument     A r g 1\nConnect             ;\n", Lex.TokenizeString( sInput));
}




TEST(TokenTest, CmndQuoteOrCmndQuoteSpace) 
{
    Lexer Lex;
    
    string sInput = "Cmd1 \"A r g 1\" || Cmd2 \"A r g 2\" \n";
    EXPECT_EQ("Command             Cmd1\nQuoted Argument     A r g 1\nConnect             ||\nCommand             Cmd2\nQuoted Argument     A r g 2\nConnect             ;\n", Lex.TokenizeString( sInput));
}




TEST(TokenTest, AssignmentExample) 
{
    Lexer Lex;
    
    string sInput = "ls -a; echo hello && mkdir test || echo world; git status\n";
    EXPECT_EQ("Command             ls\nArgument            -a\nConnect             ;\nCommand             echo\nArgument            hello\n\
Connect             &&\nCommand             mkdir\nArgument            test\nConnect             ||\nCommand             echo\n\
Argument            world\nConnect             ;\nCommand             git\nArgument            status\nConnect             ;\n", Lex.TokenizeString( sInput));
}






int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}



