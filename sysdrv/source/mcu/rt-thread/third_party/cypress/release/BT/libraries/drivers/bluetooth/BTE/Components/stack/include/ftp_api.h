/*****************************************************************************
**
**  Name:       FTP_api.h
**
**  File:       FTP API Definitions
**
**  Copyright (c) 2000-2004, WIDCOMM Inc., All Rights Reserved.
**  WIDCOMM Bluetooth Core. Proprietary and confidential.
**
*****************************************************************************/
#ifndef FTP_API_H
#define FTP_API_H

#include "bt_target.h"
#include "btm_api.h"
#include "goep_fs.h"   /* object filesystem */

/* file system interfaces */
typedef struct
{
    tGOEP_OPEN_CBACK     *p_open;
    tGOEP_CLOSE_CBACK    *p_close;
    tGOEP_READ_CBACK     *p_read;
    tGOEP_WRITE_CBACK    *p_write;
    tGOEP_DIRENTRY_CBACK *p_dirent;      /* Used by Server Only */
    tGOEP_ACCESS_CBACK   *p_access;
    tGOEP_UNLINK_CBACK   *p_unlink;
    tGOEP_RMDIR_CBACK    *p_rmdir;      /* Used by Server Only */
    tGOEP_MKDIR_CBACK    *p_mkdir;      /* Used by Server Only */
} tFTP_GOEP_CBACKS;

/* Structure used to report file transfer status */
typedef struct
{
    UINT32 file_size;   /* Total size of file (GOEP_LEN_UNKNOWN if unknown) */
    UINT16 bytes;       /* Number of bytes read or written since last progress event */
} tFTP_PROGRESS;

/**************************
**  Server Definitions
***************************/
/* Extra Debug Code */
#ifndef FTP_SRVR_DEBUG
#define FTP_SRVR_DEBUG          FALSE
#endif

/* Access event operation types */
#define FTP_OPER_PUT            1   /* Request is a PUT file */
#define FTP_OPER_GET            2   /* Request is a GET file */
typedef UINT8 tFTP_OPER;

/* Access response types */
#define FTP_ACCESS_ALLOW        0   /* Allow the requested operation */
#define FTP_ACCESS_FORBID       1   /* Disallow the requested operation */
typedef UINT8 tFTP_ACCESS;

/* Server callback function events */
#define FTP_SRVR_ENABLE_EVT     0   /* File transfer server is enabled. */
#define FTP_SRVR_OPEN_EVT       1   /* Connection to peer is open. */
#define FTP_SRVR_CLOSE_EVT      2   /* Connection to peer closed. */
#define FTP_SRVR_AUTH_EVT       3   /* Request for Authentication key and realm */
#define FTP_SRVR_ACCESS_EVT     4   /* Request for access to put a file */
#define FTP_SRVR_PROGRESS_EVT   5   /* Number of bytes read or written so far */

typedef UINT8 tFTP_SRVR_EVT;

typedef struct
{

    UINT8  *p_userid;
    UINT8   userid_len;
    BOOLEAN userid_required;    /* TRUE if user ID is required in response (rechallanged)  */
} tFTP_SRVR_AUTH;

typedef struct
{
    char          *p_name;      /* file name */
    UINT32         size;        /* file size */
    tFTP_OPER      oper;        /* operation (put, get) */
} tFTP_SRVR_ACCESS;

typedef union
{
    tFTP_PROGRESS     prog;
    tFTP_SRVR_AUTH    auth;
    tFTP_SRVR_ACCESS  access;
} tFTP_SRVR;

/* Server callback function */
typedef void tFTP_SRVR_CBACK(tFTP_SRVR_EVT event, tFTP_SRVR *p_data);

/******************
** api interfaces *
*******************/
/* Enable structure */
typedef struct
{
    tFTP_SRVR_CBACK  *p_cback;
    char              root_path[GOEP_MAX_PATH_SIZE + 1];
    char              servicename[BT_MAX_SERVICE_NAME_LEN + 1];
    UINT8             realm [OBX_MAX_REALM_LEN];    /* The realm is intended to be
                                                       displayed to users so they know
                                                       which userid and password to use.
                                                       The first byte of the string is
                                                       the character set of the string.
                                                     */
    UINT8             realm_len;        /* 0 if not used */
    UINT8             sec_mask;
    BOOLEAN           auth_enabled;
    UINT8             app_id;
} tFTP_SRVR_ENABLE;


