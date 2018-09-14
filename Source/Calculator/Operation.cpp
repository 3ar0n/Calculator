#include "Calculator.h"

//Phép cộng - addition
QInt operator + (QInt a, QInt b)
{
	QInt c;
	if (!a.overflow && !b.overflow)
	{
		int tmp = 0;
		for (int i = Base - 1; i >= 0; i--)
		{
			c.data[i] = (a.data[i] + b.data[i] + tmp) % 256;
			tmp = (a.data[i] + b.data[i] + tmp) / 256;
		}
		c.overflow = false;
		if ((a.data[0] <= 127 && b.data[0] <= 127 && c.data[0] > 127) || (a.data[0] > 127 && b.data[0] > 127 && c.data[0] <= 127))
			c.overflow = true;
	}
	else
		c.overflow = true;
	return c;
}

//Phép trừ - subtraction
QInt operator - (QInt a, QInt b)
{
	return (a + Minus(b));
}

//Phép nhân - multiplication
QInt operator * (QInt a, QInt b)
{
	QInt D;
	D.overflow = false;
	bool P, Q = 0x0;

	for (int i = 0; i < Base; i++)
		D.data[i] = 0;

	for (int j = 0; j < Base * Byte; j++)
	{
		P = a.data[Base - 1] & 0x1;
		if (P == true && Q == false)
			D = D - b;
		else if (P == false && Q == true)
			D = D + b;
		
		//right shift
		Q = a.data[Base - 1] & 0x1;			
		for (int i = Base - 1; i > 0; i--)
		{
			a.data[i] = a.data[i] >> 1;
			a.data[i] += (a.data[i - 1] & 0x1) * ((unsigned char)1 << 7);
		}
		a.data[0] = a.data[0] >> 1;
		a.data[0] += (D.data[Base - 1] & 0x1) * ((unsigned char)1 << 7);
		for (int i = Base - 1; i > 0; i--)
		{
			D.data[i] = D.data[i] >> 1;
			D.data[i] += (D.data[i - 1] & 0x1) * ((unsigned char)1 << 7);
		}
		D.data[0] = D.data[0] >> 1;
		if (D.data[0] > 63)
			D.data[0] += (unsigned char)1 << 7;
	}

	//Kiểm tra tràn số (số QInt D chỉ toàn dãy bit1 hoặc bit0 => không tràn)
	int k = 0;
	bool theSame = true;
	while (k < Base)
	{
		if ((D.data[0] == 0 || D.data[0] == 255) && (D.data[k] != D.data[0]))
		{
			a.overflow = true;
			theSame = false;
			break;
		}
		k++;
	}
	if (theSame)
	{
		if ((D.data[0] == 0 && a.data[0] > 127) || (D.data[0] == 255 && a.data[0] <= 127))
			a.overflow = true;
	}
	return a;
}

//Phép chia - division
QInt operator / (QInt a, QInt b)
{
	bool sign = false;
	if ((a.data[0] > 127 && b.data[0] <= 127) || (a.data[0] <= 127 && b.data[0] > 127))
		sign = true;

	//Chuyển sang phép chia không dấu
	if (a.data[0] > 127)
		a = Minus(a);
	if (b.data[0] > 127)
		b = Minus(b);
		
	//khởi tạo số c gồm 128bit 0 (nếu a dương) hoặc 128bit 1 (nếu a âm)
	QInt c, Sub;
	c.overflow = false;
	unsigned char tmp;
	if (a.data[0] > 127)
		tmp = 255;
	else
		tmp = 0;
	for (int i = 0; i < Base; i++)
		c.data[i] = tmp;

	//bool *tmp1, *tmp2, *tmp3;

	for (int j = 0; j < Base * Byte; j++)
	{
		//left shift
		for (int i = 0; i < Base - 1; i++)
		{
			c.data[i] = c.data[i] << 1;
			c.data[i] += (c.data[i + 1] >> 7) & 0x1;
		}
		c.data[Base - 1] = c.data[Base - 1] << 1;
		c.data[Base - 1] += (a.data[0] >> 7) & 0x1;
		for (int i = 0; i < Base - 1; i++)
		{
			a.data[i] = a.data[i] << 1;
			a.data[i] += (a.data[i + 1] >> 7) & 0x1;
		}
		a.data[Base - 1] = a.data[Base - 1] << 1;

		//Kiểm tra c - b
		Sub = c - b;
		if (Sub.data[0] <= 127)	//tức c - b >= 0
		{
			c = c - b;
			a.data[Base - 1] += 0x1;
		}
	}
	if (sign)
		a = Minus(a);
	return a;
}