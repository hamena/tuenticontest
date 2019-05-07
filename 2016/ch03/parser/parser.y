%{

// C++ code here.

#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include <iostream>

#define FILE_NAME "testInput.txt"

#include "tape.h"
#include "code.h"
#include "test.h"
#include "state.h"
#include "symbol.h"
#include "action.h"
  
extern "C" int yylex();
//extern "C" int yyparse();
extern "C" FILE *yyin;
  
void yyerror(const char *s);
  
%}

// Declare union
%union {
  char charval;
  std::string* stringval;
  Tape* tapeval;
  Tapes* vtapeval;
  Action* actionval;
  Symbols* vsymbolval;
  State* stateval;
  States* vstateval;
  Test* testval;
  Code* codeval;
}

// Declare start rule
%start start

// Declare tokens
%token OPEN CLOSE CODE TAPES WRITE MOVE STATE START
%token <stringval> ID_STATE ID_TAPE TAPE RIGHT LEFT
%token <charval> SYMBOL
			
// Declare rule types
%type <testval> test
%type <codeval> code
%type <vstateval> states
%type <stateval> state start_state
%type <vsymbolval> symbols
%type <actionval> actions move_action state_action
%type <vtapeval> tapes tapes_rec
%type <tapeval> tape

%%

// RULES here

start:		OPEN test CLOSE { $2->execute(); }
	;

test:		code tapes { $$ = new Test($1,$2); }
	;

code:		CODE start_state states { $$ = new Code($2,$3); }
	;

states:	        state states
		{
		  States aux(1,$1);
		  $2->insert($2->begin(),aux.begin(),aux.end());
		  aux[0] = nullptr;
		  $$ = $2;
		}
	|	{ $$ = new States(1,new NullState()); }
	;

state:		ID_STATE ':' symbols { $$ = new State(*$1,$3); }
	;

symbols:	SYMBOL ':' actions symbols
		{

		  Symbols aux(1,new Symbol($1,$3));
		  $4->insert($4->begin(),aux.begin(),aux.end());
		  aux[0] = nullptr;
		  $$ = $4;
		}
	|	{ $$ = new Symbols(1, new NullSymbol()); }
	;

actions:	WRITE ':' SYMBOL move_action { $$ = new WriteAction($3,$4); }
	|	move_action { $$ = new NoAction(); }
	;

move_action:	MOVE ':' RIGHT state_action { $$ = new MoveAction(RIGHT_DIRECTION,$4); }
	|	MOVE ':' LEFT state_action { $$ = new MoveAction(LEFT_DIRECTION,$4); }
	|	state_action { $$ = new NoAction(); }
	;

state_action:	STATE ':' ID_STATE { $$ = new StateAction(*$3); }
	|	{ $$ = new NoAction(); }
	;

start_state:	START ':' SYMBOL ':' actions { $$ = new StartState($3,$5); }
	;

tapes:		TAPES tapes_rec
		{
		  $$ = $2;
		}
        ;

tapes_rec:      tape tapes_rec
                {
		  Tapes aux(1,$1);
		  $2->insert($2->begin(),aux.begin(),aux.end());
		  aux[0] = nullptr;
		  $$ = $2;
		}
	|	{ $$ = new Tapes(1, new NullTape()); }
	;

tape:		ID_TAPE ':' TAPE { $$ = new Tape(*$1,*$3); }
	;

%%

// C++ definitions here

int main(){

  FILE *myfile = fopen(FILE_NAME, "r");
  if (!myfile) {
    std::cout << "I can't open " << FILE_NAME << "!" << std::endl;
    return -1;
  }

  // link to flex
  yyin = myfile;

  do {
    yyparse();
  } while (!feof(yyin));
  
  return 0;
}

void yyerror(const char *s) {
  std::cout << "Parse error!  Message: " << s << std::endl;
  exit(-1);
}
