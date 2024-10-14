/*****************************************************************************
**
**  Name:       goep_fs.h
**
**  File:       Object Storage API
**
**  Copyright (c) 2000-2009, Broadcom Corp., All Rights Reserved.
**  WIDCOMM Bluetooth Core. Proprietary and confidential.
**
*****************************************************************************/
#ifndef GOEP_FS_H
#define GOEP_FS_H

#include "bt_target.h"

/*****************************************************************************
**     Constants
*****************************************************************************/

/* Flags passed to the open function (tGOEP_OPEN_CBACK)
**      Values are OR'd together. (First 3 are
**      mutually exclusive.
*/
#define GOEP_O_RDONLY            0x0000
#define GOEP_O_WRONLY            0x0001
#define GOEP_O_RDWR              0x0002

#define GOEP_O_CREAT             0x0100
#define GOEP_O_EXCL              0x0200
#define GOEP_O_TRUNC             0x1000


#define GOEP_LEN_UNKNOWN          0xFFFFFFFF
#define GOEP_INVALID_FD          (-1)

/* Values passed to the access function (tGOEP_ACCESS_CBACK)
*/
#define GOEP_ACC_EXIST           0x0
#define GOEP_ACC_READ            0x4
#define GOEP_ACC_RDWR            0x6

/* Constants used in directory listing structure */
#define GOEP_A_RDONLY            0x1
#define GOEP_A_DIR               0x2      /* Entry is a sub directory */

#define GOEP_CTIME_LEN           17       /* Creation time "yyyymmddTHHMMSSZ" */

/*****************************************************************************
**     Seek Constants
*****************************************************************************/
/* Origin for the seek function (tGOEP_SEEK_CBACK) */
#define GOEP_SEEK_SET    0
#define GOEP_SEEK_CUR    1
#define GOEP_SEEK_END    2



/*****************************************************************************
**    Typedefs
*****************************************************************************/
typedef INT32   tGOEP_FD;

enum
{
    GOEP_OK,
    GOEP_FAIL,
    GOEP_EACCES,
    GOEP_ENOTEMPTY,
    GOEP_EOF,
    GOEP_EODIR,
    GOEP_ENOSPACE,
    GOEP_EIS_DIR,
    GOEP_RESUME,
    GOEP_NONE
};
typedef UINT16 tGOEP_STATUS;

/* Structure passed in Directory Entry Callback to be filled in */
typedef struct
{
    UINT32  refdata;                /* holder for OS specific data used to get next entry */
    UINT32  filesize;
    char    crtime[GOEP_CTIME_LEN]; /* "yyyymmddTHHMMSSZ", or "" if none */
    char   *p_name;                 /* Contains the addr of memory to copy name into */
    UINT8   mode;                   /* GOEP_A_RDONLY and/or GOEP_A_DIR */
} tGOEP_DIRENTRY;


/*****************************************************************************
**    Typedefs for messages from response functions
*****************************************************************************/
typedef struct
{
    BT_HDR          hdr;
    tGOEP_FD        fd;
    tGOEP_STATUS    status;
    UINT32          file_size;
} tGOEP_OPEN_RSP;

typedef struct
{
    BT_HDR          hdr;
    tGOEP_FD        fd;
    tGOEP_STATUS    status;
    UINT16          bytes_read;
} tGOEP_READ_RSP;

typedef struct
{
    BT_HDR          hdr;
    tGOEP_FD        fd;
    tGOEP_STATUS    status;
} tGOEP_WRITE_RSP;

typedef struct
{
    BT_HDR       hdr;
    tGOEP_STATUS status;
} tGOEP_DIRENTRY_RSP;

/*****************************************************************************
**    Object Store Interface
*****************************************************************************/
/*******************************************************************************
**
** Callback Function: tGOEP_OPEN_CBACK
**
** Description  This function is executed by OBX profiles to open
**              a file for reading or writing.
**
** Parameters   p_path      - Fully qualified path and file name.
**              flags       - permissions and mode (see constants above)
**              size        - size of file to put (0 if unavailable or not applicable)
**              event_id    - code that must be passed to the call-in function.
**
** Returns      void
**
** Note:        Upon completion of the request, a file descriptor (tGOEP_FD),
**              file size (UINT32), and an status code (tGOEP_STATUS)
**              are returned in GOEP_OpenRsp().
**
*******************************************************************************/
typedef void (tGOEP_OPEN_CBACK)(const UINT8 *p_name, UINT16 flags, UINT32 size,
                                UINT16 event_id, UINT8 app_id);

