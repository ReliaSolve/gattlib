/*
 *
 *  GattLib - GATT Library
 *
 *  Copyright (C) 2016-2017  Olivier Martin <olivier@labapart.org>
 *  Copyright (C) 2018  ReliaSolve <russ@reliasolve.com>
 *
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gattlib.h"

typedef enum { READ, WRITE} operation_t;
operation_t g_operation;

static uuid_t g_uuid;
long int value_data;

static void usage(char *name) {
	fprintf(stderr,"Usage: %s <device_address> <uuid>\n", name);
}

int main(int argc, char *argv[]) {
	uint8_t buffer[101], lastBuffer[101];
	int ret;
	size_t len;
	gatt_connection_t* connection;

	if (argc != 3) {
		usage(argv[0]);
		return 1;
	}

	if (gattlib_string_to_uuid(argv[2], strlen(argv[2]) + 1, &g_uuid) < 0) {
		usage(argv[0]);
		return 2;
	}

	connection = gattlib_connect(NULL, argv[1], BDADDR_LE_PUBLIC, BT_SEC_LOW, 0, 0);
	if (connection == NULL) {
		fprintf(stderr, "Fail to connect to the bluetooth device.\n");
		return 3;
	}

    // Print a new value whenever the string changes
    memset(buffer, 0, sizeof(buffer));
    memcpy(lastBuffer, buffer, sizeof(buffer));
    do {
		len = sizeof(buffer) - 1;
        memset(buffer, 0, sizeof(buffer));
		ret = gattlib_read_char_by_uuid(connection, &g_uuid, buffer, &len);

        if (ret == 0) if (strncmp((char*)lastBuffer, (char*)buffer, sizeof(buffer))) {
            printf("%s\n", buffer);
            fflush(stdout);
            memcpy(lastBuffer, buffer, sizeof(buffer));
        }
    } while (ret == 0);

	gattlib_disconnect(connection);
	return 0;
}
