# ipcweb-backend

> A IP Camera Web Backend Reference Implementation

---

[TOC]

# 1. Getting Started

## 1.1 Build Requirements

ipcweb-backend is designed to run on Linux platform, especially the Linux platform of Rockchip's SoCs. It is cross compiled with the following environments:

* Build Host: Ubuntu
* Host Web Server: Any web server that supports CGI application, like Lighttpd, Nginx, Apache, AppWeb etc.
* Build System: [CMake](https://cmake.org) >= 3.8
* Compiler: GCC >= 6 or Clang
* [PostMan](https://www.getpostman.com/downloads/)
* Host Libraries(To test APIs on host environments)
  * [Boost](https://www.boost.org)
  ```console
  sudo apt install libboost-all-dev
  ```
  * [Cgicc](https://www.gnu.org/software/cgicc/)
  ```console
  sudo apt install libcgicc-dev
  ```
  * [JSON for Modern C++](https://github.com/nlohmann/json)
  ```console
  sudo apt install nlohmann-json-dev
  ```
  * [jwt-cpp](https://github.com/Thalhammer/jwt-cpp)
  This library is not provided by system, it is build from souce

* Target Libraries
  The target libraries should be provided by the Buildroot SDK, if them are not found, the CMake build system build them from source.
  ```
  BR2_PACKAGE_BOOST=y
  BR2_PACKAGE_BOOST_LOG=y
  BR2_PACKAGE_BOOST_STACKTRACE=y
  BR2_PACKAGE_BOOST_SYSTEM=y
  BR2_PACKAGE_BOOST_THREAD=y

  BR2_PACKAGE_LIBCGICC

  BR2_PACKAGE_JSON_FOR_MODERN_CPP
  ```

## 1.2 Build Instructions

  ```console
  mkdir build
  cd build
  cmake .. -DCMAKE_TOOLCHAIN_FILE=../cmake/toolchains/linux-arm.cmake
  make -j8
  ```

## 1.5 Unit Testing

  This project use Google Test as test framework. Init the googletest submodule to use it.

  ```console
  git submodule init
  git submodule update
  ```

  Build and test

  ```console
  cmake .. -DIPCWEB_BUILD_TESTS
  make
  make test
  ```

  The output is like:

  ```console
  Running tests...
  Test project /work/www/ipcweb-backend/build
      Start 1: ObjectsTest.Tcpip.noArgs
  1/1 Test #1: ObjectsTest.Tcpip.noArgs .........   Passed    0.00 sec

  100% tests passed, 0 tests failed out of 1

  Total Test time (real) =   0.00 sec
  ```

## 1.4 CGI Testing

  Start the web server to test CGI applications, suggest to use AppWeb.

  ```console
  appweb <path/of/web/root> <host>:<port>
  ```

  Use tools like Postman to test the RESTful API

  > GET http://<host>:<port>/api/v1/hello.cgi?name=cody
