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
  void mtrx_##name##_init(Matrix_##name##_t *m, size_t rows, size_t cols) ; \
  void mtrx_##name##_del(Matrix_##name##_t *m) ; \
  void mtrx_##name##_resize(Matrix_##name##_t *m, size_t rows, size_t cols) ; \
  inline size_t mtrx_##name##_rows(Matrix_##name##_t *m) ; \
  inline size_t mtrx_##name##_cols(Matrix_##name##_t *m) ; \
  inline size_t mtrx_##name##_index(Matrix_##name##_t *m, size_t r, size_t c) ; \
  inline void mtrx_##name##_get(Matrix_##name##_t *m, size_t r, size_t c, elem_t *x) ; \
  inline void mtrx_##name##_set(Matrix_##name##_t *m, size_t r, size_t c, elem_t x) ; \

#define MATRIX_IMPL(name, elem_t) \
  \
  void mtrx_##name##_init(Matrix_##name##_t *m, size_t rows, size_t cols) { \
    m->rows = rows; \
    m->cols = cols; \
    m->elems = malloc(sizeof(elem_t) * rows * cols); \
  } \
  \
  void mtrx_##name##_del(Matrix_##name##_t *m) { \
    free(m->elems); \
  } \
  \
  void mtrx_##name##_resize(Matrix_##name##_t *m, size_t rows, size_t cols) { \
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
