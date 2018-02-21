#include "Containers.h"

using namespace Containers;

void main() {
	const char x[] = "D:\\sandcube\\file.bin";
	CycleList<> list(x);
	list.Push("Egor pidor");
	list.Push("Ohuevshiy");
	list.Add(1, "Sobaka");
	list.Show(cout);
	getchar();
}