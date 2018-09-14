#include "Calculator.h"

int Read_Key();
void CreateMenu(opt *head);
void CreateBase(opt *head);
int Select(opt head, int x, int y);

int main()
{
	opt MENU, BASE;
	CreateMenu(&MENU);
	CreateBase(&BASE);
	int choose, chose;
	do
	{
		system("cls");
		choose = Select(MENU, 5, 1);
		switch (choose)
		{
			case 1:
				system("cls");
				chose = Select(BASE, 5, 1);
				system("cls");
				Converter(chose);
				break;
			case 2:
				system("cls");
				chose = Select(BASE, 5, 1);
				system("cls");
				Operation(chose);
				cout << endl << "Press any key to get back to the main menu";
				int KEY = Read_Key();
				break;
		}
	} while (1);

	delete MENU, BASE;
	return 0;
}

int Read_Key()
{
	int key = -1;
	switch (_getch())
	{
		case 13: key = Enter; break;
		case 27: key = Esc; break;
		case 224:
			switch (_getch())
			{
				case 72: key = Up; break;
				case 75: key = Left; break;
				case 77: key = Right; break;
				case 80: key = Down; break;
			}	
			break;
	}
	return key;
}

void CreateMenu(opt *head)
{
	opt p1 = new menu;
	p1->value = 1;
	strcpy_s(p1->text, "CONVERTER");

	opt p2 = new menu;
	p2->value = 2;
	strcpy_s(p2->text, "OPERATION");

	*head = p1;
	p1->prev = NULL;
	p2->prev = p1;
	p1->next = p2;
	p2->next = NULL;
}

void CreateBase(opt *head)
{
	opt p1 = new menu;
	p1->value = _Bin;
	strcpy_s(p1->text, "BIN");

	opt p2 = new menu;
	p2->value = _Dec;
	strcpy_s(p2->text, "DEC");

	opt p3 = new menu;
	p3->value = _Hex;
	strcpy_s(p3->text, "HEX");

	*head = p1;
	p1->prev = NULL;
	p2->prev = p1;
	p3->prev = p2;
	p1->next = p2;
	p2->next = p3;
	p3->next = NULL;
}

int Select(opt head, int x, int y)
{
	int Key, i = 0;
	opt p = head;
	while (p)
	{
		gotoxy(x + i * 16, y);
		cout << "  " << p->text << "  ";
		i++;
		p = p->next;
	}
	p = head;
	gotoxy(x, y);
	cout << "[ " << p->text << " ]";
	gotoxy(0, 0);
	do
	{
		Key = Read_Key();
		if (Key == Left || Key == Right)
		{
			if (Key == Left && p->prev)
			{
				gotoxy(x, y);
				cout << "  " << p->text << "  ";

				p = p->prev;
				x = x - 16;

				gotoxy(x, y);
				cout << "[ " << p->text << " ]";
			}
			if (Key == Right && p->next)
			{
				gotoxy(x, y);
				cout << "  " << p->text << "  ";

				p = p->next;
				x = x + 16;

				gotoxy(x, y);
				cout << "[ " << p->text << " ]";
			}
		}
		gotoxy(0, 0);
	} while (Key != Enter);
	return p->value;
}