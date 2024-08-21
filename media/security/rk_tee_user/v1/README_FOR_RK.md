########### OP-TEE testsuite and demo by Rockchip ##########

The optee_test git contains the source code for the TEE
test suite in Linux using the ARM(R) TrustZone(R) technology.

##### HOWTO build the testsuite #####

1 build CA/TA
* mm Android.mk in optee_test to build CA(Client Application,
  running in normal world) and TA(Trust Application, running 
  in secure world).

* OR if you only have NDK environment, you can use build.sh 
  to build TA and use ndk_build.sh to build CA.

##### HOWTO run the testsuite #####

1 adb push the files mentioned above to machine.
* put libteec.so in /system/lib/ and tee_supplicant in /system/bin/.
* put CA in /system/bin/.
* mkdir optee_armtz in /system/lib, then put TA in system/lib/optee_armtz.

2 run tee_supplicant
* run tee_supplicant as root in background.
  $ su
  # tee_supplicant &

3 run CA
* run CA.
  # testapp
  # testapp_storage
  ...

##### NOTICE #####

1 After each boot, you need run tee_supplicant first, otherwise CA/TA
  can not complete interaction.
