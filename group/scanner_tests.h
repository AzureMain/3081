#include <cxxtest/TestSuite.h>

#include <stdio.h>
#include "include/read_input.h"
#include "include/scanner.h"

using namespace std;
namespace scanner = fcal::scanner;
class ScannerTestSuite : public CxxTest::TestSuite
{
public:
    /* A Scanner object is created in the test_setup_code method.
       This is a test, beginning with "test_", so that it is executed
       by the testing framework.  The scanner is used in some tests of
       the method "scan".
     */
    scanner::Scanner *s;
    void test_setup_code () {
      s = new scanner::Scanner();
    }


    // Tests for components and functions used by "scan"
    // --------------------------------------------------

    /* You should test the regular expressions you use to make and match
       regular expressions,and the functions used by makeRegex and
       matchRegex regex_tests.h.
       However, You will likely need to write several tests to
       adequately test  the functions that are called from "scan".
       Once you are confident that the components used by "scan"
       work properly, then you can run tests on the "scan" method itself.
    */

    // You should place tests for these other functions used by
    // scan below.

    // Test accessors and mutators for token class

    void test_token_terminal() {
      scanner::Token *t = new scanner::Token();
      t->set_terminal(scanner::kIntConst);
      TS_ASSERT_EQUALS(t->terminal(), scanner::kIntConst);
    }

    void test_token_lexeme() {
      scanner::Token *t = new scanner::Token();
      t->set_lexeme("test");
      TS_ASSERT_EQUALS("test", t->lexeme());
    }

    void test_token_next() {
      scanner::Token *t = new scanner::Token();
      scanner::Token *u = new scanner::Token();
      t->set_next(u);
      TS_ASSERT_EQUALS(t->next(), u);
    }

    /* Below is one of the tests for these components in the project
       solution created by your instructor.  It uses a helper
       function function called "tokenMaker_tester", which you have
       not been given.  You are expected to design your own components
       for "scan" and your own mechanisms for easily testing them.

        void xtest_TokenMaker_leftCurly () {
            tokenMaker_tester ("{ ", "^\\{", leftCurly, "{");
        }

        Note that this test is here named "xtest_Token..." The leading
        "x" is so that cxxTest doesn't scan the line above and think
        it is an actual test that isn't commented out.  cxxTest is
        very simple and doesn't even process block comments.
    */


    /* You must have at least one separate test case for each terminal
       symbol.  Thus, you need a test that will pass or fail based
       solely on the regular expression (and its corresponding code)
       for each terminal symbol.

       This requires a test case for each element of the enumerated
       type tokenType.  This may look something like the sample test
       shown in the comment above.
    */



    // Tests for "scan"
    // --------------------------------------------------

    /* Below are some helper functions and sample tests for testing the
       "scan" method on Scanner.
    */

    // Test that a list of tokens has no lexicalError tokens.
    bool noLexicalErrors (scanner::Token *tks) {
        scanner::Token *currentToken = tks;
        while (currentToken != NULL) {
          if (currentToken->terminal() == scanner::kLexicalError) {
              printf("problem: %s\n",currentToken->lexeme().c_str());
              fflush(stdout);
                                return false;
            }
            else {
              currentToken = currentToken->next();
            }
        }
        return true;
    }

    /* A quick, but inaccurate, test for scanning files.  It only
       checks that no lexical errors occurred, not that the right
       tokens were returned.
    */
    void scanFileNoLexicalErrors(const char* filename) {
        char *text =  scanner::ReadInputFromFile(filename);
        TS_ASSERT (text);
        scanner::Token *tks = s->Scan(text);
        TS_ASSERT(tks != NULL);
        TS_ASSERT(noLexicalErrors(tks));
    }


    /* This function checks that the terminal fields in the list of
       tokens matches a list of terminals.
    */
  bool sameTerminals(scanner::Token *tks, int numTerms,
                     scanner::TokenType *ts) {
        scanner::Token *currentToken = tks;
        int termIndex = 0;
        while(currentToken != NULL && termIndex < numTerms) {

            if(currentToken->terminal() != ts[termIndex]) {
                printf("item at index: %i: terminal: %i should be terminal: %i\n",termIndex,currentToken->terminal(), ts[termIndex]);
				fflush(stdout);
                return false;
            }
            else {
                ++ termIndex;
                currentToken = currentToken->next();
            }
        }
        return true;
    }


