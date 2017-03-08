#ifndef __CUTILS_MATRIX_H
#define __CUTILS_MATRIX_H

// A two dimensional matrix parametrized over its name and element type.
//
// The macro MATRIX_TYPE(name, elem_t) generates the struct definition and
// function declarations. name determines the name of the structure and
// function names. For example if name is "u8" then the struct will be named
// struct Matrix_u8 and all functions will be prefixed with matrix_u8_.
// elem_t is the type of elements of the matrix.
//
// The macro MATRIX_IMPL(name, elem_t) generates the function implementations.
// It must be called with the same name and elem_t as MATRIX_TYPE.
//
// The macro MATRIX_INIT(name, elem_t) calls the macros MATRIX_TYPE and
// MATRIX_IMPL together.
//

#define MATRIX_TYPE(name, elem_t) \
  \
  struct Matrix_##name { \
    size_t rows; \
    size_t cols; \
    elem_t *elems; \
  }; \
  void matrix_##name##_init(struct Matrix_##name *m, size_t rows, size_t cols) ; \
  void matrix_##name##_del(struct Matrix_##name *m) ; \
  void matrix_##name##_resize(struct Matrix_##name *m, size_t rows, size_t cols) ; \
  size_t matrix_##name##_rows(struct Matrix_##name *m) ; \
  size_t matrix_##name##_cols(struct Matrix_##name *m) ; \
  size_t matrix_##name##_index(struct Matrix_##name *m, size_t r, size_t c) ; \
  void matrix_##name##_get(struct Matrix_##name *m, size_t r, size_t c, elem_t *x) ; \
  void matrix_##name##_set(struct Matrix_##name *m, size_t r, size_t c, elem_t x) ; \

#define MATRIX_IMPL(name, elem_t) \
  \
  void matrix_##name##_init(struct Matrix_##name *m, size_t rows, size_t cols) { \
    m->rows = rows; \
    m->cols = cols; \
    m->elems = malloc(sizeof(elem_t) * rows * cols); \
  } \
  \
  void matrix_##name##_del(struct Matrix_##name *m) { \
    free(m->elems); \
  } \
  \
  void matrix_##name##_resize(struct Matrix_##name *m, size_t rows, size_t cols) { \
    elem_t *old = m->elems; \
    m->elems = malloc(sizeof(elem_t) * rows * cols); \
    for (size_t r=0; r<m->rows; ++r) { \
      for (size_t c=0; c<m->cols; ++c) { \
        size_t old_idx = r*m->rows + c; \
        size_t new_idx = r*rows + c; \
        m->elems[new_idx] = old[old_idx]; \
      } \
    } \
    m->rows = rows; \
    m->cols = cols; \
    free(old); \
  } \
  \
  size_t matrix_##name##_rows(struct Matrix_##name *m) { \
    return m->rows; \
  } \
  \
  size_t matrix_##name##_cols(struct Matrix_##name *m) { \
    return m->cols; \
  } \
  \
  size_t matrix_##name##_index(struct Matrix_##name *m, size_t r, size_t c) { \
    return r*m->cols + c; \
  } \
  \
  void matrix_##name##_get(struct Matrix_##name *m, size_t r, size_t c, elem_t *x) { \
    size_t i = matrix_##name##_index(m, r, c); \
    *x = m->elems[i]; \
  } \
  void matrix_##name##_set(struct Matrix_##name *m, size_t r, size_t c, elem_t x) { \
    size_t i = matrix_##name##_index(m, r, c); \
    m->elems[i] = x; \
  } \


#define MATRIX_INIT(name, elem_t) \
  MATRIX_TYPE(name, elem_t) \
  MATRIX_IMPL(name, elem_t)


#endif
