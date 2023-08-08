/*
 * Copyright (c) 2022 rockchip
 *
 */
#include "rk_sysfs.h"
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int _write_sysfs_int(const char *filename, const char *basedir, int val, bool verify) {
	int ret = 0;
	FILE *sysfsfp;
	int test;
	char *temp = (char *)malloc(strlen(basedir) + strlen(filename) + 2);
	if (!temp)
		return -ENOMEM;

	ret = sprintf(temp, "%s/%s", basedir, filename);
	if (ret < 0)
		goto error_free;

	sysfsfp = fopen(temp, "w");
	if (!sysfsfp) {
		ret = -errno;
		fprintf(stderr, "failed to open %s\n", temp);
		goto error_free;
	}

	ret = fprintf(sysfsfp, "%d", val);
	if (ret < 0) {
		if (fclose(sysfsfp))
			perror("_write_sysfs_int(): Failed to close dir");

		goto error_free;
	}

	if (fclose(sysfsfp)) {
		ret = -errno;
		goto error_free;
	}

	if (verify) {
		sysfsfp = fopen(temp, "r");
		if (!sysfsfp) {
			ret = -errno;
			fprintf(stderr, "failed to open %s\n", temp);
			goto error_free;
		}

		if (fscanf(sysfsfp, "%d", &test) != 1) {
			ret = errno ? -errno : -ENODATA;
			if (fclose(sysfsfp))
				perror("_write_sysfs_int(): Failed to close dir");

			goto error_free;
		}

		if (fclose(sysfsfp)) {
			ret = -errno;
			goto error_free;
		}

		if (test != val) {
			fprintf(stderr, "Possible failure in int write %d to %s/%s\n", val, basedir, filename);
			ret = -1;
		}
	}

error_free:
	free(temp);
	return ret;
}

int write_sysfs_int(const char *filename, const char *basedir, int val) {
	return _write_sysfs_int(filename, basedir, val, false);
}

int write_sysfs_int_and_verify(const char *filename, const char *basedir, int val) {
	return _write_sysfs_int(filename, basedir, val, true);
}

static int _write_sysfs_string(const char *filename, const char *basedir, const char *val,
                               bool verify) {
	int ret = 0;
	FILE *sysfsfp;
	char *temp = (char *)malloc(strlen(basedir) + strlen(filename) + 2);

	if (!temp) {
		fprintf(stderr, "Memory allocation failed\n");
		return -ENOMEM;
	}

	ret = sprintf(temp, "%s/%s", basedir, filename);
	if (ret < 0)
		goto error_free;

	sysfsfp = fopen(temp, "w");
	if (!sysfsfp) {
		ret = -errno;
		fprintf(stderr, "Could not open %s\n", temp);
		goto error_free;
	}

	ret = fprintf(sysfsfp, "%s", val);
	if (ret < 0) {
		if (fclose(sysfsfp))
			perror("_write_sysfs_string(): Failed to close dir");

		goto error_free;
	}

	if (fclose(sysfsfp)) {
		ret = -errno;
		goto error_free;
	}

	if (verify) {
		sysfsfp = fopen(temp, "r");
		if (!sysfsfp) {
			ret = -errno;
			fprintf(stderr, "Could not open file to verify\n");
			goto error_free;
		}

		if (fscanf(sysfsfp, "%s", temp) != 1) {
			ret = errno ? -errno : -ENODATA;
			if (fclose(sysfsfp))
				perror("_write_sysfs_string(): Failed to close dir");

			goto error_free;
		}

		if (fclose(sysfsfp)) {
			ret = -errno;
			goto error_free;
		}

		if (strcmp(temp, val) != 0) {
			fprintf(stderr,
			        "Possible failure in string write of %s "
			        "Should be %s written to %s/%s\n",
			        temp, val, basedir, filename);
			ret = -1;
		}
	}

error_free:
	free(temp);

	return ret;
}

int write_sysfs_string_and_verify(const char *filename, const char *basedir, const char *val) {
	return _write_sysfs_string(filename, basedir, val, true);
}

int write_sysfs_string(const char *filename, const char *basedir, const char *val) {
	return _write_sysfs_string(filename, basedir, val, false);
}

int read_sysfs_posint(const char *filename, const char *basedir) {
	int ret;
	FILE *sysfsfp;
	char *temp = (char *)malloc(strlen(basedir) + strlen(filename) + 2);

	if (!temp) {
		fprintf(stderr, "Memory allocation failed");
		return -ENOMEM;
	}

	ret = sprintf(temp, "%s/%s", basedir, filename);
	if (ret < 0)
		goto error_free;

	sysfsfp = fopen(temp, "r");
	if (!sysfsfp) {
		ret = -errno;
		goto error_free;
	}

	errno = 0;
	if (fscanf(sysfsfp, "%d\n", &ret) != 1) {
		ret = errno ? -errno : -ENODATA;
		if (fclose(sysfsfp))
			perror("read_sysfs_posint(): Failed to close dir");

		goto error_free;
	}

	if (fclose(sysfsfp))
		ret = -errno;

error_free:
	free(temp);

	return ret;
}

int read_sysfs_float(const char *filename, const char *basedir, float *val) {
	int ret = 0;
	FILE *sysfsfp;
	char *temp = (char *)malloc(strlen(basedir) + strlen(filename) + 2);

	if (!temp) {
		fprintf(stderr, "Memory allocation failed");
		return -ENOMEM;
	}

	ret = sprintf(temp, "%s/%s", basedir, filename);
	if (ret < 0)
		goto error_free;

	sysfsfp = fopen(temp, "r");
	if (!sysfsfp) {
		ret = -errno;
		goto error_free;
	}

	errno = 0;
	if (fscanf(sysfsfp, "%f\n", val) != 1) {
		ret = errno ? -errno : -ENODATA;
		if (fclose(sysfsfp))
			perror("read_sysfs_float(): Failed to close dir");

		goto error_free;
	}

	if (fclose(sysfsfp))
		ret = -errno;

error_free:
	free(temp);

	return ret;
}

int read_sysfs_string(const char *filename, const char *basedir, char *str) {
	int ret = 0;
	FILE *sysfsfp;
	char *temp = (char *)malloc(strlen(basedir) + strlen(filename) + 2);

	if (!temp) {
		fprintf(stderr, "Memory allocation failed");
		return -ENOMEM;
	}

	ret = sprintf(temp, "%s/%s", basedir, filename);
	if (ret < 0)
		goto error_free;

	sysfsfp = fopen(temp, "r");
	if (!sysfsfp) {
		ret = -errno;
		goto error_free;
	}

	errno = 0;
	if (fscanf(sysfsfp, "%s\n", str) != 1) {
		ret = errno ? -errno : -ENODATA;
		if (fclose(sysfsfp))
			perror("read_sysfs_string(): Failed to close dir");

		goto error_free;
	}

	if (fclose(sysfsfp))
		ret = -errno;

error_free:
	free(temp);

	return ret;
}
