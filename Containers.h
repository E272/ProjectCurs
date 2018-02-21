#ifndef _CONTAINERS_H
#define _CONTAINERS_H

#include <fstream>
#include <iostream>
#include "String.h"
#define size_block 12
const int size_int = 4;

using namespace std;

namespace Containers {
	template<class T = String>
	class CycleList : public fstream{
	private:
		bool is_opened = false;
		bool wrote = false;
		char* path;
	private://just for func
		int FindByIndex(int index) {
			if (!is_opened)
				open(path, ios::binary | ios::in);
			seekg(0, ios::beg);
			unsigned int p = 0;
			read((char*)&p, size_int);
			seekg(p + size_int, ios::beg);//elem + prev, will read length of string
			int len = 0;
			for (int i = 0; i < index; i++) {
				read((char*)&len, size_int);
				seekg(len, ios::cur);//string, will read pointer of next element
				read((char*)&p, size_int);
				seekg(p + size_int, ios::beg);
			}
			p = tellg();
			p -= size_int;
			close();
			is_opened = false;
			return p;
		}
	public:
		CycleList(const char _path[]) {
			path = new char[strlen(_path) + 1];
			memcpy((void*)path, (void*)_path, strlen(_path) + 1);
			open(path, ios::binary | ios::out);
			if (!is_open())
				throw "Can't open file!\n";
			write((char*)&size_int, size_int);//pointer to first inf-block
			close();
		}
		CycleList() {
			if (is_opened) {
				close();
				is_opened = false;
			}
		}
		String FindElement(int index) {
			int p = FindByIndex(index);
			open(path, ios::binary | ios::in);
			seekg(p + size_int, ios::beg);
			int len = 0;
			read((char*)&len, size_int);
			char* str = new char[len];
			read(str, len);
			String res(str);
			delete[]str;
			return res;
		}
		void Add(int index, String str) {
			int pointer = FindByIndex(index);
			if (!is_opened)
				open(path, ios::binary | ios::in | ios::out);
			seekg(pointer, ios::beg);
			int prev = 0;
			read((char*)&prev, size_int);
			seekg(0, ios::end);
			int end = tellg();
			seekp(pointer, ios::beg);
			write((char*)&end, size_int);
			seekg(prev + size_int, ios::beg);
			int len = 0;
			read((char*)&len, size_int);
			seekg(len, ios::cur);
			len = tellg();
			seekp(len, ios::beg);
			write((char*)&end, size_int);
			seekp(end, ios::beg);
			write((char*)&prev, size_int);
			len = str.GetCount();
			write((char*)&len, size_int);
			write((char*)str, len);
			write((char*)&pointer, size_int);
			close();
			is_opened = false;
		}
		void Push(String str) {
			if (!is_opened)
				open(path, ios::in | ios::out | ios::binary);
			if (!wrote) {
				int prev = 4;
				int next = 4;
				seekp(0, ios::end);
				int len = str.GetCount();
				write((char*)&prev, size_int);
				write((char*)&len, size_int);
				write((char*)str, len);
				write((char*)&next, size_int);
				wrote = true;
			}
			else {
				/*close();
				open(path, ios::binary | ios::in);*/
				//seekp(0, ios::end);
				seekg(0, ios::end);
				unsigned int end_p = tellg();
				seekg(0, ios::beg);
				unsigned int last_p = 0;
				unsigned int first_p = 0;
				read((char*)&first_p, size_int);
				seekg(first_p, ios::beg);
				read((char*)&last_p, size_int);
				/*close();
				open(path, ios::binary | ios::out);*/
				int len = 0;
				seekg(last_p + size_int, ios::beg);
				read((char*)&len, size_int);
				seekp(last_p + 2 * size_int + len, ios::beg);//last_p + prev + len + string
				write((char*)&end_p, size_int);
				seekp(end_p, ios::beg);
				int test = tellp();
				write((char*)&last_p, size_int);
				len = str.GetCount();
				write((char*)&len, size_int);
				write((char*)str, len);
				write((char*)&first_p, size_int);
				seekp(first_p, ios::beg);
				write((char*)&end_p, size_int);
			}
			close();
			is_opened = false;
		}
		void Show(ostream& os) {
			if (!is_opened) {
				open(path, ios::binary | ios::in);
				is_opened = true;
			}
			int first_p = 0;
			read((char*)&first_p, size_int);
			int p = 0;//now
			seekg(first_p + size_int, ios::beg);
			int len = 0;
			char* str = nullptr;
			while (first_p != p) {//while didn't a cycle
				read((char*)&len, size_int);
				str = new char[len];
				read(str, len);
				os << str << endl;
				delete[]str;
				read((char*)&p, size_int);
				seekg(p + size_int, ios::beg);
			}
			close();
			is_opened = false;
		}
	};
}


#endif