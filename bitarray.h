#ifndef BITARRAY_H
#define BITARRAY_H

//#define DEBUG

enum {
	BA_ERR_NONE,
	BA_ERR_MIN,
	BA_ERR_MAX,
	BA_ERR_MEM
};

typedef struct ba_t ba_t;

ba_t *ba_new (long bits);
void ba_free (ba_t *obj);
long ba_get_nbits (ba_t *obj);
unsigned int ba_get_bytes (ba_t *obj);
int ba_get (ba_t *obj, long bit);
int ba_set (ba_t *obj, long bit);
int ba_unset (ba_t *obj, long bit);
long ba_get_limit (void);
int ba_set_limit (long limit);
int ba_get_error (void);
long ba_get_min (ba_t *obj);
long ba_get_max (ba_t *obj);
long ba_get_next (ba_t *obj, long from);
#ifdef DEBUG
void ba_dump (ba_t *obj);
#endif

#endif /* End of BITARRAY_H */
