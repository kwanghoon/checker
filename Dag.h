
#ifndef DAG_H
#define DAG_H

enum Tag {
  Tag_Type, Tag_PiK,
  Tag_ConstA, Tag_AppA, Tag_PiA,
  Tag_Var, Tag_ConstM, Tag_App, Tag_Lam,

  Tag_Name
};

#define UNUSED   (-1)
#define OPTIONAL (-2)

typedef struct _DAG DAG;

struct _DAG {
	enum Tag tag;
	int arg1;
	int arg2;
	int type;
};

/* Kind */
void mkType(DAG dag[], int i);
void mkPiK(DAG dag[], int i, int v, int k);

/* Type */
void mkPiA(DAG dag[], int i, int v, int t);
void mkAppA(DAG dag[], int i, int f, int a);
void mkConstA(DAG dag[], int i, int t);

/* Term */
void mkLam (DAG dag[], int i, int v, int tm);
void mkApp(DAG dag[], int i, int f, int a);
void mkConstM(DAG dag[], int i, int t);
void mkVar(DAG dag[], int i, int db_index, int buf_index, int t);

/* Temporary : either a ConstM or a Var */
void mkName(DAG dag[], int i, int buf_index, int str_length);

#endif  // DAG_H
