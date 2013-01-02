#include "Lexer.h"
#include "Parser.h"
#include "Dag.h"

/* Stack */
#define STACKSIZE 100000

static int stack[STACKSIZE];
static int stack_top;
void push(int i);
int  pop();
int  top();

void push(int i)
{
  stack[stack_top] = i;
  stack_top++;
}

int pop()
{
  stack_top--;
  return stack[stack_top];
}

int top()
{
  return stack[stack_top-1];
}

/* Nonterminals */
enum Nonterminal {
	NONTERMINAL_S_, NONTERMINAL_Program, 
	NONTERMINAL_Decl, NONTERMINAL_TypeDeclaration, 
	NONTERMINAL_TermDeclaration, NONTERMINAL_DefDeclaration, 
	NONTERMINAL_TyDecls, NONTERMINAL_TmDecls, 
	NONTERMINAL_DefDecls, NONTERMINAL_K, 
	NONTERMINAL_A, NONTERMINAL_A1, 
	NONTERMINAL_M, NONTERMINAL_M1
};

/* Accept/Error code */
enum CODE {
  NOERROR=0,

  ACCEPT=1;

  REJECT          = -1,
  STACK_OVERFLOW  = -2,
  STACK_UNDERFLOW = -3
};

/* State */
enum STATE {
	S0, S1, S2, S3, S4,
	S5, S6, S7, S8, S9,
	S10, S11, S12, S13, S14,
	S15, S16, S17, S18, S19_26_37,
	S20_38, S21, S22_40, S23, S24,
	S25_52, S27_53, S28_54, S29, S30_47_65,
	S31_48_66, S32, S33, S34_69, S35_51_70,
	S36, S39, S41, S42_57, S43_58,
	S44_59, S45_60, S46, S49_81, S50_82,
	S55, S56_77_89, S61_90, S62, S63_93,
	S64_80_94, S67, S68, S71, S72_84,
	S73_85, S74_86, S75_87, S76, S78_91,
	S79_101, S83_99_106, S88, S92, S95,
	S96_102, S97_103, S98_104, S100_111, S105,
	S107, S108_112, S109_113, S110_118, S114_120,
	S115, S116_119, S117_121, S122_123
};

/* Goto table */
#define NOOFSTATES 79
#define NOOFNONTERMINALS 14

int goto_table[NOOFSTATES][NOOFNONTERMINALS] = {
  /* Needed to be filled in */
};

int parse(int toks[], int toks_len, int dag[], int dag_size)
{
  int current_tok;
  int dag_top;
  int error;

  current_tok   = 0;
  stack_top     = 0;
  dag_top       = 0;

  error = NOERROR;

  if (statck_top + 1 < STACKSIZE)
    push( S0 ); // push the initial state
  else
    error = STACK_OVERFLOW;

  while ( error == NOERROR && current_tok < toks_len ) 
    {
      switch( top() )
	{
	case S0:
	  switch ( toks[current_tok] )
	    {
	    case ATTYPE:
	      if (stack_top+2 < STACKSIZE)
		{
		  push (ATTYPE);  // push the current token
		  push (S4);      // push the current state

		  current_tok++;
		}
	      else
		error = STACK_OVERFLOW;

	      break;

	    default:
	      error = REJECT;
	      break;

	    }
	  break;

	case S1:
	  switch( toks[current_tok] )
	    {
	    case ENDOFSYMBOL:
	      error = ACCEPT;  // current_state=S1
	      break;

	    default:
	      error = REJECT;
	      break;
	    }
	  break;

	case S2:
	  switch( toks[current_tok] )
	    {
	    case ENDOFSYMBOL: // reduce Program -> Decl
	      if (0 < stack_top-2*1)
		{
		  int next;

		  pop();  // symbol
		  pop();  // state

		  next = top();
		  push (NONTERMINAL_Program);
		  next = goto_table[next, NONTERMINAL_Program];
		  if (0 <= next) // Valid state
		    push (state);
		  else 
		    error = next; 
		}
	      else
		error = STACK_UNDERFLOW;
	      break;

	    default:
	      error = REJECT;
	      break;
	    }
	  break;
	}

    }

  
  
  return 0;
}