    /* Below are the provided test files that your code should also
       pass.

       You may initially want to rename these tests to "xtest_..." so
       that the CxxTest framework does not see it as a test and won't
       run it as one.  This way you can focus on the tests that you'll
       write above for the individual terminal types first. Then focus
       on these tests.

    */

    // The "endOfFile" token is always the last one in the list of tokens.
    void test_scan_empty() {
        scanner::Token *tks = s->Scan("  ");
        TS_ASSERT(tks != NULL);
        TS_ASSERT_EQUALS(tks->terminal(), scanner::kEndOfFile);
        TS_ASSERT(tks->next() == NULL);
    }

    void test_scan_empty_comment() {
        scanner::Token *tks = s->Scan(" /* a comment */ ");
        TS_ASSERT(tks != NULL);
        TS_ASSERT_EQUALS(tks->terminal(), scanner::kEndOfFile);
        TS_ASSERT(tks->next() == NULL);
    }

    // When a lexical error occurs, the scanner creates a token with a
    // single-character lexeme and lexicalError as the terminal.
   void test_scan_lexicalErrors() {
        scanner::Token *tks = s->Scan("$&1  ");
        TS_ASSERT(tks != NULL);
        TS_ASSERT_EQUALS(tks->terminal(), scanner::kLexicalError);
        TS_ASSERT_EQUALS(tks->lexeme(), "$");
        tks = tks->next();
        TS_ASSERT(tks != NULL);
        TS_ASSERT_EQUALS(tks->terminal(), scanner::kLexicalError);
        TS_ASSERT_EQUALS(tks->lexeme(), "&");
        tks = tks->next();
        TS_ASSERT(tks != NULL);
        TS_ASSERT_EQUALS(tks->terminal(), scanner::kIntConst);
        TS_ASSERT_EQUALS(tks->lexeme(), "1");
        tks = tks->next();
        TS_ASSERT(tks != NULL);
        TS_ASSERT_EQUALS(tks->terminal(), scanner::kEndOfFile);
        TS_ASSERT(tks->next() == NULL);
    }


    // A test for scanning numbers and a variable.

    void test_scan_nums_vars() {
        scanner::Token *tks = s->Scan(" 123 x 12.34 ");
        TS_ASSERT(tks != NULL);
        scanner::TokenType ts[] = { scanner::kIntConst, scanner::kVariableName,
                                           scanner::kFloatConst, scanner::kEndOfFile };
        TS_ASSERT( sameTerminals( tks, 4, ts));
    }


    /* This test checks that the scanner returns a list of tokens with
       the correct terminal fields.  It doesn't check that the lexemes
       are correct.
     */

    void test_scan_bad_syntax_good_tokens() {
        const char *filename = "./samples/bad_syntax_good_tokens.dsl";
        char *text =  scanner::ReadInputFromFile(filename);
        TS_ASSERT(text);
        scanner::Token *tks = NULL;
        tks = s->Scan( text);
        TS_ASSERT(tks != NULL);
        scanner::TokenType ts[] = {
          scanner::kIntConst, scanner::kIntConst, scanner::kIntConst, scanner::kIntConst,
          scanner::kStringConst, scanner::kStringConst, scanner::kStringConst,
          scanner::kFloatConst, scanner::kFloatConst, scanner::kFloatConst,
          scanner::kMatrixKwd,
          scanner::kSemiColon, scanner::kColon, scanner::kToKwd,
          scanner::kLeftCurly, scanner::kLeftParen, scanner::kRightCurly, scanner::kRightParen,
          scanner::kPlusSign, scanner::kStar, scanner::kDash, scanner::kForwardSlash,
          scanner::kEqualsEquals, scanner::kLessThanEqual,
          scanner::kGreaterThanEqual, scanner::kNotEquals,
          scanner::kAssign,
          scanner::kVariableName, scanner::kVariableName, scanner::kVariableName, scanner::kVariableName,
          scanner::kVariableName, scanner::kVariableName, scanner::kVariableName,
          scanner::kIntKwd, scanner::kFloatKwd, scanner::kStringKwd,
          scanner::kEndOfFile
        };
        int count = 38;
        TS_ASSERT( sameTerminals( tks, count, ts));
    }

    void test_scan_sample_forestLoss() {
        scanFileNoLexicalErrors("./samples/forest_loss_v2.dsl");
    }

