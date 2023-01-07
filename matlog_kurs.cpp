#include <iostream>
#include <cmath>
#include <vector>
#include "properties.h"
#include "limits.h"

#include "bdd.h"

void init(bdd p[M][N][N]);


void fun(char* varset, int size);

int main()
{
  bdd_init(50000000, 1000000);
  bdd_setcacheratio(100);
  bdd_setvarnum(N_VAR);
  bdd p[M][N][N];

  init(p);

  bdd task = bddtrue;

  //variant 1: n1 = 6, n2 = 8, n3 = 5, n4 = 3

  //6 type-1 limitations
  task &= constraint1(p, 0, 0, 0);
  task &= constraint1(p, 1, 2, 3);
  task &= constraint1(p, 3, 3, 5);
  task &= constraint1(p, 2, 5, 2);
  task &= constraint1(p, 2, 1, 1);
  task &= constraint1(p, 0, 7, 8);

  //additional type-1 limitations
  task &= constraint1(p, 0, 6, 3);

  //8 type-2 limitations
  task &= constraint2(p, 2, 1, 0, 4);
  task &= constraint2(p, 3, 3, 1, 0);
  task &= constraint2(p, 2, 5, 3, 0);
  task &= constraint2(p, 0, 7, 1, 5);
  task &= constraint2(p, 2, 4, 1, 7);
  task &= constraint2(p, 3, 1, 1, 4);
  task &= constraint2(p, 0, 8, 3, 6);
  task &= constraint2(p, 0, 5, 2, 6);

  //5 type-3 limitations
  task &= constraint3(p, NeighbourRelations::UP, 2, 6, 1, 2);
  task &= constraint3(p, NeighbourRelations::DOWN, 1, 8, 3, 5);
  task &= constraint3(p, NeighbourRelations::DOWN, 3, 7, 0, 8);
  task &= constraint3(p, NeighbourRelations::DOWN, 2, 4, 0, 1);
  task &= constraint3(p, NeighbourRelations::UP, 2, 7, 1, 6);

  //additional type-3 limitations 
  task &= constraint3(p, NeighbourRelations::UP, 0, 2, 3, 4);
  task &= constraint3(p, NeighbourRelations::UP, 1, 3, 3, 2);

//3 type-4 limitations 
  task &= constraint4(p, 2, 6, 1, 6);
  task &= constraint4(p, 0, 8, 1, 2);
  task &= constraint4(p, 2, 4, 3, 5);
  
  constraint6(task, p);
  constraint5(task, p);
  

  size_t satcount = static_cast<size_t>(bdd_satcount(task));
  std::cout << satcount << " solutions:\n" << std::endl;
  if (satcount) bdd_allsat(task, fun);

  bdd_done();

}


void init(bdd p[M][N][N])
{
  size_t I = 0;
  for (size_t i = 0; i < N; ++i) {
    for (size_t j = 0; j < N; ++j) {
      for (size_t z = 0; z < M; ++z) {
        p[z][i][j] = bddtrue;
        for (size_t k = 0; k < LOG_N; ++k) {
          p[z][i][j] &= ((j >> k) & 1) ? bdd_ithvar(I + LOG_N * z + k) : bdd_nithvar(I + LOG_N * z + k);
        }
      }
    }
    I += LOG_N * M;
  }
}



void print()
{
  for (size_t i = 0; i < N; i++) {
    std::cout << i << ": ";
    for (size_t j = 0; j < M; j++) {
      int J = i * M * LOG_N + j * LOG_N;
      int num = 0;
      for (size_t k = 0; k < LOG_N; k++) num += static_cast<size_t>(var[J + k] << k);
      std::cout << num << ' ';
    }
    std::cout << "\n";
  }
  std::cout << "\n";
}

void build(char* varset, unsigned n, unsigned I)
{
  if (I == n - 1) {
    if (varset[I] >= 0) {
      var[I] = varset[I];
      print();
      return;
    }

    var[I] = 0;
    print();
    var[I] = 1;
    print();
    return;
  }

  if (varset[I] >= 0) {
    var[I] = varset[I];
    build(varset, n, I + 1);
    return;
  }

  var[I] = 0;
  build(varset, n, I + 1);
  var[I] = 1;
  build(varset, n, I + 1);
}

void fun(char* varset, int size)
{
  build(varset, size, 0);
}
