/**
 * @file mem_pool.h
 * @brief tuya memory pool module
 * @version 1.0
 * @date 2019-10-30
 *
 * @copyright Copyright (c) tuya.inc 2019
 *
 */
#ifndef _MEM_POOL_H
#define _MEM_POOL_H

#include "tuya_cloud_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief system memory debug flag
 *
 */
#define SYS_MEM_DEBUG 0

/**
 * @brief system memory debug  display flag
 *
 */
#define SHOW_MEM_POOL_DEBUG 1

/**
 * @brief support multi thread memory process
 *
 */
#define MEM_POOL_MUTLI_THREAD 1

/**
 * @brief the memory partition
 *
 */
typedef struct {        // MEMORY CONTROL BLOCK
	VOID *pMemAddr;     // Pointer to beginning of memory partition
	VOID *pMemAddrEnd;  // Pointer to ending of memory partition
	VOID *pMemFreeList; // Pointer to list of free memory blocks
	DWORD_T memBlkSize; // Size (in bytes) of each block of memory
	DWORD_T memNBlks;   // Total number of blocks in this partition
	DWORD_T memNFree;   // Number of memory blocks remaining in this partition
} MEM_PARTITION;

/**
 * @brief the memory pool table
 *
 */
typedef struct {
	DWORD_T memBlkSize; // 块大小 >=2
	DWORD_T memNBlks;   // 块数量
} MEM_POOL_SETUP_TBL;

/**
 * @brief setup the memory pool use default memory pool table and partition number
 *
 * @return OPRT_OK on success, others on failed, please refer to tuya_error_code.h
 */
OPERATE_RET SysMemoryPoolSetup(VOID);

/**
 * @brief setup the memory pool use user defined memory pool table and partition number
 *
 * @param[in] tbl the memory pool table
 * @param[in] cnt the partition number
 * @return OPRT_OK on success, others on failed, please refer to tuya_error_code.h
 */
OPERATE_RET SysMemPoolSetupUserDef(IN CONST MEM_POOL_SETUP_TBL *tbl, IN CONST INT_T cnt);

/**
 * @brief delete memory pool
 *
 * @return VOID
 */
VOID SysMemoryPoolDelete(VOID);

/**
 * @brief create memory partotion
 *
 * @param[in] nblks the number of memory block in partition
 * @param[in] blksize the size of memory block in partition
 * @param[out] ppMemPartition the memory partition
 * @return OPRT_OK on success, others on failed, please refer to tuya_error_code.h
 */
OPERATE_RET MemPartitionCreate(IN CONST DWORD_T nblks, IN CONST DWORD_T blksize,
                               OUT MEM_PARTITION **ppMemPartition);

/**
 * @brief delete memory partition
 *
 * @param[in] pMemPart the memory partition need to delete
 * @return OPRT_OK on success, others on failed, please refer to tuya_error_code.h
 */
OPERATE_RET MemPartitionDelete(IN MEM_PARTITION *pMemPart);

/**
 * @brief get a memory block from a partition
 *
 * @param[in] pMemPart the input memory partition
 * @param[out] pOprtRet the operation return value, OPRT_OK on success, others on failed, please
 * refer to tuya_error_code.h
 * @return the memory block address
 */
VOID *MemBlockGet(INOUT MEM_PARTITION *pMemPart, OUT OPERATE_RET *pOprtRet);

/**
 * @brief release memory block to partition
 *
 * @param[in] pMemPart the memory partition
 * @param[in] pblk the memory block address need to release
 * @return OPRT_OK on success, others on failed, please refer to tuya_error_code.h
 */
OPERATE_RET MemBlockPut(INOUT MEM_PARTITION *pMemPart, IN CONST VOID *pblk);

/**
 * @brief malloc memory from memory pool
 *
 * @param[in] reqSize the size of memory required
 * @param[in] pRet the operation return value, OPRT_OK on success, others on failed, please refer to
 * tuya_error_code.h
 * @return the address of the memory
 */
VOID *MallocFromSysMemPool(IN CONST DWORD_T reqSize, OPERATE_RET *pRet);

/**
 * @brief free the memory to memory pool
 *
 * @param[in] pReqMem the memory which got from MallocFromSysMemPool function
 * @return OPRT_OK on success, others on failed, please refer to tuya_error_code.h
 */
OPERATE_RET FreeFromSysMemPool(IN PVOID_T pReqMem);

#if SHOW_MEM_POOL_DEBUG
/**
 * @brief display the memory pool detail info, only valid when SHOW_MEM_POOL_DEBUG=1
 *
 * @return VOID
 */
VOID ShowSysMemPoolInfo(VOID);
#endif

#if SYS_MEM_DEBUG
/**
 * @brief a debug function which can record the file and line infomation when call Malloc
 *
 * @param[in] reqSize the required memory size
 * @param[in] file the file name where Malloc called
 * @param[in] line the line where Malloc called
 * @return the memory address got from system memory pool
 */
VOID *__Malloc(IN CONST DWORD_T reqSize, IN CONST CHAR_T *file, IN CONST INT_T line);

/**
 * @brief a debug function which can record the file and line infomation when call Free
 *
 * @param[in] pReqMem the memory address which need to free
 * @param[in] file the file name where Free called
 * @param[in] line the line where Free called
 * @return VOID
 */
VOID __Free(IN PVOID_T pReqMem, IN CONST CHAR_T *file, IN CONST INT_T line);

/**
 * @brief malloc memory
 *
 * @param[in] reqSize the required memory size
 * @return  NULL on failed, others on the address of the memory
 */
#define Malloc(x) __Malloc(x, __FILE__, __LINE__)

/**
 * @brief malloc consecutive memory, the size is reqCount*reqSize
 *
 * @param[in] reqCount the required memory count
 * @param[in] reqSize the required memory szie
 * @return NULL on failed, others on the address of the memory
 */
#define Calloc(x) __Calloc(x, y, __FILE__, __LINE__)

/**
 * @brief free memory
 *
 * @param[in] pReqMem the memory address which got from Malloc or Calloc function
 * @return VOID
 */
#define Free(x) __Free(x, __FILE__, __LINE__)

#else

/**
 * @brief malloc memory
 *
 * @param[in] reqSize the required memory size
 * @return  NULL on failed, others on the address of the memory
 */
VOID *Malloc(IN SIZE_T reqSize);

/**
 * @brief malloc consecutive memory, the size is reqCount*reqSize
 *
 * @param[in] reqCount the required memory count
 * @param[in] reqSize the required memory szie
 * @return NULL on failed, others on the address of the memory
 */
VOID *Calloc(IN CONST DWORD_T reqCount, IN SIZE_T reqSize);

/**
 * @brief free memory
 *
 * @param[in] pReqMem the memory address which got from Malloc or Calloc function
 * @return VOID
 */
VOID Free(IN PVOID_T pReqMem);
#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
