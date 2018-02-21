#include "iostream"
#include "fstream"

using namespace std;

class String {
	friend ofstream& operator<<(ofstream& of, String& x);
	friend ifstream& operator>>(ifstream& is, String& x);
	friend ostream& operator<<(ostream& of, String& x);
	friend istream& operator>>(istream& is, String& x);
private:
	char* str;
	int count;
public:
	String()
	{
		str = nullptr;
		count = 0;
	}

	String(const char x[])
	{
		count = strlen(x) + 1;
		str = new char[count];
		for (int i = 0; i < count; i++)
			str[i] = x[i];
	}

	String(int len) :count((len > 0) ? len : 0)
	{
		str = nullptr;
	}

	String(String& x)
	{
		count = x.count;
		str = new char[count];
		for (int i = 0; i < count; i++)
			str[i] = x.str[i];
	}

	String(char* _str)
	{
		count = strlen(_str) + 1;
		str = new char[count];
		for (int i = 0; i < count; i++)
			str[i] = _str[i];
	}

	char* GetStr()
	{
		char* s = new char[count];
		for (int i = 0; i < count + 1; i++)
			s[i] = str[i];
		return s;
	}

	char& operator[](int i)
	{
		if (i < 0 || i >= count)
			throw "Invalid position";
		return str[i];
	}

	operator char*()
	{
		return GetStr();
	}

	String& operator+(char *s)
	{
		int slen = strlen(s);
		char* buf = new char[count + slen + 1];
		for (int i = 0; i < count; i++)
			buf[i] = str[i];
		delete[]str;
		for (int i = count, j = 0; i < count + slen; i++, j++)
			buf[i] = s[j];
		count += slen;
		buf[count] = '\0';
		str = buf;
		return *this;
	}

	void Write(ofstream& w)
	{
		if (!w.is_open())
			throw "Error in open file";
		w.write((char*)&count, sizeof(int));
		w.write(str, count * sizeof(char));
	}

	void Read(ifstream& r)
	{
		if (!r.is_open())
			throw "Error in open file";
		r.read((char*)&count, sizeof(int));
		if (count < 0)
			throw "Invalid string";
		if (str)
			delete[]str;
		str = new char[count];
		r.read(str, count * sizeof(char));
	}

	int GetLen()
	{
		return count;
	}

	~String()
	{
		if (count)
		{
			delete[]str;
		}
	}
};

ofstream& operator<<(ofstream& of, String& x)
{
	if (of.is_open()) {
		of << x.count;
		of << x.str;
	}
	else
		throw "File wasn't availablya";
	return of;
}
ifstream& operator>>(ifstream& of, String& x)
{
	if (of.is_open()) {
		of >> x.count;
		if (x.count < 0)
			throw "Invalid size";
		if (x.str)
			delete[]x.str;
		x.str = new char[x.count];
		of >> *x.GetStr();
	}
	else
		throw "File wasn't availablya";
	return of;
}
ostream& operator<<(ostream& of, String& x)
{
	if (!of.fail()) {
		of << x.count;
		of << x.str;
	}
	else
		throw "Str wasn't availablya";
	return of;
}
istream& operator>>(istream& of, String& x)
{
	if (!of.fail()) {
		of >> x.count;
		if (x.count < 0)
			throw "Invalid size";
		if (x.str)
			delete[]x.str;
		x.str = new char[x.count];
		of >> *x.GetStr();
	}
	else
		throw "Str wasn't availablya";
	return of;
}
