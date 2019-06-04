#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef struct ba_t {
	unsigned long nbits;
	unsigned int bytes;
	unsigned char *b;
} ba_t;

ba_t *ba_new (unsigned long bits)
{
	ba_t *new;
	unsigned int bytes;

	bytes = bits / CHAR_BIT;
	if (bits % CHAR_BIT) bytes++;

	new = calloc(1, sizeof(ba_t));
	if (new == NULL) return NULL;

	new->b = calloc(bytes, sizeof(unsigned char));
	if (new->b == NULL) {
		free(new);
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

unsigned long ba_get_nbits (ba_t *obj)
{
	return obj->nbits;
}

unsigned int ba_get_bytes (ba_t *obj)
{
	return obj->bytes;
}

int ba_get (ba_t *obj, unsigned long bit)
{
	unsigned char r, mask=1;
	unsigned int byte;

	if (bit >= obj->nbits) return -1;

	byte = bit / CHAR_BIT;
	if (r = bit % CHAR_BIT) byte++;

	mask <<= r;
	return (obj->b[byte] & mask) ? 1 : 0;
}

int ba_set (ba_t *obj, unsigned long bit)
{
	unsigned char r, mask=1;
	unsigned int byte;

	if (bit >= obj->nbits) return -1;

	byte = bit / CHAR_BIT;
	if (r = bit % CHAR_BIT) byte++;

	mask <<= r;
	obj->b[byte] |= mask;

	return bit;
}

int ba_unset (ba_t *obj, unsigned long bit)
{
	unsigned char r, mask=1;
	unsigned int byte;

	if (bit >= obj->nbits) return -1;

	byte = bit / CHAR_BIT;
	if (r = bit % CHAR_BIT) byte++;

	mask <<= r;
	obj->b[byte] ^= mask;

	return bit;
}
