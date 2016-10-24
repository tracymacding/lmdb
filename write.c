/* sample-mdb.txt - MDB toy/sample
 *
 * Do a line-by-line comparison of this and sample-bdb.txt
 */
/*
 * Copyright 2012-2016 Howard Chu, Symas Corp.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted only as authorized by the OpenLDAP
 * Public License.
 *
 * A copy of this license is available in the file LICENSE in the
 * top-level directory of the distribution or, alternatively, at
 * <http://www.OpenLDAP.org/license.html>.
 */
#include <stdio.h>
#include "lmdb.h"

int main(int argc,char * argv[])
{
	int rc;
	MDB_env *env;
	MDB_dbi dbi;
	MDB_val key, data;
	MDB_txn *txn;
	char kval[32];
	char vval[32];
	int i;

	/* Note: Most error checking omitted for simplicity */

	rc = mdb_env_create(&env);
	rc = mdb_env_open(env, "./testdb", 0, 0664);
	rc = mdb_txn_begin(env, NULL, 0, &txn);
	rc = mdb_dbi_open(txn, NULL, 0, &dbi);
	key.mv_size = sizeof(int);
	key.mv_data = kval;

	data.mv_size = sizeof(vval);
	data.mv_data = vval;

	sprintf(kval, "%03x %d foo bar", 32, 0);
	sprintf(vval, "%03x %d foo bar", 32, 0);

	rc = mdb_put(txn, dbi, &key, &data, 0);
	rc = mdb_txn_commit(txn);
	if (rc) {
		fprintf(stderr, "mdb_txn_commit: (%d) %s\n", rc, mdb_strerror(rc));
		mdb_dbi_close(env, dbi);
		goto leave;
	}

	for (i = 0; i < 10; i++) {
		sprintf(vval, "%03x %d foo bar 2", 32, 0);

		rc = mdb_txn_begin(env, NULL, 0, &txn);
		rc = mdb_put(txn, dbi, &key, &data, 0);
		rc = mdb_txn_commit(txn);
		if (rc) {
			fprintf(stderr, "mdb_txn_commit: (%d) %s\n", rc, mdb_strerror(rc));
			mdb_dbi_close(env, dbi);
			goto leave;
		}

		sprintf(vval, "%03x %d foo bar 3", 32, 0);

		rc = mdb_txn_begin(env, NULL, 0, &txn);
		rc = mdb_put(txn, dbi, &key, &data, 0);
		rc = mdb_txn_commit(txn);
		if (rc) {
			fprintf(stderr, "mdb_txn_commit: (%d) %s\n", rc, mdb_strerror(rc));
			mdb_dbi_close(env, dbi);
			goto leave;
		}
	}

	// mdb_dbi_close(env, dbi);

leave:
	mdb_dbi_close(env, dbi);
	mdb_env_close(env);
	return 0;
}
