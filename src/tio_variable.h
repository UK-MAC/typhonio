/*
 Crown Copyright 2014 AWE.

 This file is part of TyphonIO. 
 Released under the BSD 3-clause license. 
 For more details see license.txt
 */

#ifndef _TIO_VARIABLE_H
#define _TIO_VARIABLE_H

#ifdef __cplusplus
extern "C" {
#endif


/***************************************************************************************************
 *
 * TIO_List_Variables
 *
 ***************************************************************************************************
 *
 * <ITEM>
 *
 * <NAME>
 * <DATA> TIO_List_Variables
 * </NAME>
 *
 * <PURP>
 * <DATA> Returns the number of Variables within a TyphonIO file or object and their names
 * </PURP>
 *
 * <CAPI>
 * <DATA> TIO_t TIO_List_Variables( const TIO_File_t   fileID,
 * <DATA>                           const TIO_Object_t objectID,
 * <DATA>                           TIO_Size_t         *nobj,
 * <DATA>                           char               names[][TIO_STRLEN] );
 * </CAPI>
 *
 * <PARA>
 * <DATA> IN  <DATA> fileID   <DATA> TyphonIO File ID
 * <DATA> IN  <DATA> objectID <DATA> TyphonIO object ID
 * <DATA> OUT <DATA> nobj     <DATA> Number of Variables
 * <DATA> OUT <DATA> names    <DATA> Array of names
 * </PARA>
 *
 * <RETN>
 * <DATA> TyphonIO return code
 * </RETN>
 *
 * <DESC>
 * <DATA> If the <code>objectID</code> is given as <code>TIO_NULL</code>, the Variables
 * <DATA> in the top file level are listed, otherwise it is the those in the given object.
 * <DATA>
 * <DATA> Does not have to be called collectively.
 * </DESC>
 *
 * <CDES>
 * <DATA> <code>names</code> can be <code>NULL</code>
 * </CDES>
 *
 * </ITEM>
 */
extern TIO_t TIO_List_Variables( const TIO_File_t   fileID,
                                 const TIO_Object_t objectID,
                                 TIO_Size_t         *nobj,
                                 char               names[][TIO_STRLEN] );


/***************************************************************************************************
 *
 * TIO_Create_Variable
 *
 ***************************************************************************************************
 *
 * <ITEM>
 *
 * <NAME>
 * <DATA> TIO_Create_Variable
 * </NAME>
 *
 * <PURP>
 * <DATA> Creates a new Variable within a TyphonIO file or object
 * </PURP>
 *
 * <CAPI>
 * <DATA> TIO_t TIO_Create_Variable( const TIO_File_t   fileID,
 * <DATA>                            const TIO_Object_t objectID,
 * <DATA>                            const char         name[],
 * <DATA>                            TIO_Object_t       *varID,
 * <DATA>                            const TIO_Data_t   datatype,
 * <DATA>                            const TIO_Dims_t   ndims,
 * <DATA>                            const TIO_Size_t   dims[],
 * <DATA>                            const char         units[] );
 * </CAPI>
 *
 * <PARA>
 * <DATA> IN  <DATA> fileID   <DATA> TyphonIO File ID
 * <DATA> IN  <DATA> objectID <DATA> TyphonIO object ID
 * <DATA> IN  <DATA> name     <DATA> Name for Variable
 * <DATA> OUT <DATA> varID    <DATA> ID of new Variable
 * <DATA> IN  <DATA> datatype <DATA> TyphonIO datatype of Variable
 * <DATA> IN  <DATA> ndims    <DATA> Number of dimensions of Variable
 * <DATA> IN  <DATA> dims     <DATA> Array containing dimensions of Variable
 * <DATA> IN  <DATA> units    <DATA> String containing units of Variable
 * </PARA>
 *
 * <RETN>
 * <DATA> TyphonIO return code
 * </RETN>
 *
 * <DESC>
 * <DATA> Creates a new variable within the given TyphonIO object, or if <code>objectID</code> is
 * <DATA> set to <code>TIO_NULL</code>, the Variable is created in the top file level.
 * <DATA>
 * <DATA> To create a scalar variable, <code>ndims</code> should be set to <code>TIO_0D</code>
 * <DATA> and the <code>dims</code> argument is then ignored.  For non-scalar variables, any
 * <DATA> number of dimensions can be specified up to the <code>TIO_MAXDIMS</code> limit.
 * <DATA>
 * <DATA> The <code>dims</code> array argument must contains at least <code>ndims</code> elements
 * <DATA> and the first <code>ndims</code> elements contain the size of each dimension in the
 * <DATA> order
 * <DATA>
 * <DATA>
 * <DATA> Must be called collectively.
 * </DESC>
 *
 * <CDES>
 * <DATA> For example, to create a Variable for an array declared as:
 * <DATA> <blockquote><code>int myarray[5][3]</code></blockquote>
 * <DATA> <code>ndims = TIO_2D</code> and <code>dims[2] = { 3, 5 }</code>
 * </CDES>
 *
 * </ITEM>
 */
extern TIO_t TIO_Create_Variable( const TIO_File_t   fileID,
                                  const TIO_Object_t objectID,
                                  const char         name[],
                                  TIO_Object_t       *varID,
                                  const TIO_Data_t   datatype,
                                  const TIO_Dims_t   ndims,
                                  const TIO_Size_t   dims[],
                                  const char         units[] );


/***************************************************************************************************
 *
 * TIO_Open_Variable
 *
 ***************************************************************************************************
 *
 * <ITEM>
 *
 * <NAME>
 * <DATA> TIO_Open_Variable
 * </NAME>
 *
 * <PURP>
 * <DATA> Opens a Variable from within a TyphonIO file or object by name
 * </PURP>
 *
 * <CAPI>
 * <DATA> TIO_t TIO_Open_Variable( const TIO_File_t   fileID,
 * <DATA>                          const TIO_Object_t objectID,
 * <DATA>                          const char         name[],
 * <DATA>                          TIO_Object_t       *varID,
 * <DATA>                          TIO_Data_t         *datatype,
 * <DATA>                          TIO_Dims_t         *ndims,
 * <DATA>                          TIO_Size_t         dims[],
 * <DATA>                          char               units[] );
 * </CAPI>
 *
 * <PARA>
 * <DATA> IN  <DATA> fileID   <DATA> TyphonIO File ID
 * <DATA> IN  <DATA> objectID <DATA> TyphonIO object ID
 * <DATA> IN  <DATA> name     <DATA> Name of Variable
 * <DATA> OUT <DATA> varID    <DATA> ID of opened Variable
 * <DATA> OUT <DATA> datatype <DATA> TyphonIO datatype of Variable
 * <DATA> OUT <DATA> ndims    <DATA> Number of dimension of Variable
 * <DATA> OUT <DATA> dims     <DATA> Array containing dimensions of Variable
 * <DATA> OUT <DATA> units    <DATA> String containing units of Variable
 * </PARA>
 *
 * <RETN>
 * <DATA> TyphonIO return code
 * </RETN>
 *
 * <DESC>
 * <DATA> If the <code>objectID</code> is given as <code>TIO_NULL</code>, the Variable is
 * <DATA> opened from the top file level, otherwise it is opened from the specified object.
 * <DATA>
 * <DATA> The <code>dims</code> and <code>units</code> arguments should be allocated large enough
 * <DATA> to hold the returned values for the variable.  If unknown, the maximum possible sizes
 * <DATA> can be used for allocation, which are <code>TIO_MAXDIMS</code> and <code>TIO_STRLEN</code>
 * <DATA> respectively.
 * <DATA>
 * <DATA> Must be called collectively.
 * </DESC>
 *
 * </ITEM>
 */
extern TIO_t TIO_Open_Variable( const TIO_File_t   fileID,
                                const TIO_Object_t objectID,
                                const char         name[],
                                TIO_Object_t       *varID,
                                TIO_Data_t         *datatype,
                                TIO_Dims_t         *ndims,
                                TIO_Size_t         dims[],
                                char               units[] );


/***************************************************************************************************
 *
 * TIO_Open_Variable_idx
 *
 ***************************************************************************************************
 *
 * <ITEM>
 *
 * <NAME>
 * <DATA> TIO_Open_Variable_idx
 * </NAME>
 *
 * <PURP>
 * <DATA> Opens a Variable from within a TyphonIO file or object by index
 * </PURP>
 *
 * <CAPI>
 * <DATA> TIO_t TIO_Open_Variable_idx( const TIO_File_t   fileID,
 * <DATA>                              const TIO_Object_t objectID,
 * <DATA>                              const TIO_Size_t   idx,
 * <DATA>                              char               name,
 * <DATA>                              TIO_Object_t       *varID,
 * <DATA>                              TIO_Data_t         *datatype,
 * <DATA>                              TIO_Dims_t         *ndims,
 * <DATA>                              TIO_Size_t         dims[],
 * <DATA>                              char               units[] );
 * </CAPI>
 *
 * <PARA>
 * <DATA> IN  <DATA> fileID   <DATA> TyphonIO File ID
 * <DATA> IN  <DATA> objectID <DATA> TyphonIO object ID
 * <DATA> IN  <DATA> idx      <DATA> Index of Variable
 * <DATA> OUT <DATA> name     <DATA> Name of Variable
 * <DATA> OUT <DATA> varID    <DATA> ID of opened Variable
 * <DATA> OUT <DATA> datatype <DATA> TyphonIO datatype of Variable
 * <DATA> OUT <DATA> ndims    <DATA> Number of dimension of Variable
 * <DATA> OUT <DATA> dims     <DATA> Array containing dimensions of Variable
 * <DATA> OUT <DATA> units    <DATA> String containing units of Variable
 * </PARA>
 *
 * <RETN>
 * <DATA> TyphonIO return code
 * </RETN>
 *
 * <DESC>
 * <DATA> If the <code>objectID</code> is given as <code>TIO_NULL</code>, the Variable is
 * <DATA> opened from the top file level, otherwise it is opened from the specified object.
 * <DATA>
 * <DATA> The <code>dims</code> and <code>units</code> arguments should be allocated large enough
 * <DATA> to hold the returned values for the variable.  If unknown, the maximum possible sizes
 * <DATA> can be used for allocation, which are <code>TIO_MAXDIMS</code> and <code>TIO_STRLEN</code>
 * <DATA> respectively.
 * <DATA>
 * <DATA> Must be called collectively.
 * </DESC>
 *
 * </ITEM>
 */
extern TIO_t TIO_Open_Variable_idx( const TIO_File_t   fileID,
                                    const TIO_Object_t objectID,
                                    const TIO_Size_t   idx,
                                    char               name[],
                                    TIO_Object_t       *varID,
                                    TIO_Data_t         *datatype,
                                    TIO_Dims_t         *ndims,
                                    TIO_Size_t         dims[],
                                    char               units[] );


/***************************************************************************************************
 *
 * TIO_Get_Variable_Info
 *
 ***************************************************************************************************
 *
 * <ITEM>
 *
 * <NAME>
 * <DATA> TIO_Get_Variable_Info
 * </NAME>
 *
 * <PURP>
 * <DATA> Gets the info of an open Variable
 * </PURP>
 *
 * <CAPI>
 * <DATA> TIO_t TIO_Get_Variable_Info( const TIO_File_t   fileID,
 * <DATA>                              const TIO_Object_t varID,
 * <DATA>                              char               name[],
 * <DATA>                              TIO_Data_t         *datatype,
 * <DATA>                              TIO_Dims_t         *ndims,
 * <DATA>                              TIO_Size_t         dims[],
 * <DATA>                              char               units[] );
 * </CAPI>
 *
 * <PARA>
 * <DATA> IN  <DATA> fileID   <DATA> TyphonIO File ID
 * <DATA> IN  <DATA> objectID <DATA> TyphonIO Variable ID
 * <DATA> OUT <DATA> name     <DATA> Name of Variable
 * <DATA> OUT <DATA> datatype <DATA> TyphonIO datatype of Variable
 * <DATA> OUT <DATA> ndims    <DATA> Number of dimension of Variable
 * <DATA> OUT <DATA> dims     <DATA> Array containing dimensions of Variable
 * <DATA> OUT <DATA> units    <DATA> String containing units of Variable
 * </PARA>
 *
 * <RETN>
 * <DATA> TyphonIO return code
 * </RETN>
 *
 * <DESC>
 * <DATA> Returns all the metadata associated with an open Variable.
 * <DATA>
 * <DATA> The <code>dims</code> and <code>units</code> arguments should be allocated large enough
 * <DATA> to hold the returned values for the variable.  If unknown, the maximum possible sizes
 * <DATA> can be used for allocation, which are <code>TIO_MAXDIMS</code> and <code>TIO_STRLEN</code>
 * <DATA> respectively.
 * <DATA>
 * <DATA> Does not have to be called collectively.
 * </DESC>
 *
 * </ITEM>
 */
extern TIO_t TIO_Get_Variable_Info( const TIO_File_t   fileID,
                                    const TIO_Object_t varID,
                                    char               name[],
                                    TIO_Data_t         *datatype,
                                    TIO_Dims_t         *ndims,
                                    TIO_Size_t         dims[],
                                    char               units[] );


/***************************************************************************************************
 *
 * TIO_Close_Variable
 *
 ***************************************************************************************************
 *
 * <ITEM>
 *
 * <NAME>
 * <DATA> TIO_Close_Variable
 * </NAME>
 *
 * <PURP>
 * <DATA> Closes an open Variable
 * </PURP>
 *
 * <CAPI>
 * <DATA> TIO_t TIO_Close_Variable( const TIO_File_t   fileID,
 * <DATA>                           const TIO_Object_t varID );
 * </CAPI>
 *r
 * <PARA>
 * <DATA> IN <DATA> fileID <DATA> TyphonIO File ID
 * <DATA> IN <DATA> varID  <DATA> TyphonIO Variable ID
 * </PARA>
 *
 * <RETN>
 * <DATA> TyphonIO return code
 * </RETN>
 *
 * <DESC>
 * <DATA> Closes an open Variable.
 * <DATA>
 * <DATA> Must be called collectively.
 * </DESC>
 *
 * </ITEM>
 */
extern TIO_t TIO_Close_Variable( const TIO_File_t   fileID,
                                 const TIO_Object_t varID );


/***************************************************************************************************
 *
 * TIO_Write_Variable
 *
 ***************************************************************************************************
 *
 * <ITEM>
 *
 * <NAME>
 * <DATA> TIO_Write_Variable
 * </NAME>
 *
 * <PURP>
 * <DATA> Write data to an open Variable
 * </PURP>
 *
 * <CAPI>
 * <DATA> TIO_t TIO_Write_Variable( const TIO_File_t   fileID,
 * <DATA>                           const TIO_Object_t varID,
 * <DATA>                           const TIO_Data_t   datatype,
 * <DATA>                           const void         *vdata );
 * </CAPI>
 *
 * <PARA>
 * <DATA> IN <DATA> fileID   <DATA> TyphonIO File ID
 * <DATA> IN <DATA> varID    <DATA> TyphonIO Variable ID
 * <DATA> IN <DATA> datatype <DATA> TyphonIO datatype of data in memory
 * <DATA> IN <DATA> vdata    <DATA> Buffer containing Variable data
 * </PARA>
 *
 * <RETN>
 * <DATA> TyphonIO return code
 * </RETN>
 *
 * <DESC>
 * <DATA> Writes data from memory into a created / open Variable.
 * <DATA>
 * <DATA> If <code>datatype</code> is different to that given at Variable creation, TyphonIO
 * <DATA> will convert the given <code>vdata</code> to the original datatype whilst writing it
 * <DATA> to the file.
 * <DATA>
 * <DATA> Does not have to be called collectively.<br>
 * <DATA> In parallel, a race condition may occur if all processes write to a Variable, so
 * <DATA> it is advised that only a single process performs the write.
 * </DESC>
 *
 * </ITEM>
 */
extern TIO_t TIO_Write_Variable( const TIO_File_t   fileID,
                                 const TIO_Object_t varID,
                                 const TIO_Data_t   datatype,
                                 const void         *vdata );


/***************************************************************************************************
 *
 * TIO_Read_Variable
 *
 ***************************************************************************************************
 *
 * <ITEM>
 *
 * <NAME>
 * <DATA> TIO_Read_Variable
 * </NAME>
 *
 * <PURP>
 * <DATA> Read data from an open Variable
 * </PURP>
 *
 * <CAPI>
 * <DATA> TIO_t TIO_Read_Variable( const TIO_File_t   fileID,
 * <DATA>                          const TIO_Object_t varID,
 * <DATA>                          const TIO_Data_t   datatype,
 * <DATA>                          void               *vdata );
 * </CAPI>
 *
 * <PARA>
 * <DATA> IN  <DATA> fileID   <DATA> TyphonIO File ID
 * <DATA> IN  <DATA> varID    <DATA> TyphonIO Variable ID
 * <DATA> IN  <DATA> datatype <DATA> TyphonIO datatype of data in memory
 * <DATA> OUT <DATA> vdata    <DATA> Buffer containing Variable data
 * </PARA>
 *
 * <RETN>
 * <DATA> TyphonIO return code
 * </RETN>
 *
 * <DESC>
 * <DATA> Reads all the data from an open Variable into memory.
 * <DATA>
 * <DATA> If <code>datatype</code> is different to the one that the Variable is stored as in the
 * <DATA> file, TyphonIO will convert it to the one given during reading.
 * <DATA>
 * <DATA> <code>vdata</code> must be allocated large enough to hold the read data.
 * <DATA>
 * <DATA> Does not have to be called collectively.<br>
 * <DATA> In parallel, the data is read by all processes that perform the read.
 * </DESC>
 *
 * </ITEM>
 */
extern TIO_t TIO_Read_Variable( const TIO_File_t   fileID,
                                const TIO_Object_t varID,
                                const TIO_Data_t   datatype,
                                void               *vdata );


/***************************************************************************************************
 *
 * TIO_Read_Variable_Section
 *
 ***************************************************************************************************
 *
 * <ITEM>
 *
 * <NAME>
 * <DATA> TIO_Read_Variable_Section
 * </NAME>
 *
 * <PURP>
 * <DATA> Read a section of data from an open Variable
 * </PURP>
 *
 * <CAPI>
 * <DATA> TIO_t TIO_Read_Variable_Section( const TIO_File_t   fileID,
 * <DATA>                                  const TIO_Object_t varID,
 * <DATA>                                  const TIO_Data_t   datatype,
 * <DATA>                                  const TIO_Dims_t   ndims,
 * <DATA>                                  const TIO_Size_t   lolimit[],
 * <DATA>                                  const TIO_Size_t   hilimit[],
 * <DATA>                                  void               *vdata );
 * </CAPI>
 *
 * <PARA>
 * <DATA> IN  <DATA> fileID   <DATA> TyphonIO File ID
 * <DATA> IN  <DATA> varID    <DATA> TyphonIO Variable ID
 * <DATA> IN  <DATA> datatype <DATA> TyphonIO datatype of data in memory
 * <DATA> IN  <DATA> ndims    <DATA> Number of dimensions in section arrays
 * <DATA> IN  <DATA> lolimit  <DATA> Array containing lower extents of the section
 * <DATA> IN  <DATA> hilimit  <DATA> Array containing upper extents of the section
 * <DATA> OUT <DATA> vdata    <DATA> Buffer containing Variable data
 * </PARA>
 *
 * <RETN>
 * <DATA> TyphonIO return code
 * </RETN>
 *
 * <DESC>
 * <DATA> Reads a section of data from an open array (ie. not a scalar) Variable into memory.
 * <DATA>
 * <DATA> <code>ndims</code> must match the dimension of the open Variable and the
 * <DATA> <code>lolimit</code> and <code>hilimit</code> arrays should be at least this size.
 * <DATA>
 * <DATA> The <code>lolimit</code> and <code>hlimit</code> arrays define a hyperslab of the
 * <DATA> Variable data that is to be read.
 * <DATA>
 * <DATA>
 * <DATA> If <code>datatype</code> is different to the one that the Variable is stored as in the
 * <DATA> file, TyphonIO will convert it to the one given during reading.
 * <DATA>
 * <DATA> <code>vdata</code> must be allocated large enough to hold the read data.
 * <DATA>
 * <DATA> Does not have to be called collectively.<br>
 * <DATA> In parallel, the data is read by all processes that perform the read.
 * </DESC>
 * <CDES>
 * <DATA> For example, to read elements <code><i>M</i></code> to <code><i>N</i></code> for a 1D
 * <DATA> Variable, <code>lolimit = { M }</code> and <code>hilimit = { N }</code>.  Elements are
 * <DATA> indexed from 0-origin.
 * </CDES>
 *
 * </ITEM>
 */
extern TIO_t TIO_Read_Variable_Section( const TIO_File_t   fileID,
                                        const TIO_Object_t varID,
                                        const TIO_Data_t   datatype,
                                        const TIO_Dims_t   ndims,
                                        const TIO_Size_t   lolimit[],
                                        const TIO_Size_t   hilimit[],
                                        void               *vdata );


#ifdef __cplusplus
}
#endif

#endif

/*
 * EOF
 */
