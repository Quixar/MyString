#include <iostream>
#include <string>
#include <ctime>
using namespace std;

class String
{
private:
	char* text = nullptr;
	unsigned int length = 0;
	unsigned int capacity = 80;
	friend istream& operator >> (istream& i, String& s);
	friend ostream& operator << (ostream& o, String& s);
public:
	String() : String("", 80) {}

	String(const char* text) : String(text, 80) {}

	String(unsigned int capacity) : String("", capacity) {}

	String(const String& original) : String(original.text, original.capacity) {}

	// main c-tor
	String(const char* text, unsigned int capacity)
	{
		SetString(text, capacity);
	}

private:
	void SetString(const char* text, unsigned int capacity = 80)
	{
		length = strlen(text);

		// для пустых строк делаем резерв памяти 80
		if (length == 0)
		{
			this->capacity = 80;
		}
		else // для НЕпустых - по ситуации
		{
			// нормализация переданного параметра capacity
			if (capacity > length * 10)
				capacity = length * 10;

			if (capacity <= length)
				capacity = length + 1;

			this->capacity = capacity;
		}

		/*cout << "\n\n=================\n\n";
		cout << "capacity param: " << this->capacity << "\n";
		cout << "length param: " << this->length << "\n";
		cout << "\n\n=================\n\n";*/

		if (this->text != nullptr)
			delete[] this->text;

		this->text = new char[this->capacity];
		strcpy_s(this->text, length + 1, text);
	}

public:
	~String()
	{
		if (text != nullptr)
		{
			delete[] text;
			text = nullptr;
		}
	}

	// возвращать из класса указатель на массив чаров нельзя - это нарушит инкапсуляцию и позволит клиенту СЛОМАТЬ содержимое (состояние) объекта
	// если создавать копию массива чаров, то под неё придётся выделять память, а почистить её клиент скорее всего ЗАБУДЕТ 
	// поэтому здесь выдаётся КОПИЯ состояния в виде объекта текущего класса
	String GetString() const
	{
		//char* copy = new char[length + 1];
		//strcpy_s(copy, length + 1, text);
		//return copy;

		String copy = text;
		return copy;
	}

	int GetLength() const
	{
		return length;
	}

	// метода SetLength не должно быть!

	int GetCapacity() const
	{
		return capacity;
	}

	// в целях повшения производительности, не тратится время на:
	// 1) очистку памяти делитом
	// 2) запись в каждый символ пробелов (не запускается цикл фор)
	// 3) не меняется capacity

	// достаточно в начало строки записать \0, и длину выставить в 0
	// многие методы класса ориентируются именно на длину, и если она равна 0, то клиент будет уверен, что строка пустая
	void Clear()
	{
		text[0] = '\0';
		length = 0;
	}

	// если резерв памяти стал значительно больше, чем размер строки, то можно вызвать принудительное перевыделение памяти, суть которого в 
	void ShrinkToFit() // том, чтобы сделать резерв такой же длины, как и length
	{
		if (length + 1 <= capacity)
		{
			return;
		}

		capacity = length + 1;
		char* temp = new char[capacity];
		strcpy_s(temp, capacity, text);
		delete[] text;
		text = temp;
	}

