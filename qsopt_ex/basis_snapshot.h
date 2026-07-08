/* Rolling buffer of the last K simplex bases (lists of basic column
 * indices into the LP's internal column-major matrix).  Used by esolver
 * to dump the last few basis matrices to disk for offline analysis. */
#ifndef QS_BASIS_SNAPSHOT_H
#define QS_BASIS_SNAPSHOT_H

/* Ring capacity.  Deliberately larger than the number of bases dumped (5):
 * the dump step validates each candidate in EXACT arithmetic and skips
 * bases that are exactly singular -- a float simplex run can pivot through
 * exactly-singular bases without noticing (its tolerances hide the
 * dependency), so the tail of the pivot trail is not automatically usable
 * -- and the extra slots give the dump more candidates to fall back on. */
#define BASIS_SNAPSHOT_CAPACITY 64

// allocate the ring's storage for an LP with m rows; safe to call again
// for a new problem (resizes if m changed)
void basis_snapshot_init(int nrows);
// clear the ring buffer (does not free the underlying storage)
void basis_snapshot_reset(void);
// copy m ints from baz into the next ring slot
void basis_snapshot_push(const int *baz);
// number of basis matrices currently held (0..BASIS_SNAPSHOT_CAPACITY)
int  basis_snapshot_count(void);
// k = 0 is the most recently retained snapshot
int  basis_snapshot_get(int k, int *nrows_out, const int **baz_out);

// return the total number of snapshots taken
int get_num_snapshots(void);

#endif
