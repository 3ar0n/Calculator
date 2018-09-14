#include "Calculator.h"


//Lấy giá trị của bit tương ứng trên ô nhớ
bool getBit(unsigned char byte, int position)
{
	//dịch phải rồi and với bit 1
	return (byte >> position) & 0x1;
}

//Chuyển dãy bit từ dạng chuỗi => dạng luận lý
bool* BinStringToBool(string s)
{
	bool* BIN = new bool[Base * Byte];  // 16 x 1 byte = Số lớn 16 bytes
	if (s.length() <= Base * Byte)
	{
		int i = s.length() - 1;
		int j = Base * Byte - 1;
		// chuyển đổi theo chiều từ phải sang trái
		do
		{
			if (i >= 0)
				BIN[j] = (s[i] - '0');
			else //bổ sung các giá trị 0 phía trước cho đủ 16 * 8 phần tử
				BIN[j] = 0;
			j--;
			i--;
		} while (j >= 0);
	}
	return BIN;
}

//Chữ số thập phân => ký tự thập lục phân
char NumberToHex(int n)
{
	char ch;
	switch (n)
	{
	case 0: case 1: case 2: case 3: case 4: case 5: case 6: case 7: case 8: case 9:
		ch = n + '0';
		break;
	case 10: ch = 'A'; break;
	case 11: ch = 'B'; break;
	case 12: ch = 'C'; break;
	case 13: ch = 'D'; break;
	case 14: ch = 'E'; break;
	case 15: ch = 'F'; break;
	default: ch = ' ';
	}
	return ch;
}

//Ký tự thập lục phân => chứ số thập phân
int HexToNumber(char ch)
{
	int n;
	switch (ch)
	{
	case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
		n = ch - '0';
		break;
	case 'A': n = 10; break;
	case 'B': n = 11; break;
	case 'C': n = 12; break;
	case 'D': n = 13; break;
	case 'E': n = 14; break;
	case 'F': n = 15; break;
	default: n = -1;
	}
	return n;
}

//Phép lấy bù 2 của 1 số nguyên khác 0
QInt Minus(QInt x)
{
	QInt y;
	y.overflow = x.overflow;
	for (int i = 0; i < Base; i++)
	{
		y.data[i] = ~x.data[i]; //thực hiện đảo toàn bộ 8 bit của 1 byte phần tử)
	}

	//cộng thêm 1 vào số vừa đảo bit
	//nếu giá trị tại byte đó = 255 thì đổi giá trị = 0 và cộng 1 cho byte bên trái
	for (int i = Base - 1; i >= 0; i--)
	{
		if (y.data[i] == 255)
			y.data[i] = 0;
		else
		{
			y.data[i] += 1;
			break;
		}
	}
	return y;
	// note: ngoại trừ số 0, giá trị byte đầu tiên (chứa bit dấu) sau khi đảo bit luôn < 255
	// dĩ nhiên nếu là số 0 thì không lấy bù 2
}

//Kiểm tra dấu của chuỗi nhập vào (chuỗi DEC)
int CheckSign(string s)
{
	for (unsigned int i = 1; i < s.length(); i++)
	{
		if (s[i] < '0' || s[i] > '9')
			return 0;
	}
	if (s[0] == '-')
		return -1;
	else if (s[0] < '0' || s[0] > '9')
		return 0;
	return 1;
	// số hợp lệ dương: trả về 1
	// số hợp lệ âm: trả về -1
	// số không hợp lệ: trả về 0
}

//Thập phân => cơ số 256 (1Byte - 8bit)
QInt DecToByte(string s)
{
	QInt x;
	x.overflow = false;
	for (int i = 0; i < Base; i++)
		x.data[i] = 0;
	
	unsigned int tmp, mod, i, j = 0;
	string div, DEC = s;
	bool isZero = false;
	
	//Kiểm tra dấu (và số 0)
	int Sign = CheckSign(s);
	if (Sign == -1)
	{
		DEC.erase(0, 1);							//Loại bỏ dấu -
		while (DEC.length() > 1 && DEC[0] == '0')	//Loại bỏ các số 0 ở đầu
		{
			DEC.erase(0, 1);
		}
		if (DEC[0] == '0')
			isZero = true;
		// Chuỗi DEC cần chuyển sang QInt có giá trị dương, sau khi chuyển sẽ bù 2 nếu giá trị ban đầu là âm
	}

	// Thực hiện chuyển cơ số, bằng cách chia (lần lượt từng phần nhỏ của chuỗi DEC cho 256)
	do
	{
		div = "";
		mod = 0;
		i = 0;
		while (i < DEC.length())
		{
			mod = mod * 10 + (unsigned int)DEC[i] - '0';
			i++;
			tmp = mod / 256;
			if (tmp != 0 || (tmp == 0 && div.length() != 0))
				div = div + (char)(tmp + '0');
			if (div[0] != '0')
				mod = mod % 256;
		}
		if (div[0] != '0')
			DEC = div;
		x.data[Base - 1 - j] = mod;

		// Kiểm tra tràn số, giá trị dương nên bit đầu tiên = 0 hay byte đầu tiên < 128
		if (j == Base - 1)
		{
			if (mod > 127)
				x.overflow = true;
			break;
		}
		j++;
	} while (DEC.length() != 0);

	if (Sign == -1 && !isZero)
		x = Minus(x);
	return x;
}