    void test_kIntKwd(){
    scanner::Token *tks = s->Scan("int");
    TS_ASSERT(tks != NULL);
    TS_ASSERT_EQUALS(tks->get_terminal(), scanner::kIntKwd);
    TS_ASSERT_EQUALS(tks->get_lexeme(), "int")
    tks = tks->get_next();
    TS_ASSERT(tks != NULL);
    TS_ASSERT_EQUALS(tks->get_terminal(), scanner::kEndOfFile)
    tks = tks ->get_next();
    TS_ASSERT(tks==NULL);
    }

    void test_kFloatKwd(){
    scanner::Token *tks = s->Scan("float");
    TS_ASSERT(tks != NULL);
    TS_ASSERT_EQUALS(tks->get_terminal(), scanner::kFloatKwd);
    TS_ASSERT_EQUALS(tks->get_lexeme(), "float")
    tks = tks->get_next();
    TS_ASSERT(tks != NULL);
    TS_ASSERT_EQUALS(tks->get_terminal(), scanner::kEndOfFile)
    tks = tks ->get_next();
    TS_ASSERT(tks==NULL);
    }

    void test_kStringKwd(){
    scanner::Token *tks = s->Scan("string");
    TS_ASSERT(tks != NULL);
    TS_ASSERT_EQUALS(tks->get_terminal(), scanner::kStringKwd);
    TS_ASSERT_EQUALS(tks->get_lexeme(), "string")
    tks = tks->get_next();
    TS_ASSERT(tks != NULL);
    TS_ASSERT_EQUALS(tks->get_terminal(), scanner::kEndOfFile)
    tks = tks ->get_next();
    TS_ASSERT(tks==NULL);
    }

    void test_kMatrixKwd(){
    scanner::Token *tks = s->Scan("matrix");
    TS_ASSERT(tks != NULL);
    TS_ASSERT_EQUALS(tks->get_terminal(), scanner::kMatrixKwd);
    TS_ASSERT_EQUALS(tks->get_lexeme(), "matrix")
    tks = tks->get_next();
    TS_ASSERT(tks != NULL);
    TS_ASSERT_EQUALS(tks->get_terminal(), scanner::kEndOfFile)
    tks = tks ->get_next();
    TS_ASSERT(tks==NULL);
    }

    void test_kLetKwd(){
    scanner::Token *tks = s->Scan("let");
    TS_ASSERT(tks != NULL);
    TS_ASSERT_EQUALS(tks->get_terminal(), scanner::kLetKwd);
    TS_ASSERT_EQUALS(tks->get_lexeme(), "let")
    tks = tks->get_next();
    TS_ASSERT(tks != NULL);
    TS_ASSERT_EQUALS(tks->get_terminal(), scanner::kEndOfFile)
    tks = tks ->get_next();
    TS_ASSERT(tks==NULL);
    }

    void test_kInKwd(){
    scanner::Token *tks = s->Scan("in");
    TS_ASSERT(tks != NULL);
    TS_ASSERT_EQUALS(tks->get_terminal(), scanner::kInKwd);
    TS_ASSERT_EQUALS(tks->get_lexeme(), "in")
    tks = tks->get_next();
    TS_ASSERT(tks != NULL);
    TS_ASSERT_EQUALS(tks->get_terminal(), scanner::kEndOfFile)
    tks = tks ->get_next();
    TS_ASSERT(tks==NULL);
    }

    void test_kEndKwd(){
    scanner::Token *tks = s->Scan("end");
    TS_ASSERT(tks != NULL);
    TS_ASSERT_EQUALS(tks->get_terminal(), scanner::kEndKwd);
    TS_ASSERT_EQUALS(tks->get_lexeme(), "end")
    tks = tks->get_next();
    TS_ASSERT(tks != NULL);
    TS_ASSERT_EQUALS(tks->get_terminal(), scanner::kEndOfFile)
    tks = tks ->get_next();
    TS_ASSERT(tks==NULL);
    }

    void test_kIfKwd(){
    scanner::Token *tks = s->Scan("if");
    TS_ASSERT(tks != NULL);
    TS_ASSERT_EQUALS(tks->get_terminal(), scanner::kIfKwd);
    TS_ASSERT_EQUALS(tks->get_lexeme(), "if")
    tks = tks->get_next();
    TS_ASSERT(tks != NULL);
    TS_ASSERT_EQUALS(tks->get_terminal(), scanner::kEndOfFile)
    tks = tks ->get_next();
    TS_ASSERT(tks==NULL);
    }

