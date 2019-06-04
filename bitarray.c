#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "bitarray.h"

#define DEBUG

typedef struct ba_t {
	long nbits;
	unsigned int bytes;
	unsigned char *b;
} ba_t;

/* Default maximum number of bytes to allocate: 10 MB */
static long ba_alloc_max = 10485760;

/* Last error produced */
static int ba_error = BA_ERR_NONE;

ba_t *ba_new (long bits)
{
	ba_t *new;
	unsigned int bytes;

	if (bits <= 0) {
		ba_error = BA_ERR_MIN;
		return NULL;
	}
	if (bits > ba_alloc_max) {
		ba_error = BA_ERR_MAX;
		return NULL;
	}

	bytes = bits / CHAR_BIT;
	if (bits % CHAR_BIT) bytes++;

	new = calloc(1, sizeof(ba_t));
	if (new == NULL) {
		ba_error = BA_ERR_MEM;
		return NULL;
	}

	new->b = calloc(bytes, sizeof(unsigned char));
	if (new->b == NULL) {
		free(new);
		ba_error = BA_ERR_MEM;
		return NULL;
	}

	new->nbits = bits;
	new->bytes = bytes;
	return new;
}

void ba_free (ba_t *obj)
{
	free(obj->b);
	free(obj);
}

long ba_get_nbits (ba_t *obj)
{
	return obj->nbits;
}

unsigned int ba_get_bytes (ba_t *obj)
{
	return obj->bytes;
}

int ba_get (ba_t *obj, long bit)
{
	unsigned char r, mask=1;
	unsigned int byte;

	if (bit < 0) {
		ba_error = BA_ERR_MIN;
		return -1;
	}

	if (bit >= obj->nbits) {
		ba_error = BA_ERR_MAX;
		return -1;
	}

	byte = bit / CHAR_BIT;
	if (r = bit % CHAR_BIT) byte++;

	mask <<= r;
	return (obj->b[byte] & mask) ? 1 : 0;
}

int ba_set (ba_t *obj, long bit)
{
	unsigned char r, mask=1;
	unsigned int byte;

	if (bit < 0) {
		ba_error = BA_ERR_MIN;
		return -1;
	}

	if (bit >= obj->nbits) {
		ba_error = BA_ERR_MAX;
		return -1;
	}

	byte = bit / CHAR_BIT;
	if (r = bit % CHAR_BIT) byte++;

	mask <<= r;
	obj->b[byte] |= mask;

	return bit;
}

int ba_unset (ba_t *obj, long bit)
{
	unsigned char r, mask=1;
	unsigned int byte;

	if (bit < 0) {
		ba_error = BA_ERR_MIN;
		return -1;
	}

	if (bit >= obj->nbits) {
		ba_error = BA_ERR_MAX;
		return -1;
	}

	byte = bit / CHAR_BIT;
	if (r = bit % CHAR_BIT) byte++;

	mask <<= r;
	obj->b[byte] ^= mask;

	return bit;
}

long ba_get_max (void)
{
	return ba_alloc_max;
}

int ba_set_max (long max)
{
	if (max <= 0) return 0;
	ba_alloc_max = max;
}

int ba_get_error (void)
{
	return ba_error;
}

/* ========================================================================== *
 * DEBUG
 * ========================================================================== */
#ifdef DEBUG
static bin (unsigned char b)
{
	int i;
	unsigned char mask = 1 << (CHAR_BIT - 1);

	for (i=0; i < CHAR_BIT; i++) {
		printf("%i", (b & mask) ? 1 : 0);
		mask >>= 1;
	}
}
int ba_dump (ba_t *obj)
{
	long i;

	printf("\n");
	for (i=0; i < obj->bytes; i++) {
		printf("  ");
		bin(obj->b[i]);
		putchar('\n');
	}
}
#endif	//DEBUG