	void Print() const
	{
		cout << "Text: " << text << "\n";
		cout << "Length: " << length << "\n";
		cout << "Capacity: " << capacity << "\n";
	}
	void FromKeybord()
	{
		cout << "Input text: ";
		cin >> text;
		length = strlen(text);
	}
	bool CompareTo(String text2)
	{
		int currLength = 0;
		if (length > text2.length)
		{
			currLength = length;
		}
		else
		{
			currLength = text2.length;
		}
		for (int i = 0; i < currLength; i++)
		{
			if (text[i] != text2.text[i])
			{
				cout << "not equal\n";
				return false;
			}

		}
		cout << "equal\n";
		return true;
	}
	bool Contains(String text2)
	{
		int i = 0;
		int j = 0;
		int start = 0;
		int countOfCoincidence = 0;

		while (i < length)
		{
			if (text[i] == text2.text[j])
			{
				countOfCoincidence++;
				j++;
				i++;
				if (countOfCoincidence == text2.length)
				{
					cout << "true\n";
					return true;
				}
			}
			else
			{
				start++;
				i = start;
				j = 0;
				countOfCoincidence = 0;
			}
		}
		cout << "false\n";
		return false;
	}
	void Concat(String text2)
	{
		int end = length;
		int start = 0;
		length += text2.length;
		ShrinkToFit();
		for (int i = end; i < length; i++)
		{
			text[i] = text2.text[start];
			start++;
		}
		text[length] = '\0';
	}
	bool EndsWith(String text2)
	{
		if (length < text2.length or length == 0 or text2.length == 0)
		{
			throw(length);
			return false;
		}
		int i = 0;
		int j = 0;
		int start = length - text2.length;
		for (int i = start; i < length; i++)
		{
			if (text[i] == text2.text[j])
			{
				j++;

			}
			else
			{
				cout << "false\n";
				return false;
			}
		}
		cout << "true\n";
		return true;
	}
	bool StartWith(String text2)
	{
		if (length < text2.length or length == 0 or text2.length == 0)
		{
			throw(length);
			return false;
		}
		int j = 0;
		for (int i = 0; i < text2.length; i++)
		{
			if (text[i] == text2.text[j])
			{
				j++;
			}
			else
			{
				cout << "false\n";
				return false;
			}
		}
		cout << "true\n";
		return true;
	}
	int IndexOf(char character)
	{
		for (int i = 0; i < length; i++)
		{
			if (text[i] == character)
			{
				cout << "index of character = " << i << "\n";
				return i;
			}
		}
		cout << "Not found\n";
		return -1;
	}
	int LastIndexOf(char character)
	{
		for (int i = length; i >= 0; i--)
		{
			if (text[i] == character)
			{
				cout << "index of character = " << i << "\n";
				return i;
			}
		}
		cout << "Not found\n";
		return -1;
	}
	void Remove(int index)
	{
		if (index < 0 or index >= length)
		{
			throw(index);
		}
		text[index] = '\0';
		length = index;
	}
	void Remove(int start, int count)
	{
		if (start < 0 or start >= length or start + count > length)
		{
			throw(start);
		}
		int j = start + count;
		for (int i = start; i < length; i++)
		{
			text[i] = text[j];
			j++;

		}
		length = length - count;
	}
	void Replace(char oldChar, char newChar)
	{
		for (int i = 0; i < length; i++)
		{
			if (text[i] == oldChar)
			{
				text[i] = newChar;
			}
		}
	}
	String ToLower()
	{
		String lowerString = String(text, capacity);
		for (int i = 0; i < length; i++)
		{
			if (lowerString.text[i] >= 65 and lowerString.text[i] <= 90)
			{
				lowerString.text[i] = char((int)lowerString.text[i] + 32);
			}
			/*else if (lowerString.text[i] >= -112 and lowerString.text[i] <= -80)
			{
				cout << "блабла";
				lowerString.text[i] = char((int)lowerString.text[i] + 32);
			}*/
		}
		return lowerString;
	}
	String ToUpper()
	{
		String upperString = String(text, capacity);
		for (int i = 0; i < length; i++)
		{
			if (upperString.text[i] >= 97 and upperString.text[i] <= 122)
			{
				upperString.text[i] = char((int)upperString.text[i] - 32);
			}
		}
		return upperString;
	}
	void Reverse()
	{
		char temp;
		int end = length - 1;
		for (int i = 0; i < length / 2; i++)
		{
			temp = text[end];
			text[end] = text[i];
			text[i] = temp;
			end--;
		}
	}
	void SortAZ()
	{
		int temp;
		int i = 0;
		int j = 1;
		int countOfCoincidence = 0;
		while (countOfCoincidence < length)
		{
			if (text[i] > text[j] and j < length)
			{
				temp = text[i];
				text[i] = text[j];
				text[j] = temp;
				i++;
				j++;
				countOfCoincidence = 0;
			}
			else if (j == length)
			{
				i = 0;
				j = 1;
			}
			else
			{
				i++;
				j++;
				countOfCoincidence++;
			}
		}
	}
	void SortZA()
	{
		int temp;
		int i = 0;
		int j = 1;
		int countOfCoincidence = 0;
		while (countOfCoincidence < length)
		{
			if (text[i] < text[j] and j < length)
			{
				temp = text[i];
				text[i] = text[j];
				text[j] = temp;
				i++;
				j++;
				countOfCoincidence = 0;
			}
			else if (j == length)
			{
				i = 0;
				j = 1;
			}
			else
			{
				i++;
				j++;
				countOfCoincidence++;
			}
		}
	}
	void Shuffle()
	{
		for (int i = 0; i < length; i++)
		{
			int x = rand() % length;
			int y = rand() % length;
			int temp;
			temp = text[x];
			text[x] = text[y];
			text[y] = temp;
		}
	}
	void RandomFill()
	{
		length = capacity;
		ShrinkToFit();
		for (int i = 0; i < capacity; i++)
		{
			int random = 65 + rand() % (128 - 65 + 1);
			if ((char)random == '0' and text[i - 1] == '\\')
			{
				continue;
			}
			text[i] = (char)random;
		}
	}

	operator string()
	{
		return text;
	}
};

istream& operator >> (istream& i, String& s) {
	cout << "Write your text: ";
	cin >> s.text;
	s.length = strlen(s.text);
	s.capacity = s.length + 1;
	return cin;
}

ostream& operator << (ostream& o, String& s)
{
	cout << s.text;
	return cout;
}

bool operator < (String& left, String& right)
{
	if (left.GetLength() < right.GetLength())
	{
		cout << "First string lesst hen string\n";
		return true;
	}
	return false;
}

bool operator <= (String& left, String& right)
{
	if (left.GetLength() <= right.GetLength())
	{
		cout << "First string less or equal then second string\n";
		return true;
	}
	return false;
}

bool operator > (String& left, String& right)
{
	if (left.GetLength() > right.GetLength())
	{
		cout << "First string greater then second string\n";
		return true;
	}
	return false;
}

bool operator >= (String& left, String& right)
{
	if (left.GetLength() >= right.GetLength())
	{
		cout << "First string greater or equal then second string\n";
		return true;
	}
	return false;
}

bool operator == (String& left, String& right) {
	return left.CompareTo(right);
}

bool operator != (String& left, String& right)
{
	return !(left == right);
}

int main()
{
	setlocale(LC_ALL, "");
	srand(time(NULL));
	//String a;
	//a.Print();

	//String b = "Alex"; // String b("Alex");
	//b.Print();

	//String c(120); // TO DO: учесть пожелание клиента, если он хочет сделать резерв больше чем 80.
	//c.Print();// как вариант, при передаче числа можно его перевести в строку, "120"
	//
	String d("Sashaa", 30);
	String b("Sasha", 20);
	d == b;
	d != b;
	d < b;
	d <= b;
	d > b;
	d >= b;
	//char character = 'A';
	//char oldChar = 'a';
	//char newChar = 'b';
	//int index = 3;
	//int start = 3;
	//int count = 2;
	//cout << (int)character;
	//String ab = d.ToUpper();
	//ab.Print();
	//d.RandomFill();
	//d.Print();
	//cin >> d;
	//cout << d << "\n";

	//string str = d;
	//cout << str;
	/*String e = d;
	e.Print();*/
}