    void test_kThenKwd(){
    scanner::Token *tks = s->Scan("then");
    TS_ASSERT(tks != NULL);
    TS_ASSERT_EQUALS(tks->get_terminal(), scanner::kThenKwd);
    TS_ASSERT_EQUALS(tks->get_lexeme(), "then")
    tks = tks->get_next();
    TS_ASSERT(tks != NULL);
    TS_ASSERT_EQUALS(tks->get_terminal(), scanner::kEndOfFile)
    tks = tks ->get_next();
    TS_ASSERT(tks==NULL);
    }

    void test_kElseKwd(){
    scanner::Token *tks = s->Scan("else");
    TS_ASSERT(tks != NULL);
    TS_ASSERT_EQUALS(tks->get_terminal(), scanner::kElseKwd);
    TS_ASSERT_EQUALS(tks->get_lexeme(), "else")
    tks = tks->get_next();
    TS_ASSERT(tks != NULL);
    TS_ASSERT_EQUALS(tks->get_terminal(), scanner::kEndOfFile)
    tks = tks ->get_next();
    TS_ASSERT(tks==NULL);
    }

    void test_kRepeatKwd(){
    scanner::Token *tks = s->Scan("repeat");
    TS_ASSERT(tks != NULL);
    TS_ASSERT_EQUALS(tks->get_terminal(), scanner::kRepeatKwd);
    TS_ASSERT_EQUALS(tks->get_lexeme(), "repeat")
    tks = tks->get_next();
    TS_ASSERT(tks != NULL);
    TS_ASSERT_EQUALS(tks->get_terminal(), scanner::kEndOfFile)
    tks = tks ->get_next();
    TS_ASSERT(tks==NULL);
    }

    void test_kPrintKwd(){
    scanner::Token *tks = s->Scan("print");
    TS_ASSERT(tks != NULL);
    TS_ASSERT_EQUALS(tks->get_terminal(), scanner::kPrintKwd);
    TS_ASSERT_EQUALS(tks->get_lexeme(), "print")
    tks = tks->get_next();
    TS_ASSERT(tks != NULL);
    TS_ASSERT_EQUALS(tks->get_terminal(), scanner::kEndOfFile)
    tks = tks ->get_next();
    TS_ASSERT(tks==NULL);
    }

    void test_kToKwd(){
    scanner::Token *tks = s->Scan("to");
    TS_ASSERT(tks != NULL);
    TS_ASSERT_EQUALS(tks->get_terminal(), scanner::kToKwd);
    TS_ASSERT_EQUALS(tks->get_lexeme(), "to")
    tks = tks->get_next();
    TS_ASSERT(tks != NULL);
    TS_ASSERT_EQUALS(tks->get_terminal(), scanner::kEndOfFile)
    tks = tks ->get_next();
    TS_ASSERT(tks==NULL);
    }

    void test_kIntConst(){
    scanner::Token *tks = s->Scan("1234567890");
    TS_ASSERT(tks != NULL);
    TS_ASSERT_EQUALS(tks->get_terminal(), scanner::kIntConst);
    TS_ASSERT_EQUALS(tks->get_lexeme(), "1234567890")
    tks = tks->get_next();
    TS_ASSERT(tks != NULL);
    TS_ASSERT_EQUALS(tks->get_terminal(), scanner::kEndOfFile)
    tks = tks ->get_next();
    TS_ASSERT(tks==NULL);
    }

    void test_kFloatConst(){
    scanner::Token *tks = s->Scan("1234567890.123");
    TS_ASSERT(tks != NULL);
    TS_ASSERT_EQUALS(tks->get_terminal(), scanner::kFloatConst);
    TS_ASSERT_EQUALS(tks->get_lexeme(), "1234567890.123")
    tks = tks->get_next();
    TS_ASSERT(tks != NULL);
    TS_ASSERT_EQUALS(tks->get_terminal(), scanner::kEndOfFile)
    tks = tks ->get_next();
    TS_ASSERT(tks==NULL);
    }
// might be wrong here
    void test_kStringConst(){
    scanner::Token *tks = s->Scan("whosyourdaddy");
    TS_ASSERT(tks != NULL);
    TS_ASSERT_EQUALS(tks->get_terminal(), scanner::kStringConst);
    TS_ASSERT_EQUALS(tks->get_lexeme(), "whosyourdaddy")
    tks = tks->get_next();
    TS_ASSERT(tks != NULL);
    TS_ASSERT_EQUALS(tks->get_terminal(), scanner::kEndOfFile)
    tks = tks ->get_next();
    TS_ASSERT(tks==NULL);
    }

