#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "Lexer.h"
#include "Parser.h"
#include "Dag.h"

#define BUFSIZE 100000
static char buf[BUFSIZE];

#define TOKSSIZE 100000
static int tokens[TOKSSIZE];

#define DAGSIZE 100000
static DAG dag[DAGSIZE];

void run(char* filename);
int  fillBuf(FILE *fp);
void test();
void prToks(int n);


int main(int argc, char* argv[])
{
	char filename[1024];
	int i;

	if (argc <= 1)
	{
		printf("File name: ");
		gets(filename);
		run(filename);
	}
	else if(argc == 2 && strcmp(argv[1], "-t")==0)
	{
		test();
	}
	else
	{
		for (i=1; i<argc; i++)
			run(argv[i]);
	}

	return 0;
}

int fillBuf(FILE *fp)
{
	int ch;
	int i;

	i = 0;
	while ( (ch = fgetc(fp)) != EOF )
		buf[i++] = (char)ch;

	return i;
}

void prToks(int n)
{
	int i;

	for(i = 0; i < n; i++)
	{
		switch( tokens[i] )
		{
		case TYPE:
			printf("Type");
			break;
		case PI:
			printf("/\\");
			break;
		case LAM:
			printf("\\");
			break;
		case COLON:
			printf(":");
			break;
		case DOT:
			printf(".");
			break;
		case OPEN:
			printf("(");
			break;
		case CLOSE:
			printf(")");
			break;
		case EQ:
			printf("=");
			break;
		case ARROW:
			printf("->");
			break;
		case ATTYPE:
			printf("@type");
			break;
		case ATTERM:
			printf("@term");
			break;
		case ATDEF:
			printf("@def");
			break; 
		case VAR:
			{
				int start;
				int length;
				int j;

				printf("VAR[");

				start  = tokens[i+1];
				length = tokens[i+2];

				for (j = 0; j<length; j++)
					putchar( buf[start+j] );

				printf("]");

				i += 2;
			}
			break;
		case NUM:
			break;
		case ENDOFSYMBOL:
			printf("$\n");
			break;
		default:
			printf("Invalid token: %d\n", tokens[i]);
		}

		printf("\n");
	}
}

#define TESTCASE_DIR ""

char* TestCases[] =
	{
		TESTCASE_DIR "FOL.lf",
		TESTCASE_DIR "HOARE.lf",
		TESTCASE_DIR "HOL.lf",
		TESTCASE_DIR "Int.lf",
		TESTCASE_DIR "Presburger.lf",
		TESTCASE_DIR "STLC.lf",
		TESTCASE_DIR "Vector.lf",
		0
	};

void test()
{
	int i;

	i = 0;
	for (i=0; TestCases[i] != 0; i++)
	{
		run(TestCases[i]);
	}
}

void run(char* filename)
{
	FILE* fp;
	int i;
	int n;

	fp = fopen(filename, "r");
	if (fp == NULL)
	{
		printf("File open error");
		return;
	}

	i = fillBuf(fp);

	n = lexer(buf, i, tokens, TOKSSIZE);
	printf("[%s] Result = %d\n", filename, n);

	if (n > 0)
	  {
		prToks(n);
		printf("Parse result: %d\n",
		       parse(tokens, n, dag, DAGSIZE));
	  }

	fflush(stdout);
	fclose(fp);
}
