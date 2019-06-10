#include <stdlib.h>
#include <limits.h>
#include "bitarray.h"

//#define DEBUG

#ifdef DEBUG
#include <stdio.h>
#endif

typedef struct ba_t {
	long nbits;
	unsigned int bytes;
	unsigned char *b;
} ba_t;

/* Default limit number of bytes to allocate: 10 MB */
static long ba_alloc_limit = 10485760;

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
	if (bits > ba_alloc_limit) {
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

	byte = (bit < (CHAR_BIT - 1)) ? 0 : bit / CHAR_BIT;
	r = (bit < (CHAR_BIT - 1)) ? bit : bit % CHAR_BIT;
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

	byte = (bit < (CHAR_BIT - 1)) ? 0 : bit / CHAR_BIT;
	r = (bit < (CHAR_BIT - 1)) ? bit : bit % CHAR_BIT;
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

	byte = (bit < (CHAR_BIT - 1)) ? 0 : bit / CHAR_BIT;
	r = (bit < (CHAR_BIT - 1)) ? bit : bit % CHAR_BIT;
	mask <<= r;
	obj->b[byte] ^= mask;

	return bit;
}

long ba_get_limit (void)
{
	return ba_alloc_limit;
}

int ba_set_limit (long limit)
{
	if (limit <= 0) return 0;
	ba_alloc_limit = limit;
	return limit;
}

int ba_get_error (void)
{
	return ba_error;
}

long ba_get_min (ba_t *obj)
{
	long min = 0;
	unsigned int byte, bit;
	unsigned char mask;

	for (byte=0; byte < obj->bytes; byte++) {
		mask=1;
		for (bit=0; bit < CHAR_BIT; bit++) {
			if (obj->b[byte] & mask) return min;
			mask <<= 1;
			min++;
		}
	}

	return 0;
}

long ba_get_max (ba_t *obj)
{
	long max = (obj->bytes * CHAR_BIT) - 1;
	unsigned int byte, bit;
	unsigned char mask;

	for (byte=obj->bytes - 1; byte >= 0; byte--) {
		mask = 1 << (CHAR_BIT - 1);
		for (bit=0; bit < CHAR_BIT; bit++) {
			if (obj->b[byte] & mask) return max;
			mask >>= 1;
			max--;
		}
	}

	return 0;
}

long ba_get_next (ba_t *obj, long from)
{
	unsigned char pos, mask;
	unsigned int cur, byte;
	long bit=from + 1;

	if (from < 0) {
		ba_error = BA_ERR_MIN;
		return -1;
	}

	if (from >= obj->nbits) {
		ba_error = BA_ERR_MAX;
		return -1;
	}

	cur = from / CHAR_BIT;
	pos = (from < CHAR_BIT) ? from : from % CHAR_BIT;
	mask = 1 << ++pos;

	for (byte=cur; byte < obj->bytes; byte++) {
		if (obj->b[byte] == 0) {
			bit += CHAR_BIT;
			continue;
		}

		while (mask) {
			if (bit >= obj->nbits) break;
			if (obj->b[byte] & mask) return bit;
			mask <<= 1;
			bit++;
		}

		mask = 1;
	}

	return from;
}

/* ========================================================================== *
 * DEBUG
 * ========================================================================== */
#ifdef DEBUG
static void bin (unsigned char b)
{
	int i;
	unsigned char mask = 1 << (CHAR_BIT - 1);

	for (i=0; i < CHAR_BIT; i++) {
		printf("%i", (b & mask) ? 1 : 0);
		mask >>= 1;
	}
}

void ba_dump (ba_t *obj)
{
	long i;

	for (i=0; i < obj->bytes; i++) {
		printf("  %li: ", i);
		bin(obj->b[i]);
		putchar('\n');
	}
	putchar('\n');
}
#endif	//DEBUG