    void test_kVariableName(){
    scanner::Token *tks = s->Scan("whosyourdaddy");
    TS_ASSERT(tks != NULL);
    TS_ASSERT_EQUALS(tks->get_terminal(), scanner::kVariableName);
    TS_ASSERT_EQUALS(tks->get_lexeme(), "whosyourdaddy")
    tks = tks->get_next();
    TS_ASSERT(tks != NULL);
    TS_ASSERT_EQUALS(tks->get_terminal(), scanner::kEndOfFile)
    tks = tks ->get_next();
    TS_ASSERT(tks==NULL);
    }

    void test_kLeftParen(){
    scanner::Token *tks = s->Scan("(");
    TS_ASSERT(tks != NULL);
    TS_ASSERT_EQUALS(tks->get_terminal(), scanner::kLeftParen);
    TS_ASSERT_EQUALS(tks->get_lexeme(), "(")
    tks = tks->get_next();
    TS_ASSERT(tks != NULL);
    TS_ASSERT_EQUALS(tks->get_terminal(), scanner::kEndOfFile)
    tks = tks ->get_next();
    TS_ASSERT(tks==NULL);
    }

    void test_kRightParen(){
    scanner::Token *tks = s->Scan(")");
    TS_ASSERT(tks != NULL);
    TS_ASSERT_EQUALS(tks->get_terminal(), scanner::kRightParen);
    TS_ASSERT_EQUALS(tks->get_lexeme(), ")")
    tks = tks->get_next();
    TS_ASSERT(tks != NULL);
    TS_ASSERT_EQUALS(tks->get_terminal(), scanner::kEndOfFile)
    tks = tks ->get_next();
    TS_ASSERT(tks==NULL);
    }

    void test_kLeftCurly(){
    scanner::Token *tks = s->Scan("{");
    TS_ASSERT(tks != NULL);
    TS_ASSERT_EQUALS(tks->get_terminal(), scanner::kLeftCurly);
    TS_ASSERT_EQUALS(tks->get_lexeme(), "{")
    tks = tks->get_next();
    TS_ASSERT(tks != NULL);
    TS_ASSERT_EQUALS(tks->get_terminal(), scanner::kEndOfFile)
    tks = tks ->get_next();
    TS_ASSERT(tks==NULL);
    }

    void test_kRightCurly(){
    scanner::Token *tks = s->Scan("}");
    TS_ASSERT(tks != NULL);
    TS_ASSERT_EQUALS(tks->get_terminal(), scanner::kRightCurly);
    TS_ASSERT_EQUALS(tks->get_lexeme(), "}")
    tks = tks->get_next();
    TS_ASSERT(tks != NULL);
    TS_ASSERT_EQUALS(tks->get_terminal(), scanner::kEndOfFile)
    tks = tks ->get_next();
    TS_ASSERT(tks==NULL);
    }

    void test_kLeftSquare(){
    scanner::Token *tks = s->Scan("[");
    TS_ASSERT(tks != NULL);
    TS_ASSERT_EQUALS(tks->get_terminal(), scanner::kLeftSquare);
    TS_ASSERT_EQUALS(tks->get_lexeme(), "[")
    tks = tks->get_next();
    TS_ASSERT(tks != NULL);
    TS_ASSERT_EQUALS(tks->get_terminal(), scanner::kEndOfFile)
    tks = tks ->get_next();
    TS_ASSERT(tks==NULL);
    }

    void test_kRightSquare(){
    scanner::Token *tks = s->Scan("]");
    TS_ASSERT(tks != NULL);
    TS_ASSERT_EQUALS(tks->get_terminal(), scanner::kRightSquare);
    TS_ASSERT_EQUALS(tks->get_lexeme(), "]")
    tks = tks->get_next();
    TS_ASSERT(tks != NULL);
    TS_ASSERT_EQUALS(tks->get_terminal(), scanner::kEndOfFile)
    tks = tks ->get_next();
    TS_ASSERT(tks==NULL);
    }

