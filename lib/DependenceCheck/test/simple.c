int main(int argc, char *argv[]) {
  int n;
  n = 3;

  int A[n];

  for (int i = 0; i < n; i++) {
    if (argc % 2)
      A[i] = 3;
    else
      A[i] = 4;
  }

  return 0;
}
