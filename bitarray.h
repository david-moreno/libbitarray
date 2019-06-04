#ifndef BITARRAY_H
#define BITARRAY_H

typedef struct ba_t ba_t;

ba_t *ba_new (unsigned long bits);
void ba_free (ba_t *obj);
unsigned long ba_get_nbits (ba_t *obj);
unsigned int ba_get_bytes (ba_t *obj);
int ba_get (ba_t *obj, unsigned long bit);
int ba_set (ba_t *obj, unsigned long bit);
int ba_unset (ba_t *obj, unsigned long bit);

#endif /* End of BITARRAY_H */
