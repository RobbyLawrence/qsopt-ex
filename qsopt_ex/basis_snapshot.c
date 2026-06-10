#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <stdlib.h>
#include <string.h>

#include "basis_snapshot.h"

// the snapshots exist in a block of memory of size BASIS_SNAPSHOT_CAPACITY * nrows * sizeof(int)
// since each snapshot will be m integers that we'll use to build the actual B's.
// slot k starts at storage + k * nrows
static int *storage = NULL;
static int  nrows_cur = 0;
// this is where the next write will gos
static int  ring_head = 0;
static int  ring_count = 0;

void basis_snapshot_init(int nrows)
{
	if (nrows <= 0) return;
	// we expect this to run, as nrows_cur == 0
	if (nrows != nrows_cur) {
		int *nb = (int *) realloc(storage, sizeof(int) * BASIS_SNAPSHOT_CAPACITY * nrows);
		// check allocation
		if (nb == NULL) return;

		storage = nb;
		nrows_cur = nrows;
	}
	ring_head = 0;
	ring_count = 0;
}

// might not need, resets ring buffer
void basis_snapshot_reset(void)
{
	ring_head = 0;
	ring_count = 0;
}

// push a new basis onto the ring buffer
void basis_snapshot_push(const int *baz)
{
	// error check
	if (storage == NULL || nrows_cur <= 0 || baz == NULL) return;


	int *slot = storage + ring_head * nrows_cur;
	memcpy(slot, baz, sizeof(int) * nrows_cur);

	ring_head = (ring_head + 1) % BASIS_SNAPSHOT_CAPACITY;
	if (ring_count < BASIS_SNAPSHOT_CAPACITY) ring_count++;
}

int basis_snapshot_count(void)
{
	return ring_count;
}

// we're outputting a double pointer with baz_out since we don't want to copy it all
// just want to give the caller a read-only peek into the slot that they ask for
int basis_snapshot_get(int k, int *nrows_out, const int **baz_out)
{
	if (k < 0 || k >= ring_count) return 1;
	// get the index of the kth most recent snapshot, we add BASIS_SNAPSHOT_CAPACITY * 2 to make sure it's pos
	int idx = (ring_head - 1 - k + BASIS_SNAPSHOT_CAPACITY * 2)
	          % BASIS_SNAPSHOT_CAPACITY;
	if (nrows_out) *nrows_out = nrows_cur;
	if (baz_out)   *baz_out   = storage + idx * nrows_cur;
	return 0;
}
