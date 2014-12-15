/*
 Crown Copyright 2014 AWE.

 This file is part of TyphonIO. 
 Released under the BSD 3-clause license. 
 For more details see license.txt
 */

#ifndef _TIO_STATE_H
#define _TIO_STATE_H

#ifdef __cplusplus
extern "C" {
#endif


/***************************************************************************************************
 *
 * TIO_List_States
 *
 ***************************************************************************************************
 *
 * <ITEM>
 *
 * <NAME>
 * <DATA> TIO_List_States
 * </NAME>
 *
 * <PURP>
 * <DATA> Returns the number of States within a TyphonIO file and their names
 * </PURP>
 *
 * <CAPI>
 * <DATA> TIO_t TIO_List_States( const TIO_File_t fileID,
 * <DATA>                        TIO_Size_t       *nobj,
 * <DATA>                        char             names[][TIO_STRLEN] );
 * </CAPI>
 *
 * <PARA>
 * <DATA> IN  <DATA> fileID   <DATA> TyphonIO File ID
 * <DATA> OUT <DATA> nobj     <DATA> Number of States
 * <DATA> OUT <DATA> names    <DATA> Array of names
 * </PARA>
 *
 * <RETN>
 * <DATA> TyphonIO return code
 * </RETN>
 *
 * <DESC>
 * <DATA> Does not have to be called collectively.
 * </DESC>
 * <CDES>
 * <DATA> <code>names</codes> can be <code>NULL</code>
 * </CDES>
 *
 * </ITEM>
 */
extern TIO_t TIO_List_States( const TIO_File_t fileID,
                              TIO_Size_t       *nobj,
                              char             names[][TIO_STRLEN] );


/***************************************************************************************************
 *
 * TIO_Create_State
 *
 ***************************************************************************************************
 *
 * <ITEM>
 *
 * <NAME>
 * <DATA> TIO_Create_State
 * </NAME>
 *
 * <PURP>
 * <DATA> Creates a new State within a TyphonIO file
 * </PURP>
 *
 * <CAPI>
 * <DATA> TIO_t TIO_Create_State( const TIO_File_t fileID,
 * <DATA>                         const char       name[],
 * <DATA>                         TIO_Object_t     *stateID,
 * <DATA>                         const TIO_Step_t step,
 * <DATA>                         const TIO_Time_t time,
 * <DATA>                         const char       units[] );
 * </CAPI>
 *
 * <PARA>
 * <DATA> IN  <DATA> fileID   <DATA> TyphonIO File ID
 * <DATA> IN  <DATA> name     <DATA> Name for State
 * <DATA> OUT <DATA> stateID  <DATA> ID of new State
 * <DATA> IN  <DATA> step     <DATA> Step number for this state
 * <DATA> IN  <DATA> time     <DATA> Time associated with this state
 * <DATA> IN  <DATA> units    <DATA> Units for the time
 * </PARA>
 *
 * <RETN>
 * <DATA> TyphonIO return code
 * </RETN>
 *
 * <DESC>
 * <DATA> The .
 * <DATA> Must be called collectively.
 * </DESC>
 *
 * </ITEM>
 */
extern TIO_t TIO_Create_State( const TIO_File_t fileID,
                               const char       name[],
                               TIO_Object_t     *stateID,
                               const TIO_Step_t step,
                               const TIO_Time_t time,
                               const char       units[]   );


/***************************************************************************************************
 *
 * TIO_Open_State
 *
 ***************************************************************************************************
 *
 * <ITEM>
 *
 * <NAME>
 * <DATA> TIO_Open_State
 * </NAME>
 *
 * <PURP>
 * <DATA> Opens a State from within a TyphonIO file by name
 * </PURP>
 *
 * <CAPI>
 * <DATA> TIO_t TIO_Open_State( const TIO_File_t fileID,
 * <DATA>                       const char       name[],
 * <DATA>                       TIO_Object_t     *stateID,
 * <DATA>                       TIO_Step_t       *step,
 * <DATA>                       TIO_Time_t       *time,
 * <DATA>                       char             units[] );
 * </CAPI>
 *
 * <PARA>
 * <DATA> IN  <DATA> fileID   <DATA> TyphonIO File ID
 * <DATA> IN  <DATA> name     <DATA> Name for State
 * <DATA> OUT <DATA> stateID  <DATA> ID of opened State
 * <DATA> OUT <DATA> step     <DATA> Step number for this state
 * <DATA> OUT <DATA> time     <DATA> Time associated with this state
 * <DATA> OUT <DATA> units    <DATA> Units for the time
 * </PARA>
 *
 * <RETN>
 * <DATA> TyphonIO return code
 * </RETN>
 *
 * <DESC>
 * <DATA> Must be called collectively.
 * </DESC>
 *
 * </ITEM>
 */
extern TIO_t TIO_Open_State( const TIO_File_t fileID,
                             const char       name[],
                             TIO_Object_t     *stateID,
                             TIO_Step_t       *step,
                             TIO_Time_t       *time,
                             char             units[]   );


/***************************************************************************************************
 *
 * TIO_Open_State_idx
 *
 ***************************************************************************************************
 *
 * <ITEM>
 *
 * <NAME>
 * <DATA> TIO_Open_State_idx
 * </NAME>
 *
 * <PURP>
 * <DATA> Opens a new State within a TyphonIO file by index
 * </PURP>
 *
 * <CAPI>
 * <DATA> TIO_t TIO_Open_State_idx( const TIO_File_t fileID,
 * <DATA>                           const TIO_Size_t idx,
 * <DATA>                           char             name[],
 * <DATA>                           TIO_Object_t     *stateID,
 * <DATA>                           TIO_Step_t       step,
 * <DATA>                           TIO_Time_t       time,
 * <DATA>                           char             units[] );
 * </CAPI>
 *
 * <PARA>
 * <DATA> IN  <DATA> fileID   <DATA> TyphonIO File ID
 * <DATA> IN  <DATA> idx      <DATA> Index number for the State
 * <DATA> OUT <DATA> name     <DATA> Name for State
 * <DATA> OUT <DATA> stateID  <DATA> ID of opened State
 * <DATA> OUT <DATA> step     <DATA> Step number for this state
 * <DATA> OUT <DATA> time     <DATA> Time associated with this state
 * <DATA> OUT <DATA> units    <DATA> Units for the time
 * </PARA>
 *
 * <RETN>
 * <DATA> TyphonIO return code
 * </RETN>
 *
 * <DESC>
 * <DATA> Must be called collectively.
 * </DESC>
 *
 * </ITEM>
 */
extern TIO_t TIO_Open_State_idx( const TIO_File_t fileID,
                                 const TIO_Size_t idx,
                                 char             name[],
                                 TIO_Object_t     *stateID,
                                 TIO_Step_t       *step,
                                 TIO_Time_t       *time,
                                 char             units[]   );


/***************************************************************************************************
 *
 * TIO_Get_State_Info
 *
 ***************************************************************************************************
 *
 * <ITEM>
 *
 * <NAME>
 * <DATA> TIO_Get_State_Info
 * </NAME>
 *
 * <PURP>
 * <DATA> Gets the info for an open State
 * </PURP>
 *
 * <CAPI>
 * <DATA> TIO_t TIO_Get_State_Info( const TIO_File_t   fileID,
 * <DATA>                           const TIO_Object_t stateID,
 * <DATA>                           char               name[]
 * <DATA>                           TIO_Step_t         step,
 * <DATA>                           TIO_Time_t         time,
 * <DATA>                           char               units[] );
 * </CAPI>
 *
 * <PARA>
 * <DATA> IN  <DATA> fileID   <DATA> TyphonIO File ID
 * <DATA> IN  <DATA> stateID  <DATA> TyphonIO State ID
 * <DATA> OUT <DATA> name     <DATA> Name for State
 * <DATA> OUT <DATA> step     <DATA> Step number for this state
 * <DATA> OUT <DATA> time     <DATA> Time associated with this state
 * <DATA> OUT <DATA> units    <DATA> Units for the time
 * </PARA>
 *
 * <RETN>
 * <DATA> TyphonIO return code
 * </RETN>
 *
 * <DESC>
 * <DATA> Does not have to be called collectively.
 * </DESC>
 *
 * </ITEM>
 */
extern TIO_t TIO_Get_State_Info( const TIO_File_t   fileID,
                                 const TIO_Object_t stateID,
                                 char               name[],
                                 TIO_Step_t         *step,
                                 TIO_Time_t         *time,
                                 char               units[] );


/***************************************************************************************************
 *
 * TIO_Close_State
 *
 ***************************************************************************************************
 *
 * <ITEM>
 *
 * <NAME>
 * <DATA> TIO_Close_State
 * </NAME>
 *
 * <PURP>
 * <DATA> Closes an open State
 * </PURP>
 *
 * <CAPI>
 * <DATA> TIO_t TIO_Close_State( const TIO_File_t   fileID,
 * <DATA>                        const TIO_Object_t stateID );
 * </CAPI>
 *
 * <PARA>
 * <DATA> IN <DATA> fileID  <DATA> TyphonIO File ID
 * <DATA> IN <DATA> stateID <DATA> TyphonIO State ID
 * </PARA>
 *
 * <RETN>
 * <DATA> TyphonIO return code
 * </RETN>
 *
 * <DESC>
 * <DATA> Must be called collectively.
 * </DESC>
 *
 * </ITEM>
 */
extern TIO_t TIO_Close_State( const TIO_File_t   fileID,
                              const TIO_Object_t stateID );




#ifdef __cplusplus
}
#endif

#endif

/*
 * EOF
 */