/**************************
**  Client Definitions
***************************/
/* Extra Debug Code */
#ifndef FTP_CLNT_DEBUG
#define FTP_CLNT_DEBUG          FALSE
#endif

/* Client callback function events */
#define FTP_CLNT_OPEN_EVT       0   /* Connection to peer is open. */
#define FTP_CLNT_CLOSE_EVT      1   /* Connection to peer closed. */
#define FTP_CLNT_AUTH_EVT       2   /* Request for Authentication key and user id */
#define FTP_CLNT_LIST_EVT       3   /* Event contains a directory entry (tFTP_CLNT_LIST) */
#define FTP_CLNT_PROGRESS_EVT   4   /* Number of bytes read or written so far */
#define FTP_CLNT_PUTFILE_EVT    5   /* Put File complete */
#define FTP_CLNT_GETFILE_EVT    6   /* Get File complete */
#define FTP_CLNT_CHDIR_EVT      7   /* Change Directory complete */
#define FTP_CLNT_MKDIR_EVT      8   /* Make Directory complete */
#define FTP_CLNT_REMOVE_EVT     9   /* Remove File/Directory complete */
#define FTP_CLNT_SEARCH_EVT     10  /* Service Search complete */

typedef UINT8 tFTP_CLNT_EVT;


#define FTP_CLNT_OK             0
#define FTP_CLNT_FAIL           1
#define FTP_CLNT_NO_PERMISSION  2
#define FTP_CLNT_NOT_FOUND      3
#define FTP_CLNT_FULL           4
#define FTP_CLNT_BUSY           5
#define FTP_CLNT_ABORTED        6

typedef UINT8 tFTP_CLNT_STATUS;

typedef struct
{
    tFTP_CLNT_STATUS status;   /* Fields are valid when status is FTP_CLNT_OK */
    UINT8           *data;
    UINT16           len;
    BOOLEAN          final;     /* If TRUE, entry is last of the series */
} tFTP_CLNT_LIST;

typedef struct
{
    UINT8  *p_realm;
    UINT8   realm_len;
    UINT8   realm_charset;
    BOOLEAN userid_required;    /* If TRUE, a user ID must be sent */
} tFTP_CLNT_AUTH;

typedef struct
{
    tFTP_CLNT_STATUS status;    /* Fields are valid when status is FTP_CLNT_OK */
    UINT8           *p_name;    /* Name of the service (NULL if not in service record) */
    UINT16           name_len;  /* Length in bytes of name */
    UINT16           lang_code; /* Language encoding: ISO 639:1988 (E/F) */
    UINT16           char_code; /* Character encoding: MIBEnum value */
    UINT16           base_id;   /* Base attribute ID */
    UINT16           profile_version; /* Profile Version (0 if not returned) */
} tFTP_CLNT_SEARCH;

typedef union
{
    tFTP_CLNT_STATUS status;
    tFTP_CLNT_LIST   list;
    tFTP_PROGRESS    prog;
    tFTP_CLNT_AUTH   auth;
    tFTP_CLNT_SEARCH search;
} tFTP_CLNT;

/* Client callback function */
typedef void tFTP_CLNT_CBACK(tFTP_CLNT_EVT event, tFTP_CLNT *p_data);

