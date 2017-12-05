#include <iostream>

struct alignas(64) huge {
  char x[64];
};

int main() {
  printf("C printf worked!\n");
  huge* x = new huge;
  delete x;
  printf("new delete worked!\n");
  printf("cout=%p, cerr=%p\n", *(void**)&std::cout, *(void**)&std::cerr);
  std::cout << "Hello C++ World!" << std::endl;
  printf("iostreams worked!\n");
  return 1;
}