/*******************************************************************************
**
** Callback Function: tGOEP_CLOSE_CBACK
**
** Description  This function is executed by OBX profiles when the file descriptor
**              is no longer in use.
**
** Returns      void
**
*******************************************************************************/
typedef void (tGOEP_CLOSE_CBACK)(tGOEP_FD fd, UINT8 app_id);

/*******************************************************************************
**
** Callback Function: tGOEP_READ_CBACK
**
** Description  This function is executed by OBX profiles to read in data from the
**              previously opened file.
**
** Returns      void
**
** Note:        Upon completion of the request, GOEP_ReadRsp() is
**              called with the buffer of data, along with the number
**              of bytes read into the buffer, and a status.  The
**              call-in function should only be called when ALL requested
**              bytes have been read, the end of file has been detected,
**              or an error has occurred.
**
*******************************************************************************/
typedef void (tGOEP_READ_CBACK)(tGOEP_FD fd, void *p_data, INT16 size,
                                UINT16 event_id, UINT8 app_id);

/*******************************************************************************
**
** Callback Function: tGOEP_WRITE_CBACK
**
** Description  This function is executed by OBX profiles to write the data to the
**              previously opened file.
**
** Returns      void
**
** Note:        Upon completion of the request, GOEP_WriteRsp() is
**              called with the file descriptor and the status.  The
**              call-in function should only be called when ALL requested
**              bytes have been written, or an error has been detected,
**
*******************************************************************************/
typedef void (tGOEP_WRITE_CBACK)(tGOEP_FD fd, const void *p_data, INT16 size,
                                 UINT16 event_id, UINT8 app_id);

/*******************************************************************************
**
** Callback Function: tGOEP_SEEK_CBACK
**
** Description  This function is executed by OBX profiles to Move a file pointer
**              of a previously opened file to the specified location for the
**              next read or write operation.
**
** Returns      void
**
*******************************************************************************/
typedef void (tGOEP_SEEK_CBACK)(tGOEP_FD fd, INT32 offset, INT16 origin, UINT8 app_id);


/*******************************************************************************
**
** Callback Function: tGOEP_DIRENTRY_CBACK
**
** Description  This function is called to retrieve a directory entry for the
**              specified path.  The first/next directory should be filled
**              into the location specified by p_entry.
**
** Parameters   p_path     - directory to search (Fully qualified path)
**              first_item - TRUE if first search, FALSE if next search
**                                      (p_cur contains previous)
**              p_entry (input/output) - Points to last entry data (valid when
**                                           first_item is FALSE)
**              event_id - event that must be passed into the call-in function.
**
** Returns      void
**
** Note:        Upon completion of the request, GOEP_DirentryRsp() is
**              filled in entry and the status.
**              GOEP_OK is returned when p_entry is valid,
**              GOEP_EODIR is returned when no more entries [finished]
**              GOEP_FAIL is returned if an error occurred
**
*******************************************************************************/
typedef void (tGOEP_DIRENTRY_CBACK)(const char *p_path, BOOLEAN first_item,
                                    tGOEP_DIRENTRY *p_entry, UINT16 event_id,
                                    UINT8 app_id);

/*******************************************************************************
**
** Callback Function: tGOEP_ACCESS_CBACK
**
** Description  This function is called to check the existence of a file or
**              directory.
**
** Returns      (tGOEP_STATUS) status of the call.
**               [GOEP_OK if it exists]
**               [GOEP_EACCES if permissions are wrong]
**               [GOEP_FAIL if it does not exist]
**
*******************************************************************************/
typedef tGOEP_STATUS(tGOEP_ACCESS_CBACK)(const char *p_path, UINT16 mode,
        BOOLEAN *p_is_dir, UINT8 app_id);