/*****************************************************************************
**  External Function Declarations
*****************************************************************************/
#ifdef __cplusplus
extern "C"
{
#endif

/**************************
**  Common Functions
***************************/

/*******************************************************************************
**
** Function         FTP_SetFsCbacks
**
** Description      This function is called once before any FTP API function is
**                  used to set the file system access functions in FTP
**                  control block.
**
** Returns          void
**
*******************************************************************************/
FTP_API extern void FTP_SetFsCbacks(tFTP_GOEP_CBACKS *p_fs_cbacks);

/*******************************************************************************
**
** Function         FTP_Init
**
** Description      This function is called once at stack startup to allocate
**                  (if using dynamic memory) and initialize the
**                  control block, and initializes the
**                  debug tracing level.
**
** Returns          void
**
*******************************************************************************/
FTP_API extern void FTP_Init(void);

/*****************************************************************************
**
**  Function        FTP_SetTraceLevel
**
**  Description     This function changes the trace level (debugging levels)
**
**  Returns         void
**
*****************************************************************************/
FTP_API extern void FTP_SetTraceLevel(UINT8 level);


/**************************
**  Server Functions
***************************/

/*******************************************************************************
**
** Function         FTP_SrvrEnable
**
** Description      Enable the file transfer server.  This function must be
**                  called before any other functions in the FTS API are called.
**                  When the enable operation is complete the callback function
**                  will be called with an FTP_SRVR_ENABLE_EVT event.
**
**
** Returns          void
**
*******************************************************************************/
FTP_API extern void FTP_SrvrEnable(tFTP_SRVR_ENABLE *p_info);

/*******************************************************************************
**
** Function         FTP_SrvrClose
**
** Description      If the server is currently connected to a peer
**                  device the connection will be closed.  The server remains
**                  in listen mode.
**
** Returns          void
**
*******************************************************************************/
FTP_API extern void FTP_SrvrClose(void);

/*******************************************************************************
**
** Function         FTP_SrvrDisable
**
** Description      Disable the file transfer server.  If the server is currently
**                  connected to a peer device the connection will be closed.
**
** Returns          void
**
*******************************************************************************/
FTP_API extern void FTP_SrvrDisable(void);

/*******************************************************************************
**
** Function         FTP_SrvrAuthRsp
**
** Description      Sends an OBEX authentication challenge to the connected
**                  OBEX client. Called in response to an FTP_SRVR_AUTH_EVT event.
**                  Used when "enable_authen" is set to TRUE in FTP_SrvrEnable().
**
**                  Note: If the "userid_required" is TRUE in the FTP_SRVR_AUTH_EVT
**                        event, then p_userid is required, otherwise it is optional.
**
**                  p_password  must be less than FTP_SRVR_MAX_AUTH_KEY_SIZE (16 bytes)
**                  p_userid    must be less than OBX_MAX_REALM_LEN (defined in target.h)
**
** Returns          void
**
*******************************************************************************/
FTP_API extern void FTP_SrvrAuthRsp(char *p_password, char *p_userid);

/*******************************************************************************
**
** Function         FTP_SrvrAccessRsp
**
** Description      Sends a reply to an access request event (FTP_SRVR_ACCESS_EVT).
**                  This call MUST be made whenever the event occurs.
**
** Parameters       oper    - operation being accessed.
**                  access  - FTP_ACCESS_ALLOW or FTP_ACCESS_FORBID
**
** Returns          void
**
*******************************************************************************/
FTP_API extern void FTP_SrvrAccessRsp(tFTP_OPER oper, tFTP_ACCESS access);


/**************************
**  Client Functions
***************************/

/*******************************************************************************
**
** Function         FTP_ClntServiceSearch
**
** Description      Searches a remote device for an file transfer service.
**                  The result is returned in the FTP_CLNT_SEARCH_EVT.  If the
**                  service exists, the optional service name is returned with
**                  the event.
**
** Returns          void
**
*******************************************************************************/
FTP_API extern void FTP_ClntServiceSearch(BD_ADDR bd_addr, tFTP_CLNT_CBACK *p_cback);

/*******************************************************************************
**
** Function         FTP_ClntOpen
**
** Description      Open a connection to an FTP server. When the connection is
**                  open the callback function will be called with a
**                  FTP_CLNT_OPEN_EVT.  If the connection fails or otherwise
**                  is closed the callback function will be
**                  called with a FTP_CLNT_CLOSE_EVT.
****
** Returns          void
**
*******************************************************************************/
FTP_API extern void FTP_ClntOpen(BD_ADDR bd_addr, tFTP_CLNT_CBACK *p_cback,
                                 UINT8 sec_mask, UINT8 app_id);

/*******************************************************************************
**
** Function         FTP_ClntClose
**
** Description      Close the current connection to an FTP server. Aborts any
**                  active file transfer.
**
** Returns          void
**
*******************************************************************************/
FTP_API extern void FTP_ClntClose(void);

/*******************************************************************************
**
** Function         FTP_ClntPutFile
**
** Description      Send a file to the FTP server.
**
**                  Note: File name is specified with a fully qualified
**                        path.
**
** Returns          void
**
*******************************************************************************/
FTP_API extern void FTP_ClntPutFile(const char *p_name);

/*******************************************************************************
**
** Function         FTP_ClntGetFile
**
** Description      Retrieve a file from the peer device and copy it to the
**                  local file system.
**
**                  Note: local file name is specified with a fully qualified
**                        path.  Remote file name is specified in UTF-8 format.
**
** Returns          void
**
*******************************************************************************/
FTP_API extern void FTP_ClntGetFile(char *p_local_name, char *p_remote_name);

/*******************************************************************************
**
** Function         FTP_ClntListDir
**
** Description      Retrieve a directory listing from the peer device.
**                  When the operation is complete the callback function will
**                  be called with one or more FTP_CLNT_LIST_EVT events
**                  containing directory list information formatted as described
**                  in the IrOBEX Spec, Version 1.2, section 9.1.2.3.
**                  This function can only be used when the client is connected
**                  to a peer device.
**
** Parameters       p_dir - Name of directory to retrieve listing of.  If NULL,
**                          the current working directory is retrieved.
**
** Returns          void
**
*******************************************************************************/
FTP_API extern void FTP_ClntListDir(char *p_dir);

/*******************************************************************************
**
** Function         FTP_ClntChDir
**
** Description      Change directory on the peer device.  When the operation is
**                  complete the status is returned with the FTP_CLNT_CHDIR_EVT
**                  event. This function can only be used when the client is
**                  connected to a peer device.
**
** Returns          void
**
*******************************************************************************/
FTP_API extern void FTP_ClntChDir(char *p_dir, BOOLEAN backup_flag);

/*******************************************************************************
**
** Function         FTP_ClntMkDir
**
** Description      Create a directory on the peer device. When the operation is
**                  complete the status is returned with the FTP_CLNT_MKDIR_EVT
**                  event. This function can only be used when the client is
**                  connected to a peer device. The current working directory is
**                  set to the new path if successful.
**
** Returns          void
**
*******************************************************************************/
FTP_API extern void FTP_ClntMkDir(char *p_dir);

/*******************************************************************************
**
** Function         FTP_ClntRemove
**
** Description      Remove a file or directory on the peer device.  When the
**                  operation is complete the status is returned with the
**                  FTP_CLNT_REMOVE_EVT event. This function can only be used
**                  when the client is connected to a peer device.
**
** Returns          void
**
*******************************************************************************/
FTP_API extern void FTP_ClntRemove(char *p_name);

/*******************************************************************************
**
** Function         FTP_ClntAuthRsp
**
** Description      Sends a response to an OBEX authentication challenge to the
**                  connected OBEX server. Called in response to an FTP_CLNT_AUTH_EVT
**                  event.
**
**                  Note: If the "userid_required" is TRUE in the FTP_CLNT_AUTH_EVT
**                        event, then p_userid is required, otherwise it is optional.
**
**                  p_password  must be less than GOEP_MAX_AUTH_KEY_SIZE (16 bytes)
**                  p_userid    must be less than OBX_MAX_REALM_LEN (defined in target.h)
**
** Returns          void
**
*******************************************************************************/
FTP_API extern void FTP_ClntAuthRsp(char *p_password, char *p_userid);

/*******************************************************************************
**
** Function         FTP_ClntAbort
**
** Description      Aborts an active Put or Get file operation. It is ignored
**                  otherwise.  Abort status will come back with the Put or Get
**                  complete event.
**
** Returns          void
**
*******************************************************************************/
FTP_API extern void FTP_ClntAbort(void);

#ifdef __cplusplus
}
#endif

#endif /* FTP_API_H */
