%{
#include <iostream>
#include <string>
#include <cstring>

#include "tape.h"
#include "code.h"
#include "test.h"
#include "state.h"
#include "symbol.h"
#include "action.h"
#include "parser.tab.h"
  
using std::cout;
using std::endl;
using std::string;

extern "C" int yylex();

%}

%%

[ \n\t]   ;

":"      { cout << "COLON " << yytext[0] << endl; return yytext[0]; }
"---"    { cout << "OPEN" << endl; return OPEN; }
"..."    { cout << "CLOSE" << endl; return CLOSE; }
"code:"  { cout << "CODE" << endl; return CODE; }
"tapes:" { cout << "TAPES" << endl; return TAPES; }
"start"  { cout << "START" << endl; return START; }
"write"  { cout << "WRITE" << endl; return WRITE; }
"move"   { cout << "MOVE" << endl; return MOVE; }
"state"  { cout << "STATE" << endl; return STATE; }
"right"  { cout << "RIGHT" << endl; return RIGHT; }
"left"   { cout << "LEFT" << endl; return LEFT; }

[a-zA-Z]+[a-zA-Z0-9_-]* {
  cout << "ID_STATE" << endl;
  yylval.stringval = new string(yytext);
  return ID_STATE;
}
[0-9]+[0-9]* {
  cout << "ID_TAPE" << endl;
  yylval.stringval = new string(yytext);
  return ID_TAPE;
}
'[^\n\t]{1}' {
  cout << "SYMBOL" << endl;
  yylval.charval = yytext[1];
  return SYMBOL;
}
'[^\n]+' {
  cout << "TAPE len: " << strlen(yytext) << endl;
  yylval.stringval = new string(yytext);
  return TAPE;
}

.     { cout << "WARNING: Character " << yytext << " has been ignored." << endl; }

%%

