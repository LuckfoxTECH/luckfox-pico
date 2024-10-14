# RK3399PRO Release Note

## rk3399pro_bl32_v2.12.bin

| Date       | file                     | Build commit | Severity  |
| ---------- | ------------------------ | ------------ | --------- |
| 2024-04-16 | rk3399pro_bl32_v2.12.bin | 369edd59c    | important |

### New

1. Optimization parameter check to enhance security.
2. Support printing TEE memory usage information.
3. Support printing FWVER information.
4. Support pstore for optee log.

------

## rk3399pro_bl32_v2.11.bin

| Date       | file                     | Build commit | Severity |
| ---------- | ------------------------ | ------------ | -------- |
| 2023-08-14 | rk3399pro_bl32_v2.11.bin | 44e25f04     | critical |

### Fixed

| Index | Severity  | Update                        | Issue description                                            | Issue source |
| ----- | --------- | ----------------------------- | ------------------------------------------------------------ | ------------ |
| 1     | critical  | Fix security vulnerabilities. | Hackers can exploit vulnerabilities to attack OPTEE OS.      | -            |
| 2     | important | Fix memory leaks.             | Customer calls TEE_ DerivekeyFromHard may experience memory leakage issues. | 374096       |
| 3     | important | Enable efuse dependent clock  | When program efuse, it relies on the saradc clock. Failure to turn on the clock may result in program efuse failure. |              |

------

## rk3399pro_ddr_{666...933}MHz_v1.30.bin

| Date       | file                                   | Build commit | Severity  |
| ---------- | -------------------------------------- | ------------ | --------- |
| 2023-04-17 | rk3399pro_ddr_{666...933}MHz_v1.30.bin | aae4a89176   | important |

### Fixed

| Index | Severity  | Update                  | Issue description                               | Issue source |
| ----- | --------- | ----------------------- | ----------------------------------------------- | ------------ |
| 1     | important | Fixed LP3 reboot error. | When LP3 reboot, it will  stuck in ddr initial. | -            |

------

## rk3399pro_ddr_{666...933}MHz_v1.29.bin

| Date       | file                                   | Build commit | Severity  |
| ---------- | -------------------------------------- | ------------ | --------- |
| 2023-03-30 | rk3399pro_ddr_{666...933}MHz_v1.29.bin | 5b2c650      | important |

### Fixed

| Index | Severity  | Update                                                | Issue description                                            | Issue source |
| ----- | --------- | ----------------------------------------------------- | ------------------------------------------------------------ | ------------ |
| 1     | moderate  | Fixed single channel loop at "advanced training done" | When in single channel mode, it will loop at "advanced training done". | -            |
| 2     | important | Fixed LP3 dbw detect bug                              | The dbw detect error when LP3                                | -            |

------

