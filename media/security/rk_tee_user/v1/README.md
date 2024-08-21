# OP-TEE sanity testsuite
The optee_test git contains the source code for the TEE sanity
testsuite in Linux using the ARM(R) TrustZone(R) technology.
It is distributed under the GPLv2 and BSD 2-clause open-source
licenses.
For a general overview of OP-TEE, please see the
[Notice.md](Notice.md) file.

## License
The client applications (`optee_test/host/*`) are provided under the
[GPL-2.0](http://opensource.org/licenses/GPL-2.0) license.
The user TAs (`optee_test/ta/*`) are provided under the
[BSD 2-Clause](http://opensource.org/licenses/BSD-2-Clause) license.


## Get and build the software

### HOWTO build the testsuite
#### Standard tests
xtest test suite comes with a standard test suite,
freely available. When installing OP-TEE through the
[manifest](https://github.com/OP-TEE/optee_os/blob/master/README.md#6-repo-manifests),
the [build](https://github.com/OP-TEE/build)
component provides the `xtest` target which builds optee_test.
It makes use of the following environment variables:
* `CROSS_COMPILE_HOST`: the cross compiler used to compile the
  Non-Secure Client Application (`host/xtest`)
* `CROSS_COMPILE_TA`: the cross compiler used to compile the
  Trusted Applications (`ta`)
* `TA_DEV_KIT_DIR`: the path to the Trusted Application Dev Kit.
  It can be found in optee_os repository, once optee_os has been compiled.
* `O`: the output repository
* When the application is compiled in 32bits mode,
  `CFG_ARM32=y` must be set.


#### Extended test (Global Platform tests)
Developers can purchase the
[Global Platform Compliance Test suite](https://www.globalplatform.org/store.asp).
This test suite comes with .xml files describing the tests and
the Trusted Applications.

Standard tests can be extended with the Global Platform test suite.
The user must only:
* Install the Global Platform `xml` files in `$CFG_GP_PACKAGE_PATH`
* Run `make patch` (or call make `xtest-patch` from the `build` repository)
  before compiling xtest. This must be run a single time after the installation
  of OP-TEE.

This will:
* Create new Trusted Applications, that can be found in `ta/GP_xxx`
* Create new tests in `host/xtest`, as for example `xtest_9000.c`
* Patches `xtest_7000.c`, adding new tests.

Then the tests must be compiled with `CFG_GP_PACKAGE_PATH=<path>`.


### HOWTO run xtest

	# all xtest
	boot and execute on your target
	$ modprobe optee_armtz
	$ tee-supplicant &
	$ xtest

	# single xtest
	boot and execute on your target
	$ modprobe optee_armtz
	$ tee-supplicant &
	$ xtest <testnumber> (i.e.: xtest 1001)

	# family xtest (i.e.: Family 1000)
	boot and execute on your target
	$ modprobe optee_armtz
	$ tee-supplicant &
	$ xtest _<family> (i.e.: xtest _1)

#### Compiler flags
To be able to see the full command when building you could build using following
flag:

`$ make V=1`

To state where build files are stored use the `O` flag.

`$ make O=$HOME/foo`

By default `optee_test` expects that `optee_client` is located at the same
folder level. However if you build optee_client in another location, then you
also would need to use (or export) the following flag:

`$ make OPTEE_CLIENT_PATH=$HOME/my_new_location`

## Coding standards
In this project we are trying to adhere to the same coding convention as used in
the Linux kernel (see
[CodingStyle](https://www.kernel.org/doc/Documentation/CodingStyle)). We achieve this by running
[checkpatch](http://git.kernel.org/cgit/linux/kernel/git/torvalds/linux.git/tree/scripts/checkpatch.pl) from Linux kernel.
However there are a few exceptions that we had to make since the code also
follows GlobalPlatform standards. The exceptions are as follows:

- CamelCase for GlobalPlatform types are allowed.
- And we also exclude checking third party code that we might use in this
  project, such as LibTomCrypt, MPA, newlib (not in this particular git, but
  those are also part of the complete TEE solution). The reason for excluding
  and not fixing third party code is because we would probably deviate too much
  from upstream and therefore it would be hard to rebase against those projects
  later on (and we don't expect that it is easy to convince other software
  projects to change coding style).

### checkpatch
Since checkpatch is licensed under the terms of GNU GPL License Version 2, we
cannot include this script directly into this project. Therefore we have
written the Makefile so you need to explicitly point to the script by exporting
an environment variable, namely CHECKPATCH. So, suppose that the source code for
the Linux kernel is at `$HOME/devel/linux`, then you have to export like follows:

	$ export CHECKPATCH=$HOME/devel/linux/scripts/checkpatch.pl
thereafter it should be possible to use one of the different checkpatch targets
in the [Makefile](Makefile). There are targets for checking all files, checking
against latest commit, against a certain base-commit etc. For the details, read
the [Makefile](Makefile).
