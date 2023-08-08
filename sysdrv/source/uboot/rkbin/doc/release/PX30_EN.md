# PX30 Release Note

## px30_ddr_333MHz_v2.08.bin

| Date       | file                      | Build commit | Severity |
| ---------- | ------------------------- | ------------ | -------- |
| 2022-10-27 | px30_ddr_333MHz_v2.08.bin | e5b9702      | moderate |

### New

1. reserved words for maskrom to add special parameters

------

## px30_bl31_v1.32.elf

| Date       | file                | Build commit | Severity  |
| ---------- | :------------------ | ------------ | --------- |
| 2022-10-25 | px30_bl31_v1.32.elf | 6f45bba91    | important |

### Fixed

| Index | Severity  | Update                         | Issue description                     | Issue source |
| ----- | --------- | ------------------------------ | ------------------------------------- | ------------ |
| 1     | important | Compatible with the old loader | Boot fail if old loader+new trust.img | -            |

------

## px30_bl32_v2.14.bin

| Date       | File                | Build commit | Severity  |
| ---------- | :------------------ | ------------ | --------- |
| 2022-09-16 | px30_bl32_v2.14.bin | d84087907    | important |

### Fixed

| Index | Severity  | Update                                                       | Issue description                                            | Issue source |
| ----- | --------- | ------------------------------------------------------------ | ------------------------------------------------------------ | ------------ |
| 1     | important | Solve the problem that OPTEE is stuck during startup when printing is closed | User use /rkbin/tools/ddrbin_tool to close printing ,  then rk_atags will notify OPTEE to disable printing, When OPTEE starts, it will be stuck and unable to enter U-Boot | -            |

------

