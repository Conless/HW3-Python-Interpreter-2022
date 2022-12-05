#include <bits/stdc++.h>

int cnt = 0;

int f() { return ++cnt;  }
int g(int a = f()) { return a; }

int main() { std::cout << g() << " " << g() << std::endl; }