#include <iostream>
#include <string>
#include <stdio.h>
#include <conio.h>
#include <windows.h>
using namespace std;

#define Base (int)16
#define Byte (int)8
#define _Bin (int)2
#define _Dec (int)10
#define _Hex (int)16

#define Enter 13
#define Esc 27
#define Up 22472
#define Left 22475
#define Right 22477
#define Down 22480

//	#define o_PLUS +
//	#define o_MINUS -
//	#define o_TIMES *
//	#define o_OBELUS /

struct QInt
{
	unsigned char data[Base];
	bool overflow;
};

typedef struct menu * opt;
struct menu
{
	int value;
	char text[30];
	opt next;
	opt prev;
};

//Console.cpp
void gotoxy(int x, int y);
//QInt inputQInt(string s);
void inputString(int x, int y, string &s, bool &exit, int mode = _Dec);
void PrintQInt(QInt x);
void outPut_QInt(QInt x);
void Converter(int mode);
bool CheckOperation(string s, int mode);
void Operation(int mode);

//Convert.cpp
bool getBit(unsigned char byte, int position);
bool* BinStringToBool(string s);
char NumberToHex(int n);
int HexToNumber(char ch);
QInt Minus(QInt x);
int CheckSign(string s);
QInt DecToByte(string s);
QInt BinToByte(bool *bit);
QInt HexToByte(string s);
string ByteToDec(QInt x);
bool* ByteToBin(QInt x);
char* ByteToHex(QInt x);

//Operation.cpp
QInt operator + (QInt a, QInt b);
QInt operator - (QInt a, QInt b);
QInt operator * (QInt a, QInt b);
QInt operator / (QInt a, QInt b);