    void test_kSemiColon(){
    scanner::Token *tks = s->Scan(";");
    TS_ASSERT(tks != NULL);
    TS_ASSERT_EQUALS(tks->get_terminal(), scanner::kSemiColon);
    TS_ASSERT_EQUALS(tks->get_lexeme(), ";")
    tks = tks->get_next();
    TS_ASSERT(tks != NULL);
    TS_ASSERT_EQUALS(tks->get_terminal(), scanner::kEndOfFile)
    tks = tks ->get_next();
    TS_ASSERT(tks==NULL);
    }

    void test_kColon(){
    scanner::Token *tks = s->Scan(":");
    TS_ASSERT(tks != NULL);
    TS_ASSERT_EQUALS(tks->get_terminal(), scanner::kColon);
    TS_ASSERT_EQUALS(tks->get_lexeme(), ":")
    tks = tks->get_next();
    TS_ASSERT(tks != NULL);
    TS_ASSERT_EQUALS(tks->get_terminal(), scanner::kEndOfFile)
    tks = tks ->get_next();
    TS_ASSERT(tks==NULL);
    }

    void test_kAssign(){
    scanner::Token *tks = s->Scan("=");
    TS_ASSERT(tks != NULL);
    TS_ASSERT_EQUALS(tks->get_terminal(), scanner::kAssign);
    TS_ASSERT_EQUALS(tks->get_lexeme(), "=")
    tks = tks->get_next();
    TS_ASSERT(tks != NULL);
    TS_ASSERT_EQUALS(tks->get_terminal(), scanner::kEndOfFile)
    tks = tks ->get_next();
    TS_ASSERT(tks==NULL);
    }

    void test_kPlusSign(){
    scanner::Token *tks = s->Scan("+");
    TS_ASSERT(tks != NULL);
    TS_ASSERT_EQUALS(tks->get_terminal(), scanner::kPlusSign);
    TS_ASSERT_EQUALS(tks->get_lexeme(), "+")
    tks = tks->get_next();
    TS_ASSERT(tks != NULL);
    TS_ASSERT_EQUALS(tks->get_terminal(), scanner::kEndOfFile)
    tks = tks ->get_next();
    TS_ASSERT(tks==NULL);
    }

    void test_kStar(){
    scanner::Token *tks = s->Scan("*");
    TS_ASSERT(tks != NULL);
    TS_ASSERT_EQUALS(tks->get_terminal(), scanner::kStar);
    TS_ASSERT_EQUALS(tks->get_lexeme(), "*")
    tks = tks->get_next();
    TS_ASSERT(tks != NULL);
    TS_ASSERT_EQUALS(tks->get_terminal(), scanner::kEndOfFile)
    tks = tks ->get_next();
    TS_ASSERT(tks==NULL);
    }

    void test_kDash(){
    scanner::Token *tks = s->Scan("-");
    TS_ASSERT(tks != NULL);
    TS_ASSERT_EQUALS(tks->get_terminal(), scanner::kDash);
    TS_ASSERT_EQUALS(tks->get_lexeme(), "-")
    tks = tks->get_next();
    TS_ASSERT(tks != NULL);
    TS_ASSERT_EQUALS(tks->get_terminal(), scanner::kEndOfFile)
    tks = tks ->get_next();
    TS_ASSERT(tks==NULL);
    }

    void test_kForwardSlash(){
    scanner::Token *tks = s->Scan("/");
    TS_ASSERT(tks != NULL);
    TS_ASSERT_EQUALS(tks->get_terminal(), scanner::kForwardSlash);
    TS_ASSERT_EQUALS(tks->get_lexeme(), "/")
    tks = tks->get_next();
    TS_ASSERT(tks != NULL);
    TS_ASSERT_EQUALS(tks->get_terminal(), scanner::kEndOfFile)
    tks = tks ->get_next();
    TS_ASSERT(tks==NULL);
    }

    void test_kLessThan(){
    scanner::Token *tks = s->Scan("<");
    TS_ASSERT(tks != NULL);
    TS_ASSERT_EQUALS(tks->get_terminal(), scanner::kLessThan);
    TS_ASSERT_EQUALS(tks->get_lexeme(), "<")
    tks = tks->get_next();
    TS_ASSERT(tks != NULL);
    TS_ASSERT_EQUALS(tks->get_terminal(), scanner::kEndOfFile)
    tks = tks ->get_next();
    TS_ASSERT(tks==NULL);
    }

