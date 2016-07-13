#ifndef __MATRIX_H
#define __MATRIX_H

/* A two dimensional matrix parametrized over its name and element type */

#define MATRIX_TYPE(name, elem_t) \
  \
  struct Matrix_##name##_t { \
    size_t rows; \
    size_t cols; \
    elem_t *elems; \
  }; \
  inline size_t mtrx_##name##_rows(Matrix_##name##_t *m) ; \
  inline size_t mtrx_##name##_cols(Matrix_##name##_t *m) ; \
  inline size_t mtrx_##name##_index(Matrix_##name##_t *m, size_t r, size_t c) ; \
  inline void mtrx_##name##_get(Matrix_##name##_t *m, size_t r, size_t c, elem_t *x) ; \
  inline void mtrx_##name##_set(Matrix_##name##_t *m, size_t r, size_t c, elem_t x) ; \

#define MATRIX_IMPL(name, elem_t) \
  \
  inline size_t mtrx_##name##_rows(Matrix_##name##_t *m) { \
    return m->rows; \
  } \
  \
  inline size_t mtrx_##name##_cols(Matrix_##name##_t *m) { \
    return m->cols; \
  } \
  \
  inline size_t mtrx_##name##_index(Matrix_##name##_t *m, size_t r, size_t c) { \
    return r*m->cols + c; \
  } \
  \
  inline void mtrx_##name##_get(Matrix_##name##_t *m, size_t r, size_t c, elem_t *x) { \
    size_t i = mtrx_##name##_index(m, r, c); \
    *x = m.elems[i]; \
  } \
  inline void mtrx_##name##_set(Matrix_##name##_t *m, size_t r, size_t c, elem_t x) { \
    size_t i = mtrx_##name##index(m, r, c); \
    m->elems[i] = x; \
  } \


#endif
