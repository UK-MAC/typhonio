/*
 Crown Copyright 2014 AWE.

 This file is part of TyphonIO. 
 Released under the BSD 3-clause license. 
 For more details see license.txt
 */

#include "tio_global_int.h"
#include "tio_error_int.h"
#include "tio_register_int.h"

#include "tio_state.h"


static TIO_t cTIO_OpenState( const char       subname[],
                             const int        ByIndex,
                             const TIO_File_t fileID,
                             const TIO_Size_t idx,
                             char             name[],
                             TIO_Object_t     *stateID,
                             TIO_Step_t       *step,
                             TIO_Time_t       *time,
                             char             units[] );

static int xTIO_GetStateInfo( const char      subname[],
                              const int       getname,
                              struct iState_t *tstate,
                              char            *name,
                              TIO_Step_t      *step,
                              TIO_Time_t      *time,
                              char            units[] );




/***************************************************************************************************
 *
 * TIO_List_States
 *
 **************************************************************************************************/
TIO_t
TIO_List_States( const TIO_File_t fileID,
                 TIO_Size_t       *nobj,
                 char             names[][TIO_STRLEN] )
{
  const char *subname = "TIO_List_States()";    /* Function name for debugging and error */
  TIO_t trc;                                    /* Return code from internal routines */


  TIObegin(subname,1);

  trc = cTIO_HierarchyListObjects( subname, fileID, TIO_NULL, CLASS_FILE, CLASS_STATE,
                                   nobj, names );

  TIOend(subname,1);

  return (trc);
}




/***************************************************************************************************
 *
 * TIO_Create_State
 *
 **************************************************************************************************/
TIO_t
TIO_Create_State( const TIO_File_t fileID,
                  const char       name[],
                  TIO_Object_t     *stateID,
                  const TIO_Step_t step,
                  const TIO_Time_t time,
                  const char       units[] )
{
  const char *subname = "TIO_Create_State()";

  struct iFile_t  *tfile;
  struct iState_t *tstate;

  TIO_Object_t lstID;
  int          irc;
  TIO_t        trc;


  TIObegin(subname,1);

  *stateID = TIO_NULL;    /* Ensure NULL value on failure */


  /* -- Check object-specific inbound args are valid */

  irc = iTIO_ValidString(units, FALSE);
  if (irc < 0) {
    TIOreturn(ERR_USER, iTIO_GetStringError(irc, "units"), TIO_ERR_INVALID_STRING);
  }

  tstate = (struct iState_t *)TIOmalloc(sizeof(struct iState_t));
  if (NULL == tstate) {
    TIOreturn(ERR_MEM, "Failed to malloc tstate", TIO_ERR_INT);
  }

  trc = cTIO_HierarchyCreateObject( subname, fileID, TIO_NULL, CLASS_FILE, CLASS_STATE, name,
                                    &tfile, &lstID, tstate );
  if (trc != TIO_SUCCESS) {
    TIOfree (tstate); tstate = NULL;
    cTIOreturn (trc);
  }


  tstate->step = step;
  tstate->time = time;
  (void) iTIO_strcpy(tstate->name,  name);
  (void) iTIO_strcpy(tstate->units, units);


  /* -- Write the object attributes */
  irc = 0;
  irc += tfile->WriteAttr( fileID, TIO_NULL, lstID, CLASS_STATE, "step",  TIO_STEP_T,
                           (void *)&tstate->step );
  irc += tfile->WriteAttr( fileID, TIO_NULL, lstID, CLASS_STATE, "time",  TIO_TIME_T,
                           (void *)&tstate->time );
  irc += tfile->WriteAttr( fileID, TIO_NULL, lstID, CLASS_STATE, "units", TIO_STRING,
                           (void *)tstate->units );
  TIOassert(irc != 0, ERR_INT, "Failed to write attributes", TIO_ERR_INT);


  *stateID = lstID;

  TIOend(subname,1);

  return (TIO_SUCCESS);
}




/***************************************************************************************************
 *
 * TIO_Open_State
 * TIO_Open_State_idx
 *
 **************************************************************************************************/
TIO_t
TIO_Open_State( const TIO_File_t fileID,
                const char       name[],
                TIO_Object_t     *stateID,
                TIO_Step_t       *step,
                TIO_Time_t       *time,
                char             units[] )
{
  return ( cTIO_OpenState("TIO_Open_State()", FALSE,
                          fileID, TIO_NULL, (char *)name, stateID, step, time, units) );
}


TIO_t
TIO_Open_State_idx( const TIO_File_t fileID,
                    const TIO_Size_t idx,
                    char             name[],
                    TIO_Object_t     *stateID,
                    TIO_Step_t       *step,
                    TIO_Time_t       *time,
                    char             units[] )
{
  return ( cTIO_OpenState("TIO_Open_State_idx()", TRUE,
                          fileID, idx, name, stateID, step, time, units) );
}