//Nhị phân => cơ số 256
QInt BinToByte(bool *bit)
{
	QInt x;
	x.overflow = false;
	int i, j;
	for (i = 0; i < Base; i++)
	{
		x.data[i] = 0;
	}

	// Thực hiện chuyển từ phải sang trái
	// k là vị trái trên chuỗi BIN
	int k = Base * Byte - 1;
	while (k >= 0)
	{
		// i là vị trí của byte đang thao tác của QInt
		i = Base - 1;
		do
		{
			// j là vị trí bit trong byte đang thao tác, j = 0 là vị trí cuối cùng (bit bé nhất)
			j = 0;
			do
			{
				x.data[i] += bit[k] * ((unsigned char)1 << j);
				k--;
				j++;
			} while (j < Byte);
			i--;
		} while (i >= 0);
	}
	return x;
}

//Thập lục phân => cơ số 256
// Chuỗi HEX phải có số lượng chẵn kí tự (mỗi 2 kí tự chuyển thành 1 byte QInt)
QInt HexToByte(string s)
{
	QInt x;
	x.overflow = false;
	for (int k = 0; k < Base; k++)
		x.data[k] = 0;
	int i, len = s.length();

	// kiểm tra tràn số
	if (len > 32)
	{
		x.overflow = true;
		return x;
	}
	else if (len % 2 != 0) // thêm '0' đằng trước nếu chuỗi HEX có số lẻ kí tự
	{
		s = '0' + s;
		len++;
	}

	// chuyển đổi
	i = Base - 1;
	for (int j = len - 1; j > 0; j = j - 2)
	{
		x.data[i] = HexToNumber(s[j - 1]) * 16 + HexToNumber(s[j]);
		i--;
	}
	return x;
}

//Cơ số 256 => nhị phân
bool* ByteToBin(QInt x)
{
	if (x.overflow)
		return NULL;
	else
	{
		bool* BIN = new bool[Base * Byte];
		int k = 0;

		for (int i = 0; i < Base; i++)
		{
			for (int j = Byte - 1; j >= 0; j--)
			{
				BIN[k] = getBit(x.data[i], j);
				k++;
			}
		}
		return BIN;
	}
}

//Cơ số 256 => thập lục phân
char* ByteToHex(QInt x)
{
	if (x.overflow)
		return NULL;
	else
	{
		char* HEX = new char[Base * 2];
		int k = 0;
		for (int i = 0; i < Base; i++)
		{
			HEX[k] = NumberToHex(x.data[i] / 16);
			HEX[k + 1] = NumberToHex(x.data[i] % 16);
			k += 2;
		}
		return HEX;
	}
}

//Cơ số 256 => thập phân
string ByteToDec(QInt x)
{
	string DEC = "";
	int Sign = 0;
	if (x.overflow)
		DEC = "overflow";
	else
	{
		if (x.data[0] > 127)
		{
			Sign = -1;
			x = Minus(x);
		}
		unsigned int i = 0;
		while (x.data[i] == 0 && i < Base)
			i++;

		if (i >= Base)
			DEC = "0";
		else
		{
			string str = "", s = "";
			unsigned int tmp = 0;

			//tính S1 (do S0 = 0 và S1 < 65281) 
			tmp = x.data[i];
			do
			{
				s = (char)(tmp % 10 + '0') + s;
				tmp = tmp / 10;
			} while (tmp != 0);
			i++;
			while (i < Base)
			{
				//Nhân S_i với 256 => DEC
				for (int j = s.length() - 1; j >= 0; j--)
				{
					tmp = (s[j] - '0') * 256 + tmp;
					if (j != 0)
					{
						DEC = (char)(tmp % 10 + '0') + DEC;
						tmp = tmp / 10;
					}
					else
					{
						do
						{
							DEC = (char)(tmp % 10 + '0') + DEC;
							tmp = tmp / 10;
						} while (tmp != 0);
					}
				}
				//tách 3 giá trị cuối từ DEC
				str = DEC.substr(DEC.length() - 3, 3);
				tmp = 0;
				for (int j = 0; j < 3; j++)
				{
					tmp = tmp * 10 + (str[j] - '0');
				}
				//rồi cộng với phần dư: x.data[i]
				tmp = tmp + x.data[i];

				//nếu giá trị > 1000 => tăng các phía trước lên 1 đơn vị
				if (tmp >= 1000)
				{
					DEC = '0' + DEC;
					tmp = tmp % 1000;
					int k = 3;
					while (DEC[DEC.length() - 1 - k] == '9')
					{
						DEC = '0' + DEC;
						DEC[DEC.length() - 1 - k] = '0';
						k++;
					}
					DEC[DEC.length() - 1 - k] += 1;
				}
				//trả ngược 3 ký tự cuối về chuỗi DEC
				for (int j = 2; j >= 0; j--)
				{
					DEC[DEC.length() - 3 + j] = (char)(tmp % 10 + '0');
					tmp = tmp / 10;
				}

				s = DEC;
				while (s[0] == '0')
				{
					s.erase(0, 1);
				}
				DEC = "";
				i++;
			}
			DEC = s;
		}
		if (Sign == -1)
			DEC = "-" + DEC;
	}
	return DEC;
}

/*
Số hệ cơ số 256 có dạng [a0 a1 a2 .... ai] (với 0 <= ai < 256) chuyển sang hệ cơ số 10 theo công thức
S1 = 256 * S0 + a0 = a0		(với S0 = 0)
S2 = 256 * S1 + a1
...
X  = 256 * Si + ai
*/