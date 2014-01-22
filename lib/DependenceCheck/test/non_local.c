int A;
// Both functions access the global variable A
void func1(void);
void func2(void);

void func1(void) {
  A = 2;

}

void func2(void) {
  A = 3;
}

int main(int argc, char *argv[]) {
  A = 0;

  func1();
  func2();

  return 0;
}
