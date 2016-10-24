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
	char sval[32];

	/* Note: Most error checking omitted for simplicity */

	rc = mdb_env_create(&env);
	rc = mdb_env_open(env, "./testdb", 0, 0664);

	key.mv_size = sizeof(int);
	key.mv_data = sval;
	data.mv_size = sizeof(sval);
	data.mv_data = sval;

	sprintf(sval, "%03x %d foo bar", 32, 0);
	rc = mdb_txn_begin(env, NULL, MDB_RDONLY, &txn);
	if (rc) {
		printf("mdb_tx_begin error\n");
		goto leave;
	}
	rc = mdb_dbi_open(txn, NULL, 0, &dbi);
	if (rc) {
		printf("mdb_dbi_open error\n");
		goto leave;
	}
	rc = mdb_get(txn, dbi, &key, &data);
	if (rc) {
		printf("mdb_get error\n");
		goto leave;
	}
	printf("value: %s\n", (char *)data.mv_data);
leave:
	mdb_dbi_close(env, dbi);
	mdb_env_close(env);
	mdb_txn_abort(txn);
	return 0;
}