/*******************************************************************************
**
** Callback Function: tGOEP_MKDIR_CBACK
**
** Description  This function is called to create a directory with
**              the pathname given by path. The pathname is a null terminated
**              string. All components of the path must already exist.
**
** Parameters   p_path   - (input) name of directory to create (fully qualified path).
**
** Returns      (tGOEP_STATUS) status of the call.
**               [GOEP_OK if successful]
**               [GOEP_FAIL if unsuccessful]
**
*******************************************************************************/
typedef tGOEP_STATUS(tGOEP_MKDIR_CBACK)(const char *p_path, UINT8 app_id);

/*******************************************************************************
**
** Callback Function: tGOEP_RMDIR_CBACK
**
** Description  This function is called to remove a directory whose
**                  name is given by path. The directory must be empty.
**
** Parameters   p_path   - (input) name of directory to remove (fully qualified path).
**
** Returns      (tGOEP_STATUS) status of the call.
**               [GOEP_OK if successful]
**               [GOEP_EACCES if read-only]
**               [GOEP_ENOTEMPTY if directory is not empty]
**               [GOEP_FAIL otherwise]
**
*******************************************************************************/
typedef tGOEP_STATUS(tGOEP_RMDIR_CBACK)(const char *p_path, UINT8 app_id);

/*******************************************************************************
**
** Callback Function: tGOEP_UNLINK_CBACK
**
** Description  This function is called to remove a directory whose
**                  name is given by path. The directory must be empty.
**
** Parameters   p_path   - (input) name of file to remove (fully qualified path).
**
** Returns      (tGOEP_STATUS) status of the call.
**               [GOEP_OK if successful]
**               [GOEP_EACCES if read-only]
**               [GOEP_FAIL otherwise]
**
*******************************************************************************/
typedef tGOEP_STATUS(tGOEP_UNLINK_CBACK)(const char *p_path, UINT8 app_id);


/*****************************************************************************
**     Prototypes
*****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
**
**  Function:    GOEP_OpenRsp
**
**  Purpose:     Report the status of tGOEP_OPEN_CBACK callback function.
**
**  Parameters:  fd         - File handle.
**               status     - Status of the operation.
**               file_size  - total number of bytes in this file.
**               event_id   - event id as given in the tGOEP_OPEN_CBACK function.
**
**  Returns:     void
**
*****************************************************************************/
GOEP_API extern void GOEP_OpenRsp(tGOEP_FD fd, tGOEP_STATUS status,
                                  UINT32 file_size, UINT16 event_id);

/*****************************************************************************
**
**  Function:    GOEP_ReadRsp
**
**  Purpose:     Report the status of tGOEP_READ_CBACK callback function.
**
**  Parameters:  fd         - File handle.
**               status     - Status of the operation.
**               bytes_read - total number of bytes read from the file.
**               event_id   - event id as given in the tGOEP_READ_CBACK function.
**
**  Returns:     void
**
*****************************************************************************/
GOEP_API extern void GOEP_ReadRsp(tGOEP_FD fd, tGOEP_STATUS status,
                                  UINT16 bytes_read, UINT16 event_id);

/*****************************************************************************
**
**  Function:    GOEP_WriteRsp
**
**  Purpose:     Report the status of tGOEP_WRITE_CBACK callback function.
**
**  Parameters:  fd         - File handle.
**               status     - Status of the operation.
**               event_id   - event id as given in the tGOEP_WRITE_CBACK function.
**
**  Returns:     void
**
*****************************************************************************/
GOEP_API extern void GOEP_WriteRsp(tGOEP_FD fd, tGOEP_STATUS status, UINT16 event_id);

/*******************************************************************************
**
** Function     GOEP_DirentryRsp
**
** Description  This function is called in response to the
**              tGOEP_DIRENTRY_CBACK function with a filled in directory listing
**              entry.
**
** Parameters   status - GOEP_OK if p_entry points to a valid entry.
**                       GOEP_EODIR if no more entries (p_entry is ignored).
**                       GOEP_FAIL if any errors have occurred.
**              event_id - event id as given in the tGOEP_DIRENTRY_CBACK function.
**
** Returns      void
**
*******************************************************************************/
GOEP_API extern void GOEP_DirentryRsp(tGOEP_STATUS status, UINT16 event_id);

#ifdef __cplusplus
}
#endif

#endif /* GOEP_FS_H */