/*

124 states
I0:
[S' -> .Program, $]
[Program -> .Decl, $]
[Decl -> .TypeDeclaration TermDeclaration DefDeclaration, $]
[TypeDeclaration -> .atType TyDecls, atTerm]

I1:
[S' -> Program., $]

I2:
[Program -> Decl., $]

I3:
[Decl -> TypeDeclaration.TermDeclaration DefDeclaration, $]
[TermDeclaration -> .atTerm TmDecls, atDef]
[TermDeclaration -> .atTerm TmDecls, $]

I4:
[TypeDeclaration -> atType.TyDecls, atTerm]
[TyDecls -> .var : K ., atTerm]
[TyDecls -> .var : K . TyDecls, atTerm]

I5:
[Decl -> TypeDeclaration TermDeclaration.DefDeclaration, $]
[DefDeclaration -> ., $]
[DefDeclaration -> .atDef DefDecls, $]

I6:
[TermDeclaration -> atTerm.TmDecls, atDef]
[TermDeclaration -> atTerm.TmDecls, $]
[TmDecls -> .var : A ., atDef]
[TmDecls -> .var : A . TmDecls, atDef]
[TmDecls -> .var : A ., $]
[TmDecls -> .var : A . TmDecls, $]

I7:
[TypeDeclaration -> atType TyDecls., atTerm]

I8:
[TyDecls -> var.: K ., atTerm]
[TyDecls -> var.: K . TyDecls, atTerm]

I9:
[Decl -> TypeDeclaration TermDeclaration DefDeclaration., $]

I10:
[DefDeclaration -> atDef.DefDecls, $]
[DefDecls -> .var = M ., $]
[DefDecls -> .var = M . DefDecls, $]

I11:
[TermDeclaration -> atTerm TmDecls., atDef]
[TermDeclaration -> atTerm TmDecls., $]

I12:
[TmDecls -> var.: A ., atDef]
[TmDecls -> var.: A . TmDecls, atDef]
[TmDecls -> var.: A ., $]
[TmDecls -> var.: A . TmDecls, $]

I13:
[TyDecls -> var :.K ., atTerm]
[TyDecls -> var :.K . TyDecls, atTerm]
[K -> .Type, .]
[K -> .Pi var : A . K, .]
[K -> .( K ), .]
[K -> .A1 arrow K, .]
[A1 -> .var, arrow]
[A1 -> .( A ), arrow]
[A1 -> .A1 var, arrow]
[A1 -> .A1 ( M ), arrow]
[A1 -> .var, var]
[A1 -> .( A ), var]
[A1 -> .A1 var, var]
[A1 -> .A1 ( M ), var]
[A1 -> .var, (]
[A1 -> .( A ), (]
[A1 -> .A1 var, (]
[A1 -> .A1 ( M ), (]

I14:
[DefDeclaration -> atDef DefDecls., $]

I15:
[DefDecls -> var.= M ., $]
[DefDecls -> var.= M . DefDecls, $]

I16:
[TmDecls -> var :.A ., atDef]
[TmDecls -> var :.A . TmDecls, atDef]
[TmDecls -> var :.A ., $]
[TmDecls -> var :.A . TmDecls, $]
[A -> .Pi var : A . A, .]
[A -> .A1, .]
[A -> .A1 arrow A, .]
[A1 -> .var, .]
[A1 -> .( A ), .]
[A1 -> .A1 var, .]
[A1 -> .A1 ( M ), .]
[A1 -> .var, arrow]
[A1 -> .( A ), arrow]
[A1 -> .A1 var, arrow]
[A1 -> .A1 ( M ), arrow]
[A1 -> .var, var]
[A1 -> .( A ), var]
[A1 -> .A1 var, var]
[A1 -> .A1 ( M ), var]
[A1 -> .var, (]
[A1 -> .( A ), (]
[A1 -> .A1 var, (]
[A1 -> .A1 ( M ), (]

I17:
[TyDecls -> var : K.., atTerm]
[TyDecls -> var : K.. TyDecls, atTerm]

I18:
[K -> A1.arrow K, .]
[A1 -> A1.var, arrow]
[A1 -> A1.( M ), arrow]
[A1 -> A1.var, var]
[A1 -> A1.( M ), var]
[A1 -> A1.var, (]
[A1 -> A1.( M ), (]

I19:
[A1 -> var., arrow]
[A1 -> var., var]
[A1 -> var., (]

I20:
[K -> Type., .]

I21:
[K -> Pi.var : A . K, .]

I22:
[K -> (.K ), .]
[A1 -> (.A ), arrow]
[A1 -> (.A ), var]
[A1 -> (.A ), (]
[K -> .Type, )]
[K -> .Pi var : A . K, )]
[K -> .( K ), )]
[K -> .A1 arrow K, )]
[A -> .Pi var : A . A, )]
[A -> .A1, )]
[A -> .A1 arrow A, )]
[A1 -> .var, arrow]
[A1 -> .( A ), arrow]
[A1 -> .A1 var, arrow]
[A1 -> .A1 ( M ), arrow]
[A1 -> .var, )]
[A1 -> .( A ), )]
[A1 -> .A1 var, )]
[A1 -> .A1 ( M ), )]
[A1 -> .var, var]
[A1 -> .( A ), var]
[A1 -> .A1 var, var]
[A1 -> .A1 ( M ), var]
[A1 -> .var, (]
[A1 -> .( A ), (]
[A1 -> .A1 var, (]
[A1 -> .A1 ( M ), (]

I23:
[DefDecls -> var =.M ., $]
[DefDecls -> var =.M . DefDecls, $]
[M -> .Lam var : A . M, .]
[M -> .M1, .]
[M1 -> .var, .]
[M1 -> .( M ), .]
[M1 -> .M1 var, .]
[M1 -> .M1 ( M ), .]
[M1 -> .var, var]
[M1 -> .( M ), var]
[M1 -> .M1 var, var]
[M1 -> .M1 ( M ), var]
[M1 -> .var, (]
[M1 -> .( M ), (]
[M1 -> .M1 var, (]
[M1 -> .M1 ( M ), (]

I24:
[TmDecls -> var : A.., atDef]
[TmDecls -> var : A.. TmDecls, atDef]
[TmDecls -> var : A.., $]
[TmDecls -> var : A.. TmDecls, $]

I25:
[A -> A1., .]
[A -> A1.arrow A, .]
[A1 -> A1.var, .]
[A1 -> A1.( M ), .]
[A1 -> A1.var, arrow]
[A1 -> A1.( M ), arrow]
[A1 -> A1.var, var]
[A1 -> A1.( M ), var]
[A1 -> A1.var, (]
[A1 -> A1.( M ), (]

I26:
[A1 -> var., .]
[A1 -> var., arrow]
[A1 -> var., var]
[A1 -> var., (]

I27:
[A -> Pi.var : A . A, .]

I28:
[A1 -> (.A ), .]
[A1 -> (.A ), arrow]
[A1 -> (.A ), var]
[A1 -> (.A ), (]
[A -> .Pi var : A . A, )]
[A -> .A1, )]
[A -> .A1 arrow A, )]
[A1 -> .var, )]
[A1 -> .( A ), )]
[A1 -> .A1 var, )]
[A1 -> .A1 ( M ), )]
[A1 -> .var, arrow]
[A1 -> .( A ), arrow]
[A1 -> .A1 var, arrow]
[A1 -> .A1 ( M ), arrow]
[A1 -> .var, var]
[A1 -> .( A ), var]
[A1 -> .A1 var, var]
[A1 -> .A1 ( M ), var]
[A1 -> .var, (]
[A1 -> .( A ), (]
[A1 -> .A1 var, (]
[A1 -> .A1 ( M ), (]

I29:
[TyDecls -> var : K .., atTerm]
[TyDecls -> var : K ..TyDecls, atTerm]
[TyDecls -> .var : K ., atTerm]
[TyDecls -> .var : K . TyDecls, atTerm]

I30:
[A1 -> A1 var., arrow]
[A1 -> A1 var., var]
[A1 -> A1 var., (]

I31:
[A1 -> A1 (.M ), arrow]
[A1 -> A1 (.M ), var]
[A1 -> A1 (.M ), (]
[M -> .Lam var : A . M, )]
[M -> .M1, )]
[M1 -> .var, )]
[M1 -> .( M ), )]
[M1 -> .M1 var, )]
[M1 -> .M1 ( M ), )]
[M1 -> .var, var]
[M1 -> .( M ), var]
[M1 -> .M1 var, var]
[M1 -> .M1 ( M ), var]
[M1 -> .var, (]
[M1 -> .( M ), (]
[M1 -> .M1 var, (]
[M1 -> .M1 ( M ), (]

I32:
[K -> A1 arrow.K, .]
[K -> .Type, .]
[K -> .Pi var : A . K, .]
[K -> .( K ), .]
[K -> .A1 arrow K, .]
[A1 -> .var, arrow]
[A1 -> .( A ), arrow]
[A1 -> .A1 var, arrow]
[A1 -> .A1 ( M ), arrow]
[A1 -> .var, var]
[A1 -> .( A ), var]
[A1 -> .A1 var, var]
[A1 -> .A1 ( M ), var]
[A1 -> .var, (]
[A1 -> .( A ), (]
[A1 -> .A1 var, (]
[A1 -> .A1 ( M ), (]

I33:
[K -> Pi var.: A . K, .]

I34:
[K -> ( K.), .]

I35:
[A1 -> ( A.), arrow]
[A1 -> ( A.), var]
[A1 -> ( A.), (]

I36:
[K -> A1.arrow K, )]
[A -> A1., )]
[A -> A1.arrow A, )]
[A1 -> A1.var, arrow]
[A1 -> A1.( M ), arrow]
[A1 -> A1.var, )]
[A1 -> A1.( M ), )]
[A1 -> A1.var, var]
[A1 -> A1.( M ), var]
[A1 -> A1.var, (]
[A1 -> A1.( M ), (]

I37:
[A1 -> var., arrow]
[A1 -> var., )]
[A1 -> var., var]
[A1 -> var., (]

I38:
[K -> Type., )]

I39:
[K -> Pi.var : A . K, )]
[A -> Pi.var : A . A, )]

I40:
[K -> (.K ), )]
[A1 -> (.A ), arrow]
[A1 -> (.A ), )]
[A1 -> (.A ), var]
[A1 -> (.A ), (]
[K -> .Type, )]
[K -> .Pi var : A . K, )]
[K -> .( K ), )]
[K -> .A1 arrow K, )]
[A -> .Pi var : A . A, )]
[A -> .A1, )]
[A -> .A1 arrow A, )]
[A1 -> .var, arrow]
[A1 -> .( A ), arrow]
[A1 -> .A1 var, arrow]
[A1 -> .A1 ( M ), arrow]
[A1 -> .var, )]
[A1 -> .( A ), )]
[A1 -> .A1 var, )]
[A1 -> .A1 ( M ), )]
[A1 -> .var, var]
[A1 -> .( A ), var]
[A1 -> .A1 var, var]
[A1 -> .A1 ( M ), var]
[A1 -> .var, (]
[A1 -> .( A ), (]
[A1 -> .A1 var, (]
[A1 -> .A1 ( M ), (]

I41:
[DefDecls -> var = M.., $]
[DefDecls -> var = M.. DefDecls, $]

I42:
[M -> M1., .]
[M1 -> M1.var, .]
[M1 -> M1.( M ), .]
[M1 -> M1.var, var]
[M1 -> M1.( M ), var]
[M1 -> M1.var, (]
[M1 -> M1.( M ), (]

I43:
[M1 -> var., .]
[M1 -> var., var]
[M1 -> var., (]

I44:
[M1 -> (.M ), .]
[M1 -> (.M ), var]
[M1 -> (.M ), (]
[M -> .Lam var : A . M, )]
[M -> .M1, )]
[M1 -> .var, )]
[M1 -> .( M ), )]
[M1 -> .M1 var, )]
[M1 -> .M1 ( M ), )]
[M1 -> .var, var]
[M1 -> .( M ), var]
[M1 -> .M1 var, var]
[M1 -> .M1 ( M ), var]
[M1 -> .var, (]
[M1 -> .( M ), (]
[M1 -> .M1 var, (]
[M1 -> .M1 ( M ), (]

I45:
[M -> Lam.var : A . M, .]

I46:
[TmDecls -> var : A .., atDef]
[TmDecls -> var : A ..TmDecls, atDef]
[TmDecls -> var : A .., $]
[TmDecls -> var : A ..TmDecls, $]
[TmDecls -> .var : A ., atDef]
[TmDecls -> .var : A . TmDecls, atDef]
[TmDecls -> .var : A ., $]
[TmDecls -> .var : A . TmDecls, $]

I47:
[A1 -> A1 var., .]
[A1 -> A1 var., arrow]
[A1 -> A1 var., var]
[A1 -> A1 var., (]

I48:
[A1 -> A1 (.M ), .]
[A1 -> A1 (.M ), arrow]
[A1 -> A1 (.M ), var]
[A1 -> A1 (.M ), (]
[M -> .Lam var : A . M, )]
[M -> .M1, )]
[M1 -> .var, )]
[M1 -> .( M ), )]
[M1 -> .M1 var, )]
[M1 -> .M1 ( M ), )]
[M1 -> .var, var]
[M1 -> .( M ), var]
[M1 -> .M1 var, var]
[M1 -> .M1 ( M ), var]
[M1 -> .var, (]
[M1 -> .( M ), (]
[M1 -> .M1 var, (]
[M1 -> .M1 ( M ), (]

I49:
[A -> A1 arrow.A, .]
[A -> .Pi var : A . A, .]
[A -> .A1, .]
[A -> .A1 arrow A, .]
[A1 -> .var, .]
[A1 -> .( A ), .]
[A1 -> .A1 var, .]
[A1 -> .A1 ( M ), .]
[A1 -> .var, arrow]
[A1 -> .( A ), arrow]
[A1 -> .A1 var, arrow]
[A1 -> .A1 ( M ), arrow]
[A1 -> .var, var]
[A1 -> .( A ), var]
[A1 -> .A1 var, var]
[A1 -> .A1 ( M ), var]
[A1 -> .var, (]
[A1 -> .( A ), (]
[A1 -> .A1 var, (]
[A1 -> .A1 ( M ), (]

I50:
[A -> Pi var.: A . A, .]

I51:
[A1 -> ( A.), .]
[A1 -> ( A.), arrow]
[A1 -> ( A.), var]
[A1 -> ( A.), (]

I52:
[A -> A1., )]
[A -> A1.arrow A, )]
[A1 -> A1.var, )]
[A1 -> A1.( M ), )]
[A1 -> A1.var, arrow]
[A1 -> A1.( M ), arrow]
[A1 -> A1.var, var]
[A1 -> A1.( M ), var]
[A1 -> A1.var, (]
[A1 -> A1.( M ), (]

I53:
[A -> Pi.var : A . A, )]

I54:
[A1 -> (.A ), )]
[A1 -> (.A ), arrow]
[A1 -> (.A ), var]
[A1 -> (.A ), (]
[A -> .Pi var : A . A, )]
[A -> .A1, )]
[A -> .A1 arrow A, )]
[A1 -> .var, )]
[A1 -> .( A ), )]
[A1 -> .A1 var, )]
[A1 -> .A1 ( M ), )]
[A1 -> .var, arrow]
[A1 -> .( A ), arrow]
[A1 -> .A1 var, arrow]
[A1 -> .A1 ( M ), arrow]
[A1 -> .var, var]
[A1 -> .( A ), var]
[A1 -> .A1 var, var]
[A1 -> .A1 ( M ), var]
[A1 -> .var, (]
[A1 -> .( A ), (]
[A1 -> .A1 var, (]
[A1 -> .A1 ( M ), (]

I55:
[TyDecls -> var : K . TyDecls., atTerm]

I56:
[A1 -> A1 ( M.), arrow]
[A1 -> A1 ( M.), var]
[A1 -> A1 ( M.), (]

I57:
[M -> M1., )]
[M1 -> M1.var, )]
[M1 -> M1.( M ), )]
[M1 -> M1.var, var]
[M1 -> M1.( M ), var]
[M1 -> M1.var, (]
[M1 -> M1.( M ), (]

I58:
[M1 -> var., )]
[M1 -> var., var]
[M1 -> var., (]

I59:
[M1 -> (.M ), )]
[M1 -> (.M ), var]
[M1 -> (.M ), (]
[M -> .Lam var : A . M, )]
[M -> .M1, )]
[M1 -> .var, )]
[M1 -> .( M ), )]
[M1 -> .M1 var, )]
[M1 -> .M1 ( M ), )]
[M1 -> .var, var]
[M1 -> .( M ), var]
[M1 -> .M1 var, var]
[M1 -> .M1 ( M ), var]
[M1 -> .var, (]
[M1 -> .( M ), (]
[M1 -> .M1 var, (]
[M1 -> .M1 ( M ), (]

I60:
[M -> Lam.var : A . M, )]

I61:
[K -> A1 arrow K., .]

I62:
[K -> Pi var :.A . K, .]
[A -> .Pi var : A . A, .]
[A -> .A1, .]
[A -> .A1 arrow A, .]
[A1 -> .var, .]
[A1 -> .( A ), .]
[A1 -> .A1 var, .]
[A1 -> .A1 ( M ), .]
[A1 -> .var, arrow]
[A1 -> .( A ), arrow]
[A1 -> .A1 var, arrow]
[A1 -> .A1 ( M ), arrow]
[A1 -> .var, var]
[A1 -> .( A ), var]
[A1 -> .A1 var, var]
[A1 -> .A1 ( M ), var]
[A1 -> .var, (]
[A1 -> .( A ), (]
[A1 -> .A1 var, (]
[A1 -> .A1 ( M ), (]

I63:
[K -> ( K )., .]

I64:
[A1 -> ( A )., arrow]
[A1 -> ( A )., var]
[A1 -> ( A )., (]

I65:
[A1 -> A1 var., arrow]
[A1 -> A1 var., )]
[A1 -> A1 var., var]
[A1 -> A1 var., (]

I66:
[A1 -> A1 (.M ), arrow]
[A1 -> A1 (.M ), )]
[A1 -> A1 (.M ), var]
[A1 -> A1 (.M ), (]
[M -> .Lam var : A . M, )]
[M -> .M1, )]
[M1 -> .var, )]
[M1 -> .( M ), )]
[M1 -> .M1 var, )]
[M1 -> .M1 ( M ), )]
[M1 -> .var, var]
[M1 -> .( M ), var]
[M1 -> .M1 var, var]
[M1 -> .M1 ( M ), var]
[M1 -> .var, (]
[M1 -> .( M ), (]
[M1 -> .M1 var, (]
[M1 -> .M1 ( M ), (]

I67:
[K -> A1 arrow.K, )]
[A -> A1 arrow.A, )]
[K -> .Type, )]
[K -> .Pi var : A . K, )]
[K -> .( K ), )]
[K -> .A1 arrow K, )]
[A -> .Pi var : A . A, )]
[A -> .A1, )]
[A -> .A1 arrow A, )]
[A1 -> .var, arrow]
[A1 -> .( A ), arrow]
[A1 -> .A1 var, arrow]
[A1 -> .A1 ( M ), arrow]
[A1 -> .var, )]
[A1 -> .( A ), )]
[A1 -> .A1 var, )]
[A1 -> .A1 ( M ), )]
[A1 -> .var, var]
[A1 -> .( A ), var]
[A1 -> .A1 var, var]
[A1 -> .A1 ( M ), var]
[A1 -> .var, (]
[A1 -> .( A ), (]
[A1 -> .A1 var, (]
[A1 -> .A1 ( M ), (]

I68:
[K -> Pi var.: A . K, )]
[A -> Pi var.: A . A, )]

I69:
[K -> ( K.), )]

I70:
[A1 -> ( A.), arrow]
[A1 -> ( A.), )]
[A1 -> ( A.), var]
[A1 -> ( A.), (]

I71:
[DefDecls -> var = M .., $]
[DefDecls -> var = M ..DefDecls, $]
[DefDecls -> .var = M ., $]
[DefDecls -> .var = M . DefDecls, $]

I72:
[M1 -> M1 var., .]
[M1 -> M1 var., var]
[M1 -> M1 var., (]

I73:
[M1 -> M1 (.M ), .]
[M1 -> M1 (.M ), var]
[M1 -> M1 (.M ), (]
[M -> .Lam var : A . M, )]
[M -> .M1, )]
[M1 -> .var, )]
[M1 -> .( M ), )]
[M1 -> .M1 var, )]
[M1 -> .M1 ( M ), )]
[M1 -> .var, var]
[M1 -> .( M ), var]
[M1 -> .M1 var, var]
[M1 -> .M1 ( M ), var]
[M1 -> .var, (]
[M1 -> .( M ), (]
[M1 -> .M1 var, (]
[M1 -> .M1 ( M ), (]

I74:
[M1 -> ( M.), .]
[M1 -> ( M.), var]
[M1 -> ( M.), (]

I75:
[M -> Lam var.: A . M, .]

I76:
[TmDecls -> var : A . TmDecls., atDef]
[TmDecls -> var : A . TmDecls., $]

I77:
[A1 -> A1 ( M.), .]
[A1 -> A1 ( M.), arrow]
[A1 -> A1 ( M.), var]
[A1 -> A1 ( M.), (]

I78:
[A -> A1 arrow A., .]

I79:
[A -> Pi var :.A . A, .]
[A -> .Pi var : A . A, .]
[A -> .A1, .]
[A -> .A1 arrow A, .]
[A1 -> .var, .]
[A1 -> .( A ), .]
[A1 -> .A1 var, .]
[A1 -> .A1 ( M ), .]
[A1 -> .var, arrow]
[A1 -> .( A ), arrow]
[A1 -> .A1 var, arrow]
[A1 -> .A1 ( M ), arrow]
[A1 -> .var, var]
[A1 -> .( A ), var]
[A1 -> .A1 var, var]
[A1 -> .A1 ( M ), var]
[A1 -> .var, (]
[A1 -> .( A ), (]
[A1 -> .A1 var, (]
[A1 -> .A1 ( M ), (]

I80:
[A1 -> ( A )., .]
[A1 -> ( A )., arrow]
[A1 -> ( A )., var]
[A1 -> ( A )., (]

I81:
[A -> A1 arrow.A, )]
[A -> .Pi var : A . A, )]
[A -> .A1, )]
[A -> .A1 arrow A, )]
[A1 -> .var, )]
[A1 -> .( A ), )]
[A1 -> .A1 var, )]
[A1 -> .A1 ( M ), )]
[A1 -> .var, arrow]
[A1 -> .( A ), arrow]
[A1 -> .A1 var, arrow]
[A1 -> .A1 ( M ), arrow]
[A1 -> .var, var]
[A1 -> .( A ), var]
[A1 -> .A1 var, var]
[A1 -> .A1 ( M ), var]
[A1 -> .var, (]
[A1 -> .( A ), (]
[A1 -> .A1 var, (]
[A1 -> .A1 ( M ), (]

I82:
[A -> Pi var.: A . A, )]

I83:
[A1 -> A1 ( M )., arrow]
[A1 -> A1 ( M )., var]
[A1 -> A1 ( M )., (]

I84:
[M1 -> M1 var., )]
[M1 -> M1 var., var]
[M1 -> M1 var., (]

I85:
[M1 -> M1 (.M ), )]
[M1 -> M1 (.M ), var]
[M1 -> M1 (.M ), (]
[M -> .Lam var : A . M, )]
[M -> .M1, )]
[M1 -> .var, )]
[M1 -> .( M ), )]
[M1 -> .M1 var, )]
[M1 -> .M1 ( M ), )]
[M1 -> .var, var]
[M1 -> .( M ), var]
[M1 -> .M1 var, var]
[M1 -> .M1 ( M ), var]
[M1 -> .var, (]
[M1 -> .( M ), (]
[M1 -> .M1 var, (]
[M1 -> .M1 ( M ), (]

I86:
[M1 -> ( M.), )]
[M1 -> ( M.), var]
[M1 -> ( M.), (]

I87:
[M -> Lam var.: A . M, )]

I88:
[K -> Pi var : A.. K, .]

I89:
[A1 -> A1 ( M.), arrow]
[A1 -> A1 ( M.), )]
[A1 -> A1 ( M.), var]
[A1 -> A1 ( M.), (]

I90:
[K -> A1 arrow K., )]

I91:
[A -> A1 arrow A., )]

I92:
[K -> Pi var :.A . K, )]
[A -> Pi var :.A . A, )]
[A -> .Pi var : A . A, .]
[A -> .A1, .]
[A -> .A1 arrow A, .]
[A1 -> .var, .]
[A1 -> .( A ), .]
[A1 -> .A1 var, .]
[A1 -> .A1 ( M ), .]
[A1 -> .var, arrow]
[A1 -> .( A ), arrow]
[A1 -> .A1 var, arrow]
[A1 -> .A1 ( M ), arrow]
[A1 -> .var, var]
[A1 -> .( A ), var]
[A1 -> .A1 var, var]
[A1 -> .A1 ( M ), var]
[A1 -> .var, (]
[A1 -> .( A ), (]
[A1 -> .A1 var, (]
[A1 -> .A1 ( M ), (]

I93:
[K -> ( K )., )]

I94:
[A1 -> ( A )., arrow]
[A1 -> ( A )., )]
[A1 -> ( A )., var]
[A1 -> ( A )., (]

I95:
[DefDecls -> var = M . DefDecls., $]

I96:
[M1 -> M1 ( M.), .]
[M1 -> M1 ( M.), var]
[M1 -> M1 ( M.), (]

I97:
[M1 -> ( M )., .]
[M1 -> ( M )., var]
[M1 -> ( M )., (]

I98:
[M -> Lam var :.A . M, .]
[A -> .Pi var : A . A, .]
[A -> .A1, .]
[A -> .A1 arrow A, .]
[A1 -> .var, .]
[A1 -> .( A ), .]
[A1 -> .A1 var, .]
[A1 -> .A1 ( M ), .]
[A1 -> .var, arrow]
[A1 -> .( A ), arrow]
[A1 -> .A1 var, arrow]
[A1 -> .A1 ( M ), arrow]
[A1 -> .var, var]
[A1 -> .( A ), var]
[A1 -> .A1 var, var]
[A1 -> .A1 ( M ), var]
[A1 -> .var, (]
[A1 -> .( A ), (]
[A1 -> .A1 var, (]
[A1 -> .A1 ( M ), (]

I99:
[A1 -> A1 ( M )., .]
[A1 -> A1 ( M )., arrow]
[A1 -> A1 ( M )., var]
[A1 -> A1 ( M )., (]

I100:
[A -> Pi var : A.. A, .]

I101:
[A -> Pi var :.A . A, )]
[A -> .Pi var : A . A, .]
[A -> .A1, .]
[A -> .A1 arrow A, .]
[A1 -> .var, .]
[A1 -> .( A ), .]
[A1 -> .A1 var, .]
[A1 -> .A1 ( M ), .]
[A1 -> .var, arrow]
[A1 -> .( A ), arrow]
[A1 -> .A1 var, arrow]
[A1 -> .A1 ( M ), arrow]
[A1 -> .var, var]
[A1 -> .( A ), var]
[A1 -> .A1 var, var]
[A1 -> .A1 ( M ), var]
[A1 -> .var, (]
[A1 -> .( A ), (]
[A1 -> .A1 var, (]
[A1 -> .A1 ( M ), (]

I102:
[M1 -> M1 ( M.), )]
[M1 -> M1 ( M.), var]
[M1 -> M1 ( M.), (]

I103:
[M1 -> ( M )., )]
[M1 -> ( M )., var]
[M1 -> ( M )., (]

I104:
[M -> Lam var :.A . M, )]
[A -> .Pi var : A . A, .]
[A -> .A1, .]
[A -> .A1 arrow A, .]
[A1 -> .var, .]
[A1 -> .( A ), .]
[A1 -> .A1 var, .]
[A1 -> .A1 ( M ), .]
[A1 -> .var, arrow]
[A1 -> .( A ), arrow]
[A1 -> .A1 var, arrow]
[A1 -> .A1 ( M ), arrow]
[A1 -> .var, var]
[A1 -> .( A ), var]
[A1 -> .A1 var, var]
[A1 -> .A1 ( M ), var]
[A1 -> .var, (]
[A1 -> .( A ), (]
[A1 -> .A1 var, (]
[A1 -> .A1 ( M ), (]

I105:
[K -> Pi var : A ..K, .]
[K -> .Type, .]
[K -> .Pi var : A . K, .]
[K -> .( K ), .]
[K -> .A1 arrow K, .]
[A1 -> .var, arrow]
[A1 -> .( A ), arrow]
[A1 -> .A1 var, arrow]
[A1 -> .A1 ( M ), arrow]
[A1 -> .var, var]
[A1 -> .( A ), var]
[A1 -> .A1 var, var]
[A1 -> .A1 ( M ), var]
[A1 -> .var, (]
[A1 -> .( A ), (]
[A1 -> .A1 var, (]
[A1 -> .A1 ( M ), (]

I106:
[A1 -> A1 ( M )., arrow]
[A1 -> A1 ( M )., )]
[A1 -> A1 ( M )., var]
[A1 -> A1 ( M )., (]

I107:
[K -> Pi var : A.. K, )]
[A -> Pi var : A.. A, )]

I108:
[M1 -> M1 ( M )., .]
[M1 -> M1 ( M )., var]
[M1 -> M1 ( M )., (]

I109:
[M -> Lam var : A.. M, .]

I110:
[A -> Pi var : A ..A, .]
[A -> .Pi var : A . A, .]
[A -> .A1, .]
[A -> .A1 arrow A, .]
[A1 -> .var, .]
[A1 -> .( A ), .]
[A1 -> .A1 var, .]
[A1 -> .A1 ( M ), .]
[A1 -> .var, arrow]
[A1 -> .( A ), arrow]
[A1 -> .A1 var, arrow]
[A1 -> .A1 ( M ), arrow]
[A1 -> .var, var]
[A1 -> .( A ), var]
[A1 -> .A1 var, var]
[A1 -> .A1 ( M ), var]
[A1 -> .var, (]
[A1 -> .( A ), (]
[A1 -> .A1 var, (]
[A1 -> .A1 ( M ), (]

I111:
[A -> Pi var : A.. A, )]

I112:
[M1 -> M1 ( M )., )]
[M1 -> M1 ( M )., var]
[M1 -> M1 ( M )., (]

I113:
[M -> Lam var : A.. M, )]

I114:
[K -> Pi var : A . K., .]

I115:
[K -> Pi var : A ..K, )]
[A -> Pi var : A ..A, )]
[K -> .Type, )]
[K -> .Pi var : A . K, )]
[K -> .( K ), )]
[K -> .A1 arrow K, )]
[A -> .Pi var : A . A, )]
[A -> .A1, )]
[A -> .A1 arrow A, )]
[A1 -> .var, arrow]
[A1 -> .( A ), arrow]
[A1 -> .A1 var, arrow]
[A1 -> .A1 ( M ), arrow]
[A1 -> .var, )]
[A1 -> .( A ), )]
[A1 -> .A1 var, )]
[A1 -> .A1 ( M ), )]
[A1 -> .var, var]
[A1 -> .( A ), var]
[A1 -> .A1 var, var]
[A1 -> .A1 ( M ), var]
[A1 -> .var, (]
[A1 -> .( A ), (]
[A1 -> .A1 var, (]
[A1 -> .A1 ( M ), (]

I116:
[M -> Lam var : A ..M, .]
[M -> .Lam var : A . M, .]
[M -> .M1, .]
[M1 -> .var, .]
[M1 -> .( M ), .]
[M1 -> .M1 var, .]
[M1 -> .M1 ( M ), .]
[M1 -> .var, var]
[M1 -> .( M ), var]
[M1 -> .M1 var, var]
[M1 -> .M1 ( M ), var]
[M1 -> .var, (]
[M1 -> .( M ), (]
[M1 -> .M1 var, (]
[M1 -> .M1 ( M ), (]

I117:
[A -> Pi var : A . A., .]

I118:
[A -> Pi var : A ..A, )]
[A -> .Pi var : A . A, )]
[A -> .A1, )]
[A -> .A1 arrow A, )]
[A1 -> .var, )]
[A1 -> .( A ), )]
[A1 -> .A1 var, )]
[A1 -> .A1 ( M ), )]
[A1 -> .var, arrow]
[A1 -> .( A ), arrow]
[A1 -> .A1 var, arrow]
[A1 -> .A1 ( M ), arrow]
[A1 -> .var, var]
[A1 -> .( A ), var]
[A1 -> .A1 var, var]
[A1 -> .A1 ( M ), var]
[A1 -> .var, (]
[A1 -> .( A ), (]
[A1 -> .A1 var, (]
[A1 -> .A1 ( M ), (]

I119:
[M -> Lam var : A ..M, )]
[M -> .Lam var : A . M, )]
[M -> .M1, )]
[M1 -> .var, )]
[M1 -> .( M ), )]
[M1 -> .M1 var, )]
[M1 -> .M1 ( M ), )]
[M1 -> .var, var]
[M1 -> .( M ), var]
[M1 -> .M1 var, var]
[M1 -> .M1 ( M ), var]
[M1 -> .var, (]
[M1 -> .( M ), (]
[M1 -> .M1 var, (]
[M1 -> .M1 ( M ), (]

I120:
[K -> Pi var : A . K., )]

I121:
[A -> Pi var : A . A., )]

I122:
[M -> Lam var : A . M., .]

I123:
[M -> Lam var : A . M., )]


0: S' -> Program
1: Program -> Decl
2: Decl -> TypeDeclaration TermDeclaration DefDeclaration
3: TypeDeclaration -> atType TyDecls
4: TermDeclaration -> atTerm TmDecls
5: DefDeclaration -> 
6: DefDeclaration -> atDef DefDecls
7: TyDecls -> var : K .
8: TyDecls -> var : K . TyDecls
9: TmDecls -> var : A .
10: TmDecls -> var : A . TmDecls
11: DefDecls -> var = M .
12: DefDecls -> var = M . DefDecls
13: K -> Type
14: K -> Pi var : A . K
15: K -> ( K )
16: K -> A1 arrow K
17: A -> Pi var : A . A
18: A -> A1
19: A -> A1 arrow A
20: A1 -> var
21: A1 -> ( A )
22: A1 -> A1 var
23: A1 -> A1 ( M )
24: M -> Lam var : A . M
25: M -> M1
26: M1 -> var
27: M1 -> ( M )
28: M1 -> M1 var
29: M1 -> M1 ( M )

79 states
[0]
[1]
[2]
[3]
[4]
[5]
[6]
[7]
[8]
[9]
[10]
[11]
[12]
[13]
[14]
[15]
[16]
[17]
[18]
[19,26,37]
[20,38]
[21]
[22,40]
[23]
[24]
[25,52]
[27,53]
[28,54]
[29]
[30,47,65]
[31,48,66]
[32]
[33]
[34,69]
[35,51,70]
[36]
[39]
[41]
[42,57]
[43,58]
[44,59]
[45,60]
[46]
[49,81]
[50,82]
[55]
[56,77,89]
[61,90]
[62]
[63,93]
[64,80,94]
[67]
[68]
[71]
[72,84]
[73,85]
[74,86]
[75,87]
[76]
[78,91]
[79,101]
[83,99,106]
[88]
[92]
[95]
[96,102]
[97,103]
[98,104]
[100,111]
[105]
[107]
[108,112]
[109,113]
[110,118]
[114,120]
[115]
[116,119]
[117,121]
[122,123]

[0]	atType	LALRShift [4]
[1]	$	LALRAccept
[2]	$	LALRReduce 1
[3]	atTerm	LALRShift [6]
[4]	var	LALRShift [8]
[5]	$	LALRReduce 5
[5]	atDef	LALRShift [10]
[6]	var	LALRShift [12]
[7]	atTerm	LALRReduce 3
[8]	:	LALRShift [13]
[9]	$	LALRReduce 2
[10]	var	LALRShift [15]
[11]	atDef	LALRReduce 4
[11]	$	LALRReduce 4
[12]	:	LALRShift [16]
[13]	Type	LALRShift [20,38]
[13]	Pi	LALRShift [21]
[13]	(	LALRShift [22,40]
[13]	var	LALRShift [19,26,37]
[14]	$	LALRReduce 6
[15]	=	LALRShift [23]
[16]	Pi	LALRShift [27,53]
[16]	var	LALRShift [19,26,37]
[16]	(	LALRShift [28,54]
[17]	.	LALRShift [29]
[18]	arrow	LALRShift [32]
[18]	var	LALRShift [30,47,65]
[18]	(	LALRShift [31,48,66]
[19,26,37]	arrow	LALRReduce 20
[19,26,37]	var	LALRReduce 20
[19,26,37]	(	LALRReduce 20
[19,26,37]	.	LALRReduce 20
[19,26,37]	)	LALRReduce 20
[20,38]	.	LALRReduce 13
[20,38]	)	LALRReduce 13
[21]	var	LALRShift [33]
[22,40]	Type	LALRShift [20,38]
[22,40]	Pi	LALRShift [39]
[22,40]	(	LALRShift [22,40]
[22,40]	var	LALRShift [19,26,37]
[23]	Lam	LALRShift [45,60]
[23]	var	LALRShift [43,58]
[23]	(	LALRShift [44,59]
[24]	.	LALRShift [46]
[25,52]	.	LALRReduce 18
[25,52]	arrow	LALRShift [49,81]
[25,52]	var	LALRShift [30,47,65]
[25,52]	(	LALRShift [31,48,66]
[25,52]	)	LALRReduce 18
[27,53]	var	LALRShift [50,82]
[28,54]	Pi	LALRShift [27,53]
[28,54]	var	LALRShift [19,26,37]
[28,54]	(	LALRShift [28,54]
[29]	atTerm	LALRReduce 7
[29]	var	LALRShift [8]
[30,47,65]	arrow	LALRReduce 22
[30,47,65]	var	LALRReduce 22
[30,47,65]	(	LALRReduce 22
[30,47,65]	.	LALRReduce 22
[30,47,65]	)	LALRReduce 22
[31,48,66]	Lam	LALRShift [45,60]
[31,48,66]	var	LALRShift [43,58]
[31,48,66]	(	LALRShift [44,59]
[32]	Type	LALRShift [20,38]
[32]	Pi	LALRShift [21]
[32]	(	LALRShift [22,40]
[32]	var	LALRShift [19,26,37]
[33]	:	LALRShift [62]
[34,69]	)	LALRShift [63,93]
[35,51,70]	)	LALRShift [64,80,94]
[36]	arrow	LALRShift [67]
[36]	)	LALRReduce 18
[36]	var	LALRShift [30,47,65]
[36]	(	LALRShift [31,48,66]
[39]	var	LALRShift [68]
[41]	.	LALRShift [71]
[42,57]	.	LALRReduce 25
[42,57]	var	LALRShift [72,84]
[42,57]	(	LALRShift [73,85]
[42,57]	)	LALRReduce 25
[43,58]	.	LALRReduce 26
[43,58]	var	LALRReduce 26
[43,58]	(	LALRReduce 26
[43,58]	)	LALRReduce 26
[44,59]	Lam	LALRShift [45,60]
[44,59]	var	LALRShift [43,58]
[44,59]	(	LALRShift [44,59]
[45,60]	var	LALRShift [75,87]
[46]	atDef	LALRReduce 9
[46]	$	LALRReduce 9
[46]	var	LALRShift [12]
[49,81]	Pi	LALRShift [27,53]
[49,81]	var	LALRShift [19,26,37]
[49,81]	(	LALRShift [28,54]
[50,82]	:	LALRShift [79,101]
[55]	atTerm	LALRReduce 8
[56,77,89]	)	LALRShift [83,99,106]
[61,90]	.	LALRReduce 16
[61,90]	)	LALRReduce 16
[62]	Pi	LALRShift [27,53]
[62]	var	LALRShift [19,26,37]
[62]	(	LALRShift [28,54]
[63,93]	.	LALRReduce 15
[63,93]	)	LALRReduce 15
[64,80,94]	arrow	LALRReduce 21
[64,80,94]	var	LALRReduce 21
[64,80,94]	(	LALRReduce 21
[64,80,94]	.	LALRReduce 21
[64,80,94]	)	LALRReduce 21
[67]	Type	LALRShift [20,38]
[67]	Pi	LALRShift [39]
[67]	(	LALRShift [22,40]
[67]	var	LALRShift [19,26,37]
[68]	:	LALRShift [92]
[71]	$	LALRReduce 11
[71]	var	LALRShift [15]
[72,84]	.	LALRReduce 28
[72,84]	var	LALRReduce 28
[72,84]	(	LALRReduce 28
[72,84]	)	LALRReduce 28
[73,85]	Lam	LALRShift [45,60]
[73,85]	var	LALRShift [43,58]
[73,85]	(	LALRShift [44,59]
[74,86]	)	LALRShift [97,103]
[75,87]	:	LALRShift [98,104]
[76]	atDef	LALRReduce 10
[76]	$	LALRReduce 10
[78,91]	.	LALRReduce 19
[78,91]	)	LALRReduce 19
[79,101]	Pi	LALRShift [27,53]
[79,101]	var	LALRShift [19,26,37]
[79,101]	(	LALRShift [28,54]
[83,99,106]	arrow	LALRReduce 23
[83,99,106]	var	LALRReduce 23
[83,99,106]	(	LALRReduce 23
[83,99,106]	.	LALRReduce 23
[83,99,106]	)	LALRReduce 23
[88]	.	LALRShift [105]
[92]	Pi	LALRShift [27,53]
[92]	var	LALRShift [19,26,37]
[92]	(	LALRShift [28,54]
[95]	$	LALRReduce 12
[96,102]	)	LALRShift [108,112]
[97,103]	.	LALRReduce 27
[97,103]	var	LALRReduce 27
[97,103]	(	LALRReduce 27
[97,103]	)	LALRReduce 27
[98,104]	Pi	LALRShift [27,53]
[98,104]	var	LALRShift [19,26,37]
[98,104]	(	LALRShift [28,54]
[100,111]	.	LALRShift [110,118]
[105]	Type	LALRShift [20,38]
[105]	Pi	LALRShift [21]
[105]	(	LALRShift [22,40]
[105]	var	LALRShift [19,26,37]
[107]	.	LALRShift [115]
[108,112]	.	LALRReduce 29
[108,112]	var	LALRReduce 29
[108,112]	(	LALRReduce 29
[108,112]	)	LALRReduce 29
[109,113]	.	LALRShift [116,119]
[110,118]	Pi	LALRShift [27,53]
[110,118]	var	LALRShift [19,26,37]
[110,118]	(	LALRShift [28,54]
[114,120]	.	LALRReduce 14
[114,120]	)	LALRReduce 14
[115]	Type	LALRShift [20,38]
[115]	Pi	LALRShift [39]
[115]	(	LALRShift [22,40]
[115]	var	LALRShift [19,26,37]
[116,119]	Lam	LALRShift [45,60]
[116,119]	var	LALRShift [43,58]
[116,119]	(	LALRShift [44,59]
[117,121]	.	LALRReduce 17
[117,121]	)	LALRReduce 17
[122,123]	.	LALRReduce 24
[122,123]	)	LALRReduce 24

[0]	Program	[1]
[0]	Decl	[2]
[0]	TypeDeclaration	[3]
[3]	TermDeclaration	[5]
[4]	TyDecls	[7]
[5]	DefDeclaration	[9]
[6]	TmDecls	[11]
[10]	DefDecls	[14]
[13]	K	[17]
[13]	A1	[18]
[16]	A	[24]
[16]	A1	[25,52]
[22,40]	K	[34,69]
[22,40]	A	[35,51,70]
[22,40]	A1	[36]
[23]	M	[41]
[23]	M1	[42,57]
[28,54]	A	[35,51,70]
[28,54]	A1	[25,52]
[29]	TyDecls	[55]
[31,48,66]	M	[56,77,89]
[31,48,66]	M1	[42,57]
[32]	K	[61,90]
[32]	A1	[18]
[44,59]	M	[74,86]
[44,59]	M1	[42,57]
[46]	TmDecls	[76]
[49,81]	A	[78,91]
[49,81]	A1	[25,52]
[62]	A	[88]
[62]	A1	[25,52]
[67]	K	[61,90]
[67]	A	[78,91]
[67]	A1	[36]
[71]	DefDecls	[95]
[73,85]	M	[96,102]
[73,85]	M1	[42,57]
[79,101]	A	[100,111]
[79,101]	A1	[25,52]
[92]	A	[107]
[92]	A1	[25,52]
[98,104]	A	[109,113]
[98,104]	A1	[25,52]
[105]	K	[114,120]
[105]	A1	[18]
[110,118]	A	[117,121]
[110,118]	A1	[25,52]
[115]	K	[114,120]
[115]	A	[117,121]
[115]	A1	[36]
[116,119]	M	[122,123]
[116,119]	M1	[42,57]


 */


