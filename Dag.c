#include "Dag.h"

/* Kind */
void mkType(DAG dag[], int i)
{
  dag[i].tag  = Tag_Type;  // Type
  dag[i].arg1 = UNUSED;
  dag[i].arg2 = UNUSED;
  dag[i].type = UNUSED;
}

void mkPiK(DAG dag[], int i, int v, int k)
{
  dag[i].tag  = Tag_PiK;   // PiK
  dag[i].arg1 = v;         // variable
  dag[i].arg2 = k;         // kind
  dag[i].type = OPTIONAL;
}

/* Type */
void mkPiA(DAG dag[], int i, int v, int t)
{
  dag[i].tag  = Tag_PiA;    // PiA
  dag[i].arg1 = v;          // variable
  dag[i].arg2 = t;          // type
  dag[i].type = OPTIONAL;
}

void mkAppA(DAG dag[], int i, int f, int a)
{
  dag[i].tag  = Tag_AppA;   // AppA
  dag[i].arg1 = f;          // function
  dag[i].arg2 = a;          // argument
  dag[i].type = OPTIONAL;
}

void mkConstA(DAG dag[], int i, int t)
{
  dag[i].tag  = Tag_ConstA; // ConstA
  dag[i].arg1 = UNUSED;  
  dag[i].arg2 = UNUSED;  
  dag[i].type = t;          // type
}

/* Term */
void mkLam (DAG dag[], int i, int v, int tm)
{
  dag[i].tag  = Tag_Lam;     // Lam
  dag[i].arg1 = v;           // variable
  dag[i].arg2 = tm;          // term
  dag[i].type = OPTIONAL;
}

void mkApp(DAG dag[], int i, int f, int a)
{
  dag[i].tag  = Tag_App;    // App
  dag[i].arg1 = f;          // function
  dag[i].arg2 = a;          // argument
  dag[i].type = OPTIONAL;
}

void mkConstM(DAG dag[], int i, int t)
{
  dag[i].tag  = Tag_ConstM; // ConstA
  dag[i].arg1 = UNUSED;  
  dag[i].arg2 = UNUSED;  
  dag[i].type = t;          // type
}

void mkVar(DAG dag[], int i, int db_index, int buf_index, int t)
{
  dag[i].tag  = Tag_Var;    // Var
  dag[i].arg1 = db_index;   // de bruijn index
  dag[i].arg2 = buf_index;  // buffer index
  dag[i].type = t;          // type
}

/* Utility */
void addBVar(int const_list[], int ptr, int buf_index, int str_length, int dag_index)
{
  const_list[ptr]   = buf_index;
  const_list[ptr+1] = str_length;
  const_list[ptr+2] = dag_index;
}

int searchBVar(int const_list[], int len, char buf[], int buf_index, int str_len)
{
  int i;
  int j;
  int found;

  found = -1;
  for (i=0; found==-1 && i<len; i=i+2)
    {
      int buf_index2;
      int str_len2;
      int flag;

      buf_index2 = const_list[i];
      str_len2   = const_list[i+1];

      flag = 1;
      if (str_len == str_len2) {
	for (j=0; flag==1 && j<str_len; j++)
	  {
	    if (buf[buf_index+j] == buf[buf_index2+j])
	      flag = 1;
	    else
	      flag = 0;
	  }
      }
      else
	flag = 0;

      if (flag == 1)
	found = i;
      else
	found = -1;
    }

  return found;
}
