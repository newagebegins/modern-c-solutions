#ifndef CIRCULAR_H
#define CIRCULAR_H

typedef struct circular circular;

circular* circular_append(circular* c, double value);
double circular_pop(circular* c);
double* circular_element(circular* c, size_t pos);
circular* circular_init(circular* c, size_t max_len);
void circular_destroy(circular* c);
circular* circular_new(size_t len);
void circular_delete(circular* c);
circular* circular_resize(circular* c, size_t max_len);
size_t circular_getlength(circular* c);

#endif
