#ifndef _COMMERNT_CONVER__
#define _COMMERNT_CONVER__

#include<stdio.h>  
#include<stdlib.h>  


#define INPUTNAME "input.c"  
#define OUTPUTNAME "ouput.c"  

enum STATE
{
	NULL_STATE,
	C_STATE,
	CPP_STATE,
	END_STATE
};

void CommentConvert(FILE *pfRead, FILE *pfWrite);
void Do_NULL_State(FILE *pfRead, FILE *pfWrite);
void Do_C_State(FILE *pfRead, FILE *pfWrite);
void Do_Cpp_State(FILE *pfRead, FILE *pfWrite);



#endif //_COMMERNT_CONVER__

#include "Comment.h"

enum STATE state = NULL_STATE;

void CommentConvert(FILE *pfRead, FILE *pfWrite)
{
	while (state != END_STATE)
	{
		switch (state)
		{
		case C_STATE:
			Do_C_State(pfRead, pfWrite);
			break;
		case CPP_STATE:
			Do_Cpp_State(pfRead, pfWrite);
			break;
		case NULL_STATE:
			Do_NULL_State(pfRead, pfWrite);
			break;
		case END_STATE:
			break;
		default:
			break;
		}
	}
}

void Do_NULL_State(FILE *pfRead, FILE *pfWrite)
{
	int first = 0;
	int second = 0;
	first = fgetc(pfRead);
	switch (first)
	{
	case '/':
	{
				second = fgetc(pfRead);
				if (second == '*')            //NULL状态下遇到/*时将其置为//并进入C状态  
				{
					fputc('/', pfWrite);
					fputc('/', pfWrite);
					state = C_STATE;
				}
				else if (second == '/')       //NULL状态下遇到//时直接输出  
				{
					fputc(first, pfWrite);
					fputc(second, pfWrite);
					state = CPP_STATE;
				}
				else                         //NULL状态下遇到/后又遇到其他字符将其直接输出  
				{
					fputc(first, pfWrite);
					fputc(second, pfWrite);
				}
	}
		break;
	case EOF:
		state = END_STATE;
	default:
		fputc(first, pfWrite);
		break;
	}
}

void Do_C_State(FILE *pfRead, FILE *pfWrite)
{
	int first = 0;
	int second = 0;
	int third = 0;
	first = fgetc(pfRead);
	switch (first)
	{
	case '*':
	{
				second = fgetc(pfRead);
				if (second == '/')       //匹配问题  
				{
					third = fgetc(pfRead);

					//多行注释与连续注释问题  
					if (third != '\n')
					{
						fputc('\n', pfWrite);
						ungetc(third, pfRead);
					}
					else
					{
						fputc('\n', pfWrite);
					}
					state = NULL_STATE;
					//如果'*/'之后不是'\n'，说明在second之后还有字符，因此在此处输入一个换行，并将  
					//读到的third字符返回到pfRead中去，并将状态置为 NUL_STATE  
					//如果是'\n'，说明c语言注释已完，下一个字符有可是‘/’也有可能是其他字符，因此  
					//将状态回到 NUL_STATE  
				}
				else if (second == '*')   //连续的**/问题  
				{
					third = fgetc(pfRead);
					fputc(first, pfWrite);
					if (third == '/')
						state = NULL_STATE;
				}
				else
				{
					fputc(first, pfWrite);
					fputc(second, pfWrite);
				}
	}
		break;
	case '\n':
		fputc(first, pfWrite);
		fputc('/', pfWrite);
		fputc('/', pfWrite);
		break;
	case EOF:
		state = END_STATE;
		break;
	default:
		fputc(first, pfWrite);
		break;
	}
}

void Do_Cpp_State(FILE *pfRead, FILE *pfWrite)
{
	int first = 0;
	first = fgetc(pfRead);
	switch (first)
	{
	case '\n':                   //CPP状态下遇到'\n'，说明前一行注释已完成  
		fputc(first, pfWrite);
		state = NULL_STATE;
		break;
	case EOF:
		state = END_STATE;
		break;
	default:
		fputc(first, pfWrite);
		break;
	}
}

#define _CRT_SECURE_NO_WARNINGS  

#include"Comment.h"  

int main()
{
	FILE *pfRead = NULL;
	FILE *pfWrite = NULL;

	printf("转换开始\n");
	pfRead = fopen(INPUTNAME, "r");

	if (pfRead == NULL)
	{
		perror("open file for read");
		exit(0);
	}
	pfWrite = fopen(OUTPUTNAME, "w");
	if (pfWrite == NULL)
	{
		fclose(pfRead);
		perror("open file for write");
		exit(0);
	}

	CommentConvert(pfRead, pfWrite);
	printf("转换结束\n");
	system("pause");
	return 0;
}
