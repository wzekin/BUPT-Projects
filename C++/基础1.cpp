#include <iostream>
#include <ostream>

struct Matrix {
  int** value;
};

struct Matrix* matrix_init() {
  struct Matrix* m = (struct Matrix*)malloc(sizeof(struct Matrix));
  m->value = (int**)malloc(sizeof(int*) * 4);
  for (int i = 0; i < 4; i++) {
    m->value[i] = (int*)malloc(sizeof(int) * 5);
  }
  return m;
}

void matrix_scanf(struct Matrix* self) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 5; j++) {
      std::cin >> self->value[i][j];
    }
  }
}

void matrix_printf(struct Matrix* self) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 5; j++) {
      std::cout << self->value[i][j];
      if (j != 4) {
        std::cout << " ";
      }
    }
    std::cout << std::endl;
  }
}

void matrix_add(struct Matrix* self, struct Matrix* other, struct Matrix* m) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 5; j++) {
      m->value[i][j] = self->value[i][j] + other->value[i][j];
    }
  }
}

void matrix_reduce(struct Matrix* self,
                   struct Matrix* other,
                   struct Matrix* m) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 5; j++) {
      m->value[i][j] = self->value[i][j] - other->value[i][j];
    }
  }
}

void matrix_free(struct Matrix* self) {
  for (int i = 0; i < 4; i++) {
    free(self->value[i]);
  }
  free(self->value);
  free(self);
}

int main(){
  struct Matrix* A1 = matrix_init(); 
  struct Matrix* A2 = matrix_init(); 
  struct Matrix* A3 = matrix_init(); 

  matrix_scanf(A1);
  matrix_scanf(A2);

  matrix_add(A1,A2,A3);
  matrix_printf(A3);

  matrix_reduce(A1,A2,A3);
  matrix_printf(A3);

  matrix_free(A1);
  matrix_free(A2);
  matrix_free(A3);
}