static TIO_t
cTIO_OpenState( const char       subname[],
                const int        ByIndex,
                const TIO_File_t fileID,
                const TIO_Size_t idx,
                char             name[],
                TIO_Object_t     *stateID,
                TIO_Step_t       *step,
                TIO_Time_t       *time,
                char             units[] )
{
  struct iFile_t  *tfile;
  struct iState_t *tstate;

  TIO_Object_t lstID;
  int          irc;
  TIO_t        trc;


  TIObegin(subname,1);

  *stateID = TIO_NULL;    /* Ensure NULL value on failure */


  tstate = (struct iState_t *)TIOmalloc(sizeof(struct iState_t));
  if (NULL == tstate) {
    TIOreturn(ERR_MEM, "Failed to malloc tstate", TIO_ERR_INT);
  }

  trc = cTIO_HierarchyOpenObject( subname, fileID, TIO_NULL, CLASS_FILE, CLASS_STATE,
                                  name, ByIndex, idx,
                                  tstate->name, &tfile, &lstID, (void *)tstate );
  if (trc != TIO_SUCCESS) {
    TIOfree (tstate); tstate = NULL;
    cTIOreturn (trc);
  }


  /* -- Read object attributes */
  irc = 0;
  irc += tfile->ReadAttr( fileID, TIO_NULL, lstID, CLASS_STATE, "step",  TIO_STEP_T,
                          (void *)&tstate->step);
  irc += tfile->ReadAttr( fileID, TIO_NULL, lstID, CLASS_STATE, "time",  TIO_TIME_T,
                          (void *)&tstate->time);
  irc += tfile->ReadAttr( fileID, TIO_NULL, lstID, CLASS_STATE, "units", TIO_STRING,
                          (void *)tstate->units);
  TIOassert(irc != 0, ERR_INT, "Failed to read attributes", TIO_ERR_INT);


  irc = xTIO_GetStateInfo(subname, ByIndex, tstate, name, step, time, units);
  TIOassert(irc < 0, ERR_INT, "Failed to get state info", TIO_ERR_INT);

  *stateID = lstID;

  TIOend(subname,1);

  return (TIO_SUCCESS);
}




/***************************************************************************************************
 *
 * TIO_Get_State_Info
 *
 **************************************************************************************************/
TIO_t
TIO_Get_State_Info( const TIO_File_t   fileID,
                    const TIO_Object_t stateID,
                    char               *name,
                    TIO_Step_t         *step,
                    TIO_Time_t         *time,
                    char               units[] )
{
  const char *subname = "TIO_Get_State_Info()";    /* Function name for debugging and error */

  struct iState_t *tstate;

  int   irc;                           /* Return code from internal routines */
  TIO_t trc;


  TIObegin(subname,1);

  trc = cTIO_HierarchyGetObjectInfo( subname, fileID, NULL, stateID, CLASS_STATE,
                                     NULL, (void **)&tstate);
  if (trc != TIO_SUCCESS) {
    cTIOreturn (trc);
  }

  irc = xTIO_GetStateInfo(subname, TRUE, tstate, name, step, time, units);
  TIOassert(irc < 0, ERR_INT, "Failed to get state info", TIO_ERR_INT);

  TIOend(subname,1);

  return (TIO_SUCCESS);
}




/***************************************************************************************************
 *
 * TIO_Close_State
 *
 **************************************************************************************************/
TIO_t
TIO_Close_State( const TIO_File_t   fileID,
                 const TIO_Object_t stateID )
{
  const char *subname = "TIO_Close_State()";      /* Function name for debugging and error */

  struct iState_t *tstate;

  TIO_t trc;                                      /* Return code from internal routines */


  TIObegin(subname,1);

  trc = cTIO_HierarchyPreClose( subname, fileID, NULL, stateID, CLASS_STATE,
                                NULL, (void **)&tstate );
  if (trc != TIO_SUCCESS) {
    cTIOreturn (trc);
  }

  /* -- Free the object cache */
  TIOfree (tstate); tstate = NULL;

  trc = cTIO_HierarchyCloseObject( subname, fileID, TIO_NULL, stateID, CLASS_STATE );

  TIOend(subname,1);

  return (trc);
}




/**************************************************************************************************/
static int
xTIO_GetStateInfo( const char      subname[],
                   const int       getname,
                   struct iState_t *tstate,
                   char            *name,
                   TIO_Step_t      *step,
                   TIO_Time_t      *time,
                   char            units[] )
{
  if (getname && (name  != NULL)) {
    (void) iTIO_strcpy(name, tstate->name);
  }

  TIOset(step, tstate->step);
  TIOset(time, tstate->time);

  if (units != NULL) {
    (void) iTIO_strcpy(units, tstate->units);
  }


  return (0);
}




/*
 * EOF
 */
