#include "Calculator.h"

//Đưa con trỏ console đến tọa độ (x, y)
void gotoxy(int x, int y)
{
	static HANDLE h = NULL;
	if (!h)
		h = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD c = { x, y };
	SetConsoleCursorPosition(h, c);
}

//Hàm nhập
void ScanQInt(QInt &x)
{
	string DEC;
	cout << "DEC: ";
	cin >> DEC;
	while (CheckSign(DEC) == 0)
	{
		cout << "Khong hop le. Nhap lai: ";
		cin >> DEC;
	}
	x = DecToByte(DEC);
}

//Hàm nhập chuỗi (DEC/HEX/BIN)
void inputString(int x, int y, string &s, bool &exit, int mode)
{
	char ch;
	int xLen = s.length() % 64;
	int yLen = s.length() / 64;
	gotoxy(x + xLen, y + yLen);
	ch = _getch();

	unsigned int maxLength;
	switch (mode)
	{
	case _Bin: maxLength = 128; break;
	case _Dec: maxLength = 38; break;
	case _Hex: maxLength = 32; break;
	}

	if (ch == 8 && s.length() > 0) //backspace
	{
		gotoxy(x + xLen - 1, y + yLen);
		if (xLen == 0 && yLen >= 1)
			gotoxy(x + 63, y + yLen - 1);
		printf(" ");
		s.erase(s.length() - 1, 1);
	}
	else if (ch == Esc)
		exit = 1;
	else if (ch == '-' && s.length() == 0 && mode == _Dec)
	{
		s += ch;
		gotoxy(x + xLen, y + yLen);
		printf("%c", ch);
	}
	else if ((ch == '0' || ch == '1') && s.length() < maxLength && mode == _Bin)
	{
		s += ch;
		gotoxy(x + xLen, y + yLen);
		printf("%c", ch);
	}
	else if ((ch >= '0' && ch <= '9') && s.length() < maxLength && mode != _Bin)
	{
		s += ch;
		gotoxy(x + xLen, y + yLen);
		printf("%c", ch);
	}
	else if ((ch >= 'A' && ch <= 'F') && s.length() < maxLength && mode == _Hex)
	{
		s += ch;
		gotoxy(x + xLen, y + yLen);
		printf("%c", ch);
	}
	else if ((ch >= 'a' && ch <= 'f') && s.length() < maxLength && mode == _Hex)
	{
		ch -= 'a' - 'A';
		s = s + ch;
		gotoxy(x + xLen, y + yLen);
		printf("%c", ch);
	}
	//return s;
}

//Hàm xuất (hệ cơ số 256)
void PrintQInt(QInt x)
{
	cout << "BASE-256" << endl;
	for (int i = 0; i < Base; i++)
		cout << (unsigned int)x.data[i] << " ";
	cout << endl << endl;
}

//Hàm xuất (DEC, HEX, BIN)
void outPut_QInt(QInt x)
{
	//Thập phân
	string Dec = ByteToDec(x);
	gotoxy(0, 5);
	cout << "DEC" << endl;
	cout << Dec << "    " << endl << endl;

	//Thập lục phân
	char* Hex = ByteToHex(x);
	gotoxy(0, 8);
	cout << "HEX" << endl;
	if (Hex)
	{
		for (int i = 0; i < 32; i++)
		{
			if (i % 4 == 0 && i > 0)
				cout << " ";
			cout << Hex[i];
		}
		delete[] Hex;
	}
	else
		cout << "overflow";

	//Nhị phân
	bool* Bin = ByteToBin(x);
	gotoxy(0, 11);
	cout << "BIN" << endl;
	if (Bin)
	{
		for (int i = 0; i < 128; i++)
		{
			if (i % 4 == 0 && i > 0)
				cout << " ";
			cout << Bin[i];
		}
		delete[] Bin;
	}
	else
		cout << "overflow";
	cout << endl << endl;
}

//Chuyển đổi các hệ cơ số
void Converter(int mode)
{
	string s = "";
	QInt x;
	do
	{
		bool exit = 0;

		gotoxy(0, 0);
		cout << "Input: ";
		switch (mode)
		{
		case _Bin: cout << "BIN"; break;
		case _Dec: cout << "DEC"; break;
		case _Hex: cout << "HEX"; break;
		}

		inputString(0, 1, s, exit, mode);
		switch (mode)
		{
		case _Bin: x = BinToByte(BinStringToBool(s)); break;
		case _Dec: x = DecToByte(s); break;
		case _Hex: x = HexToByte(s); break;
		}

		outPut_QInt(x);

		if (exit == 1)
			break;
	} while (1);
}

//Kiểm tra tính hợp lệ của phép tính (dạng chuỗi)
bool CheckOperation(string s, int mode)
{
	string sample[4];
	sample[0] = "01+-*/";
	sample[1] = "0123456789+-*/";
	sample[2] = "0123456789abcdefABCDEF+-*/";
	sample[3] = "+-*/";
	int num;
	switch (mode)
	{
	case _Bin: num = 0; break;
	case _Dec: num = 1; break;
	case _Hex: num = 2; break;
	}
	if (s[0] == '-')
	{
		if (mode == _Dec)
			s.erase(0, 1);
		else
			return false;
	}

	int OperatoR = 0;
	bool check = true;

	for (unsigned int i = 0; i < s.length(); i++)
	{
		for (unsigned int j = 0; j < 4; j++)
		{
			if (s[i] == sample[3][j])
				OperatoR++;
		}
		for (unsigned int k = 0; k < sample[num].length(); k++)
		{
			check = false;
			if (s[i] == sample[num][k])
			{
				check = true;
				break;
			}
		}
	}
	if (OperatoR != 1)
		check = false;
	return check;
}

//Tính toán phép tính
void Operation(int mode)
{
	string s;
	cout << "Input (without space): " << endl;
	cin >> s;

	//Kiểm tra tính hợp lệ của chuỗi nhập vào
	bool check = CheckOperation(s, mode);
	if (check)
	{
		int i = s.length() - 1;
		char oP;

		//tách 2 chuối số từ chuối nhập vào
		string sa, sb;
		while (i >= 0)
		{
			if (s[i] == '+' || s[i] == '-' || s[i] == '*' || s[i] == '/')
			{
				oP = s[i];
				break;
			}
			i--;
		}
		sa = s.substr(0, i);
		sb = s.substr(i + 1, s.length() - 1 - i);

		//Tính phép tính
		QInt a, b, c;
		switch (mode)
		{
		case _Bin:
			a = BinToByte(BinStringToBool(sa));
			b = BinToByte(BinStringToBool(sb));
			break;
		case _Dec:
			a = DecToByte(sa);
			b = DecToByte(sb);
			break;
		case _Hex:
			a = HexToByte(sa);
			b = DecToByte(sb);
		}
		switch (oP)
		{
		case '+': c = a + b; break;
		case '-': c = a - b; break;
		case '*': c = a * b; break;
		case '/': c = a / b; break;
		}

		outPut_QInt(c);
	}
	else
		cout << "SYNTAX ERROR";
}