    void test_kLessThanEqual(){
    scanner::Token *tks = s->Scan("<=");
    TS_ASSERT(tks != NULL);
    TS_ASSERT_EQUALS(tks->get_terminal(), scanner::kLessThanEqual);
    TS_ASSERT_EQUALS(tks->get_lexeme(), "<=")
    tks = tks->get_next();
    TS_ASSERT(tks != NULL);
    TS_ASSERT_EQUALS(tks->get_terminal(), scanner::kEndOfFile)
    tks = tks ->get_next();
    TS_ASSERT(tks==NULL);
    }

    void test_kGreaterThan(){
    scanner::Token *tks = s->Scan(">");
    TS_ASSERT(tks != NULL);
    TS_ASSERT_EQUALS(tks->get_terminal(), scanner::kGreaterThan);
    TS_ASSERT_EQUALS(tks->get_lexeme(), ">")
    tks = tks->get_next();
    TS_ASSERT(tks != NULL);
    TS_ASSERT_EQUALS(tks->get_terminal(), scanner::kEndOfFile)
    tks = tks ->get_next();
    TS_ASSERT(tks==NULL);
    }

    void test_kGreaterThanEqual(){
    scanner::Token *tks = s->Scan(">=");
    TS_ASSERT(tks != NULL);
    TS_ASSERT_EQUALS(tks->get_terminal(), scanner::kGreaterThanEqual);
    TS_ASSERT_EQUALS(tks->get_lexeme(), ">=")
    tks = tks->get_next();
    TS_ASSERT(tks != NULL);
    TS_ASSERT_EQUALS(tks->get_terminal(), scanner::kEndOfFile)
    tks = tks ->get_next();
    TS_ASSERT(tks==NULL);
    }

    void test_kEqualsEquals(){
    scanner::Token *tks = s->Scan("==");
    TS_ASSERT(tks != NULL);
    TS_ASSERT_EQUALS(tks->get_terminal(), scanner::kEqualsEquals);
    TS_ASSERT_EQUALS(tks->get_lexeme(), "==")
    tks = tks->get_next();
    TS_ASSERT(tks != NULL);
    TS_ASSERT_EQUALS(tks->get_terminal(), scanner::kEndOfFile)
    tks = tks ->get_next();
    TS_ASSERT(tks==NULL);
    }

    void test_kNotEquals(){
    scanner::Token *tks = s->Scan("!=");
    TS_ASSERT(tks != NULL);
    TS_ASSERT_EQUALS(tks->get_terminal(), scanner::kNotEquals);
    TS_ASSERT_EQUALS(tks->get_lexeme(), "!=")
    tks = tks->get_next();
    TS_ASSERT(tks != NULL);
    TS_ASSERT_EQUALS(tks->get_terminal(), scanner::kEndOfFile)
    tks = tks ->get_next();
    TS_ASSERT(tks==NULL);
    }

    void test_kAndOp(){
    scanner::Token *tks = s->Scan("&&");
    TS_ASSERT(tks != NULL);
    TS_ASSERT_EQUALS(tks->get_terminal(), scanner::kAndOp);
    TS_ASSERT_EQUALS(tks->get_lexeme(), "&&")
    tks = tks->get_next();
    TS_ASSERT(tks != NULL);
    TS_ASSERT_EQUALS(tks->get_terminal(), scanner::kEndOfFile)
    tks = tks ->get_next();
    TS_ASSERT(tks==NULL);
    }

    void test_kOrOp(){
    scanner::Token *tks = s->Scan("||");
    TS_ASSERT(tks != NULL);
    TS_ASSERT_EQUALS(tks->get_terminal(), scanner::kOrOp);
    TS_ASSERT_EQUALS(tks->get_lexeme(), "||")
    tks = tks->get_next();
    TS_ASSERT(tks != NULL);
    TS_ASSERT_EQUALS(tks->get_terminal(), scanner::kEndOfFile)
    tks = tks ->get_next();
    TS_ASSERT(tks==NULL);
    }

    void test_kNotOp(){
    scanner::Token *tks = s->Scan("!");
    TS_ASSERT(tks != NULL);
    TS_ASSERT_EQUALS(tks->get_terminal(), scanner::kNotOp);
    TS_ASSERT_EQUALS(tks->get_lexeme(), "!")
    tks = tks->get_next();
    TS_ASSERT(tks != NULL);
    TS_ASSERT_EQUALS(tks->get_terminal(), scanner::kEndOfFile)
    tks = tks ->get_next();
    TS_ASSERT(tks==NULL);
    }
};
