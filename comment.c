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
				if (second == '*')            //NULL״̬������/*ʱ������Ϊ//������C״̬  
				{
					fputc('/', pfWrite);
					fputc('/', pfWrite);
					state = C_STATE;
				}
				else if (second == '/')       //NULL״̬������//ʱֱ�����  
				{
					fputc(first, pfWrite);
					fputc(second, pfWrite);
					state = CPP_STATE;
				}
				else                         //NULL״̬������/�������������ַ�����ֱ�����  
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
				if (second == '/')       //ƥ������  
				{
					third = fgetc(pfRead);

					//����ע��������ע������  
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
					//���'*/'֮����'\n'��˵����second֮�����ַ�������ڴ˴�����һ�����У�����  
					//������third�ַ����ص�pfRead��ȥ������״̬��Ϊ NUL_STATE  
					//�����'\n'��˵��c����ע�����꣬��һ���ַ��п��ǡ�/��Ҳ�п����������ַ������  
					//��״̬�ص� NUL_STATE  
				}
				else if (second == '*')   //������**/����  
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
	case '\n':                   //CPP״̬������'\n'��˵��ǰһ��ע�������  
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

	printf("ת����ʼ\n");
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
	printf("ת������\n");
	system("pause");
	return 0;
}
