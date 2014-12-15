/*
 Crown Copyright 2014 AWE.

 This file is part of TyphonIO. 
 Released under the BSD 3-clause license. 
 For more details see license.txt
 */

#ifndef _TIO_ACCESS_H
#define _TIO_ACCESS_H

#ifdef __cplusplus
extern "C" {
#endif


/***************************************************************************************************
 *
 * TIO_CheckFile
 *
 ***************************************************************************************************
 *
 * <ITEM>
 *
 * <NAME>
 * <DATA> TIO_CheckFile
 * </NAME>
 *
 * <PURP>
 * <DATA> Checks whether a file is a TyphonIO v1.x file
 * </PURP>
 *
 * <CAPI>
 * <DATA> TIO_t TIO_CheckFile( const char filename );
 * </CAPI>
 *
 * <PARA>
 * <DATA> IN <DATA> filename <DATA> Relative or absolute path of file
 * </PARA>
 *
 * <RETN>
 * <DATA> TyphonIO return code
 * </RETN>
 *
 * <DESC>
 * <DATA> If the file is a TyphonIO v1.x file this function will return <code>TIO_SUCCESS</code>.<br>
 * <DATA> <code>TIO_ERR_VERSION_MISMATCH</code> is returned if the file is an earlier pre-v1.0
 * <DATA> TyphonIO file.  In all other cases the function will return failure.
 * <DATA>
 * <DATA> Does not have to be called collectively.
 * </DESC>
 *
 * </ITEM>
 */
extern TIO_t TIO_CheckFile( const char filename[] );


/***************************************************************************************************
 *
 * TIO_CheckObject
 *
 ***************************************************************************************************
 *
 * <ITEM>
 *
 * <NAME>
 * <DATA> TIO_CheckObject
 * </NAME>
 *
 * <PURP>
 * <DATA> Checks whether an object exists
 * </PURP>
 *
 * <CAPI>
 * <DATA> TIO_t TIO_CheckObject( const TIO_File_t   fileID,
 * <DATA>                        const TIO_Object_t parentID,
 * <DATA>                        const char         name[] );
 * </CAPI>
 *
 * <PARA>
 * <DATA> IN <DATA> fileID   <DATA> TyphonIO file ID
 * <DATA> IN <DATA> parentID <DATA> TyphonIO object ID
 * <DATA> IN <DATA> name     <DATA> Name of
 * </PARA>
 *
 * <RETN>
 * <DATA> TyphonIO return code
 * </RETN>
 *
 * <DESC>
 * <DATA> If the object exists in the TyphonIO file within the given parent, this function will
 * <DATA> return <code>TIO_SUCCESS</code>.  If the object does not exist, it will return
 * <DATA> <code>TIO_ERR_OBJECT_NOT_EXIST</code>.<br>br>
 * <DATA> Other errors will return other appropriate values.
 * <DATA>
 * <DATA> Does not have to be called collectively.
 * </DESC>
 *
 * </ITEM>
 */
extern TIO_t TIO_CheckObject( const TIO_File_t   fileID,
                              const TIO_Object_t parentID,
                              const char         name[] );


/***************************************************************************************************
 *
 * TIO_Get_LibVersion
 *
 ***************************************************************************************************
 *
 * <ITEM>
 *
 * <NAME>
 * <DATA> TIO_Get_LibVersion
 * </NAME>
 *
 * <PURP>
 * <DATA> Returns the version information of the TyphonIO library in use
 * </PURP>
 *
 * <CAPI>
 * <DATA> TIO_t TIO_Get_LibVersion( int *maj, int *min, int *rel );
 * </CAPI>
 *
 * <PARA>
 * <DATA> OUT <DATA> maj <DATA> Major version number
 * <DATA> OUT <DATA> min <DATA> Minor version number
 * <DATA> OUT <DATA> rel <DATA> Release version number
 * </PARA>
 *
 * <RETN>
 * <DATA> TyphonIO return code
 * </RETN>
 *
 * <DESC>
 * <DATA> TyphonIO version numbers have the <i>maj.min.rel</i> format.
 * <DATA>
 * <DATA> Does not have to be called collectively.
 * </DESC>
 *
 * </ITEM>
 */
extern TIO_t TIO_Get_LibVersion( int *maj, int *min, int *rel );


/***************************************************************************************************
 *
 * TIO_Get_FileVersion
 *
 ***************************************************************************************************
 *
 * <ITEM>
 *
 * <NAME>
 * <DATA> TIO_Get_FileVersion
 * </NAME>
 *
 * <PURP>
 * <DATA> Returns the version information of the TyphonIO library used to write a file
 * </PURP>
 *
 * <CAPI>
 * <DATA> TIO_t TIO_Get_FileVersion( const char filename[], int *maj, int *min, int *rel );
 * </CAPI>
 *
 * <PARA>
 * <DATA> IN  <DATA> filename <DATA> Relative or absolute path of file
 * <DATA> OUT <DATA> maj <DATA> Major version number
 * <DATA> OUT <DATA> min <DATA> Minor version number
 * <DATA> OUT <DATA> rel <DATA> Release version number
 * </PARA>
 *
 * <RETN>
 * <DATA> TyphonIO return code
 * </RETN>
 *
 * <DESC>
 * <DATA> TyphonIO version numbers have the <i>maj.min.rel</i> format.
 * <DATA>
 * <DATA> Does not have to be called collectively.
 * </DESC>
 *
 * </ITEM>
 */
extern TIO_t TIO_Get_FileVersion( const char filename[], int *maj, int *min, int *rel );


/***************************************************************************************************
 *
 * TIO_IsFileOpen
 *
 ***************************************************************************************************
 *
 * <ITEM>
 *
 * <NAME>
 * <DATA> TIO_IsFileOpen
 * </NAME>
 *
 * <PURP>
 * <DATA> Returns whether a given file is already open
 * </PURP>
 *
 * <CAPI>
 * <DATA> TIO_t TIO_IsFileOpen( const char filename[] );
 * </CAPI>
 *
 * <PARA>
 * <DATA> IN <DATA> filename <DATA> Relative or absolute path of file
 * </PARA>
 *
 * <RETN>
 * <DATA> TyphonIO return code
 * </RETN>
 *
 * <DESC>
 * <DATA>
 * </DESC>
 * <DESC>
 * <DATA> Returns <code>TIO_SUCCESS</code> if the file is already open and <code>TIO_FAIL</code>
 * <DATA> if it isn't.
 * <DATA>
 * <DATA> Does not have to be called collectively.
 * </DESC>
 *
 * </ITEM>
 */
extern TIO_t TIO_IsFileOpen( const char filename[] );


/***************************************************************************************************
 *
 * TIO_IsObjectOpen
 *
 ***************************************************************************************************
 *
 * <ITEM>
 *
 * <NAME>
 * <DATA> TIO_IsObjectOpen
 * </NAME>
 *
 * <PURP>
 * <DATA> Returns whether a given object is already open
 * </PURP>
 *
 * <CAPI>
 * <DATA> TIO_t TIO_IsObjectOpen( const TIO_File_t fileID,
 * <DATA>                         const TIO_Object_t parentID,
 * <DATA>                         const char name[] );
 * </CAPI>
 *
 * <PARA>
 * <DATA> IN <DATA> fileID   <DATA> TyphonIO file ID
 * <DATA> IN <DATA> parentID <DATA> TyphonIO object ID
 * <DATA> IN <DATA> name     <DATA> Object name
 * </PARA>
 *
 * <RETN>
 * <DATA> TyphonIO return code
 * </RETN>
 *
 * <DESC>
 * <DATA>
 * </DESC>
 * <DESC>
 * <DATA> Returns <code>TIO_SUCCESS</code> if the object has already been opened within the
 * <DATA> given file and parent object, and <code>TIO_FAIL</code> if it hasn't.
 * <DATA>
 * <DATA> If the <code>parentID</code> is given as <code>TIO_NULL</code>, the parent
 * <DATA> is taken as the top file level, otherwise it is the given object.
 * <DATA>
 * <DATA> Does not have to be called collectively.
 * </DESC>
 *
 * </ITEM>
 */
extern TIO_t TIO_IsObjectOpen( const TIO_File_t   fileID,
                               const TIO_Object_t parentID,
                               const char         name[] );


/***************************************************************************************************
 *
 * TIO_Create
 *
 ***************************************************************************************************
 *
 * <ITEM>
 *
 * <NAME>
 * <DATA> TIO_Create
 * </NAME>
 *
 * <PURP>
 * <DATA> Creates a new TyphonIO file
 * </PURP>
 *
 * <CAPI>
 * <DATA> TIO_t TIO_Create( const char         filename[],
 * <DATA>                   TIO_File_t         *fileID,
 * <DATA>                   const TIO_Access_t access,
 * <DATA>                   const char         codename[],
 * <DATA>                   const char         version[],
 * <DATA>                   const char         date[],
 * <DATA>                   const char         title[],
 * <DATA>                   const MPI_Comm     comm,
 * <DATA>                   const MPI_Info     info,
 * <DATA>                   const int          rank );
 * </CAPI>
 *
 * <PARA>
 * <DATA> IN  <DATA> filename <DATA> Relative or absolute path of file
 * <DATA> OUT <DATA> fileID   <DATA> TyphonIO fileID
 * <DATA> IN  <DATA> access   <DATA> Access mode parameter
 * <DATA> IN  <DATA> codename <DATA> Name string of the application
 * <DATA> IN  <DATA> version  <DATA> Version string of the application
 * <DATA> IN  <DATA> date     <DATA> Date string
 * <DATA> IN  <DATA> title    <DATA> Title for the dump in the file
 * <DATA> IN  <DATA> comm     <DATA> MPI communicator handle
 * <DATA> IN  <DATA> info     <DATA> MPI info handle
 * <DATA> IN  <DATA> rank     <DATA> MPI rank of calling process within comm
 * </PARA>
 *
 * <RETN>
 * <DATA> TyphonIO return code
 * </RETN>
 *
 * <DESC>
 * <DATA> Creates a new TyphonIO file or replaces an existing one as specfied with the
 * <DATA> <code>access</code> parameter:
 * <DATA> <blockquote>
 * <DATA> <code>access = TIO_ACC_CREATE </code> - if a file already exists, create will fail<br>
 * <DATA> <code>access = TIO_ACC_REPLACE</code> - any existing file is truncated
 * <DATA> </blockquote>
 * <DATA> If not using parallel TyphonIO <code>comm</code>, <code>info</code> and <code>rank</code>
 * <DATA> can be given null values, with <code>TIO_NULL</code> or the MPI built-in null values
 * <DATA> <code>MPI_COMM_NULL</code>, <code>MPI_INFO_NULL</code> and <code>MPI_PROC_NULL</code>
 * <DATA> respectively.  These null values are provided by TyphonIO if MPI hasn't been included.
 * <DATA>
 * <DATA> For parallel TyphonIO, the <code>comm</code>, <code>info</code> and <code>rank</code>
 * <DATA> must be valid MPI handles for the communicator, info object and process rank.  The
 * <DATA> communicator is not duplicated by TyphonIO, so the user may wish to do this before
 * <DATA> calling.  The <code>info</code> object can be <code>MPI_INFO_NULL</code>, but the user
 * <DATA> may wish to create an info object that contains MPI-IO hints to improve performance.
 * <DATA> The <code>rank</code> must be the value given by <code>MPI_Comm_rank</code>
 * <DATA> for the process within the given communicator.
 * <DATA>
 * <DATA> Must be called collectively.
 * </DESC>
 *
 * </ITEM>
 */
extern TIO_t TIO_Create( const char         filename[],
                         TIO_File_t         *fileID,
                         const TIO_Access_t access,
                         const char         codename[],
                         const char         version[],
                         const char         date[],
                         const char         title[],
                         const MPI_Comm     comm,
                         const MPI_Info     info,
                         const int          rank );


/***************************************************************************************************
 *
 * TIO_Open
 *
 ***************************************************************************************************
 *
 * <ITEM>
 *
 * <NAME>
 * <DATA> TIO_Open
 * </NAME>
 *
 * <PURP>
 * <DATA> Opens an existing TyphonIO file
 * </PURP>
 *
 * <CAPI>
 * <DATA> TIO_t TIO_Open( const char         filename[],
 * <DATA>                 TIO_File_t         *fileID,
 * <DATA>                 const TIO_Access_t access,
 * <DATA>                 char               codename[],
 * <DATA>                 char               version[],
 * <DATA>                 char               date[],
 * <DATA>                 char               title[],
 * <DATA>                 const MPI_Comm     comm,
 * <DATA>                 const MPI_Info     info,
 * <DATA>                 const int          rank );
 * </CAPI>
 *
 * <PARA>
 * <DATA> IN    <DATA> filename <DATA> Relative or absolute path of file
 * <DATA> OUT   <DATA> fileID   <DATA> TyphonIO fileID
 * <DATA> IN    <DATA> access   <DATA> Access mode parameter
 * <DATA> INOUT <DATA> codename <DATA> Name string of the application
 * <DATA> INOUT <DATA> version  <DATA> Version string of the application
 * <DATA> INOUT <DATA> date     <DATA> Date string
 * <DATA> INOUT <DATA> title    <DATA> Title for the dump in the file
 * <DATA> IN    <DATA> comm     <DATA> MPI communicator handle
 * <DATA> IN    <DATA> info     <DATA> MPI info handle
 * <DATA> IN    <DATA> rank     <DATA> MPI rank of calling process within comm
 * </PARA>
 *
 * <RETN>
 * <DATA> TyphonIO return code
 * </RETN>
 *
 * <DESC>
 * <DATA> Opens an exisiting TyphonIO file for reading and/or appending.
 * <DATA> <blockquote>
 * <DATA> <code>access = TIO_ACC_READWRITE</code> - open for read and write access<br>
 * <DATA> <code>access = TIO_ACC_READONLY </code> - opens for read access only<br>
 * <DATA> <code>access = TIO_ACC_READWRITESTRICT </code> - opens for read and write without 
 * <DATA> overwriting the initial info.
 * <DATA> </blockquote>
 * <DATA> The <code>codename</code>, <code>version</code>, <code>date</code> and <code>title</code>
 * <DATA> values are used to update the values in the file in <code>READWRITE</code> mode, but are
 * <DATA> just read in <code>READONLY</code>.
 * <DATA>
 * <DATA> If not using parallel TyphonIO <code>comm</code>, <code>info</code> and <code>rank</code>
 * <DATA> can be given null values, with <code>TIO_NULL</code> or the MPI built-in null values
 * <DATA> <code>MPI_COMM_NULL</code>, <code>MPI_INFO_NULL</code> and <code>MPI_PROC_NULL</code>
 * <DATA> respectively.  These null values are provided by TyphonIO if MPI hasn't been included.
 * <DATA>
 * <DATA> For parallel TyphonIO, the <code>comm</code>, <code>info</code> and <code>rank</code>
 * <DATA> must be valid MPI handles for the communicator, info object and process rank.  The
 * <DATA> communicator is not duplicated by TyphonIO, so the user may wish to do this before
 * <DATA> calling.  The <code>info</code> object can be <code>MPI_INFO_NULL</code>, but the user
 * <DATA> may wish to create an info object that contains MPI-IO hints to improve performance.
 * <DATA> The <code>rank</code> must be the value given by <code>MPI_Comm_rank</code>
 * <DATA> for the process within the given communicator.
 * <DATA>
 * <DATA> Must be called collectively.
 * </DESC>
 *
 * </ITEM>
 */
extern TIO_t TIO_Open( const char         filename[],
                       TIO_File_t         *fileID,
                       const TIO_Access_t access,
                       char               codename[],
                       char               version[],
                       char               date[],
                       char               title[],
                       const MPI_Comm     comm,
                       const MPI_Info     info,
                       const int          rank );

/***************************************************************************************************
 *
 * TIO_Get_File_Info
 *
 ***************************************************************************************************
 *
 * <ITEM>
 *
 * <NAME>
 * <DATA> TIO_Get_File_Info
 * </NAME>
 *
 * <PURP>
 * <DATA> Gets the info for an open file
 * </PURP>
 *
 * <CAPI>
 * <DATA> TIO_t TIO_Get_File_Info( const TIO_File_t fileID,
 * <DATA>                          char             filename[],
 * <DATA>                          char             codename[],
 * <DATA>                          char             version[],
 * <DATA>                          char             date[],
 * <DATA>                          char             title[] );
 * </CAPI>
 *
 * <PARA>
 * <DATA> IN  <DATA> fileID   <DATA> TyphonIO fileID
 * <DATA> OUT <DATA> filename <DATA> Relative or absolute path of file
 * <DATA> OUT <DATA> codename <DATA> Name string of the application
 * <DATA> OUT <DATA> version  <DATA> Version string of the application
 * <DATA> OUT <DATA> date     <DATA> Date string
 * <DATA> OUT <DATA> title    <DATA> Title for the dump in the file
 * </PARA>
 *
 * <RETN>
 * <DATA> TyphonIO return code
 * </RETN>
 *
 * <DESC>
 * <DATA> Returns all the metadata attributes for a file as written at file creation / modification.
 * <DATA>
 * <DATA> Does not have to be called collectively.
 * </DESC>
 *
 * </ITEM>
 */
extern TIO_t TIO_Get_File_Info( const TIO_File_t fileID,
                                char             filename[],
                                char             codename[],
                                char             version[],
                                char             date[],
                                char             title[] );

/***************************************************************************************************
 *
 * TIO_Close
 *
 ***************************************************************************************************
 *
 * <ITEM>
 *
 * <NAME>
 * <DATA> TIO_Close
 * </NAME>
 *
 * <PURP>
 * <DATA> Closes an open TyphonIO file
 * </PURP>
 *
 * <CAPI>
 * <DATA> TIO_t TIO_Close( const TIO_File_t fileID );
 * </CAPI>
 *
 * <PARA>
 * <DATA> IN <DATA> fileID <DATA> TyphonIO fileID
 * </PARA>
 *
 * <RETN>
 * <DATA> TyphonIO return code
 * </RETN>
 *
 * <DESC>
 * <DATA> Closes and finalizes an open file.  All child objects within the file should be closed
 * <DATA> before calling this routine.
 * <DATA>
 * <DATA> Must be called collectively.
 * </DESC>
 *
 * </ITEM>
 */
extern TIO_t TIO_Close( const TIO_File_t fileID );


/***************************************************************************************************
 *
 * TIO_Flush
 *
 ***************************************************************************************************
 *
 * <ITEM>
 *
 * <NAME>
 * <DATA> TIO_Flush
 * </NAME>
 *
 * <PURP>
 * <DATA> Flushes a TyphonIO file
 * </PURP>
 *
 * <CAPI>
 * <DATA> TIO_t TIO_Flush( const TIO_File_t fileID );
 * </CAPI>
 *
 * <PARA>
 * <DATA> IN <DATA> fileID   <DATA> TyphonIO fileID
 * </PARA>
 *
 * <RETN>
 * <DATA> TyphonIO return code
 * </RETN>
 *
 * <DESC>
 * <DATA> Flushes all data cached by TyphonIO.  This is effectively the same as closing the file
 * <DATA> but leaving the file and all open objects open.
 * <DATA>
 * <DATA> It is advisable to flush a writeable file if it is being held open for any long length
 * <DATA> of time to prevent accidental data loss, say, if the application terminates unexpectedly.
 * <DATA>
 * <DATA> Must be called collectively.
 * </DESC>
 *
 * </ITEM>
 */
extern TIO_t TIO_Flush( const TIO_File_t fileID );



#ifdef __cplusplus
}
#endif

#endif

/*
 * EOF
 */
