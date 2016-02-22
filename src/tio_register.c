/*
 Crown Copyright 2014 AWE.

 This file is part of TyphonIO. 
 Released under the BSD 3-clause license. 
 For more details see license.txt
 */

#include "tio_global_int.h"
#include "tio_error_int.h"

#include "tio_register_int.h"

#include <sys/types.h>
#include <sys/stat.h>
#ifndef _WIN32
#include <unistd.h>
#endif


/*
 *   File register is an array of structs containing pointers to iFile_t's and the ffinfo
 *   File register grows with number of open files
 */

static size_t FileRegisterSize  = 0;                /* Will be = FileRegisterDelta on 1st call  */
static size_t FileRegisterIndex = 0;                /* Current position in file register        */
static size_t FileRegisterHoles = 0;                /* Number of empty slots in file register   */
static size_t FileRegisterDelta = 5;                /* Just a reasonable start/growth number    */
static size_t FileTally         = 0;                /* Tally of the number of open files        */

struct xFileCache_t {
  TIO_File_t     ID;
  struct iFile_t *tfile;
  void           *ffinfo;
};

static struct xFileCache_t *FileRegister  = NULL;   /* FileRegister structure (to be alloc'ed) */
static struct xFileCache_t *LastFileCache = NULL;



/*
 *   Object register is a chained hash table.
 *   Due to hashing, object register does not need to grow - the entries simply get more members.
 *
 *   Hopefully there should be enough entries in the table so that each queue only has the head
 *   member.  We always keep a head member allocated to prevent too much memory thrashing.
 *   It's expected that due to the way that TyphonIO works (access-action-close) the head members
 *   will always be the head members, ie. latter member will be removed before the head.
 *   Algorithm will cope if the head is removed though.
 */

static const size_t ObjectRegisterSize  = 200;
static size_t       ObjectRegisterIndex = 0;
static size_t       ObjectTally         = 0;

#define objectID2pos(x) ((size_t)(x-1) %  ObjectRegisterSize);

struct xObjectCache_t {
  TIO_Object_t          ID;
  TIO_Object_t          parentID;
  TIO_File_t            fileID;
  iClass_t              class;
  void                  *tobject;
  void                  *ffinfo;
  struct xObjectCache_t *prev;
  struct xObjectCache_t *next;
};

struct xRegister_t {
  size_t n;
  struct xObjectCache_t *head;
};

static struct xRegister_t    *ObjectRegister  = NULL;  /* ObjectRegister structure */
static struct xObjectCache_t *LastObjectCache = NULL;




static int xTIO_FindFile  ( const TIO_Object_t    fileID,
                            struct xFileCache_t   **cache);

static int xTIO_FindObject( const TIO_Object_t    objectID,
                            struct xObjectCache_t **cache );



/***************************************************************************************************
 *
 * iTIO_RegisterFile
 *
 **************************************************************************************************/
int
iTIO_RegisterFile( TIO_File_t *fileID, struct iFile_t *tfile )
{
  const char *subname = "iTIO_RegisterFile()";

  size_t newsize;     /* Extended size of file register */
  size_t ii;          /* Loop over entries in the file register table */



  if (FileRegisterIndex < FileRegisterSize) {
    ii = FileRegisterIndex;
    ++FileRegisterIndex;
  }
  else if (FileRegisterHoles > 0) {
    /* -- Find the first empty position in the register */
    for (ii=0; ii<FileRegisterSize; ++ii) {
      if (0 == FileRegister[ii].ID) {
        break;
      }
    }
    --FileRegisterHoles;
  }
  else {

    /* -- Enlarge the file register if necessary */

    newsize = FileRegisterSize + FileRegisterDelta;

    FileRegister = (struct xFileCache_t *)TIOrealloc( FileRegister,
                                                      (sizeof(struct xFileCache_t) * newsize) );

    if (NULL == FileRegister) {
      TIOreturn(ERR_MEM, "Failed to realloc FileRegister", -1);
    }

    for (ii=FileRegisterSize; ii<newsize; ++ii) {
      FileRegister[ii].ID     = 0;
#ifndef NDEBUG
      FileRegister[ii].tfile  = NULL;
      FileRegister[ii].ffinfo = NULL;
#endif
    }

    ii = FileRegisterSize;

    FileRegisterSize = newsize;

    ++FileRegisterIndex;

  }


  /* -- Set the register entry at this position */
  FileRegister[ii].ID        = (TIO_File_t)(ii + 1);
  FileRegister[ii].tfile     = tfile;

  /* -- Return the TyphonIO fileID, which corresponds to the file's position in the register */
  *fileID = FileRegister[ii].ID;

  /* -- Increment the tally */
  ++FileTally;

  LastFileCache = &FileRegister[ii];


  return (0);
}




/***************************************************************************************************
 *
 * iTIO_DeregisterFile
 *
 **************************************************************************************************/
int
iTIO_DeregisterFile( const TIO_File_t fileID )
{
  const char *subname = "iTIO_DeregisterFile()";

  struct xFileCache_t *fcache;

  int    irc;


  irc = xTIO_FindFile(fileID, &fcache);
  if (irc < 0) {
    TIOreturn(ERR_INT, "Could not find file", ITIO_ERR_FILEID);
  }

  ++FileRegisterHoles;

  fcache->ID = 0;

  /* -- Decrement the file tally */
  --FileTally;


  /* -- If there are no files left, free the file register structure */
  if (0 == FileTally) {
    TIOfree (FileRegister);
    FileRegister      = NULL;
    FileRegisterSize  = 0;
    FileRegisterIndex = 0;
    FileRegisterHoles = 0;
  }

  LastFileCache = NULL;


  return (0);
}




/***************************************************************************************************
 *
 * iTIO_GetFile
 *
 **************************************************************************************************/
int
iTIO_GetFile( const TIO_File_t fileID, struct iFile_t **tfile )
{
  const char *subname = "iTIO_GetFile()";

  struct xFileCache_t *fcache;

  int    irc;


  irc = xTIO_FindFile(fileID, &fcache);
  if (irc < 0) {
    TIOreturn(ERR_INT, "Could not find file", ITIO_ERR_FILEID);
  }

  if (tfile != NULL) {
    *tfile = fcache->tfile;
  }

  LastFileCache = fcache;


  return (0);
}




/***************************************************************************************************
 *
 * iTIO_GetFileInstances
 *
 **************************************************************************************************/
int
iTIO_GetFileInstances( const TIO_File_t fileID, const char filename[], int *writeable )
{
  const char *subname = "iTIO_GetFileInstances()";

  struct xFileCache_t *fcache;

  const char *fname;


  struct stat statbuf;
  ino_t f_ino, r_ino;

  int ii, ni, iwrite;
  int irc;


  if (0 == FileTally) {
    TIOset(writeable, FALSE);
    return (0);
  }

  iwrite = FALSE;

  if (filename != NULL) {
    fname = filename;
  }
  else {
    irc = xTIO_FindFile( fileID, &fcache);
    if (irc < 0) {
      TIOreturn(ERR_INT, "Could not find file", ITIO_ERR_FILEID);
    }
    fname = (fcache->tfile)->name;
  }


  irc = stat(fname, &statbuf);
  if (irc < 0) {
    TIOreturn(ERR_INT, "Could not find file", -100);   /* User may have given invalid filename */
  }

  f_ino = statbuf.st_ino;

  ni = 0;
  for (ii=0; ii<(int)FileRegisterSize; ++ii) {
    if (FileRegister[ii].ID != 0) {
      irc = stat((FileRegister[ii].tfile)->name, &statbuf);

      r_ino = statbuf.st_ino;

      if (r_ino == f_ino) {
        ++ni;
        iwrite = (iwrite || (FileRegister[ii].tfile)->writeable);
      }
    }
  }


#ifdef ANOTHER_WAY
{
  char   *rfnameA=NULL, *rfnameB=NULL;

  rfnameA = (char *)TIOmalloc(PATH_MAX);
  rfnameB = (char *)TIOmalloc(PATH_MAX);

  if (realpath(fname, rfnameA) == NULL) {
    TIOreturn(ERR_INT, "Failed in realpath", -1);
  }

  ni = 0;

  for (ii=0; ii<FileRegisterSize; ++ii) {
    if (FileRegister[ii].ID != 0) {

      if (realpath((FileRegister[ii].tfile)->name, rfnameB) == NULL) {
        return (ERR_INT, "Failed in realpath", -2);
      }
      if (strcmp(rfnameA, rfnameB) == 0) {
        ++ni;
      }
    }
  }

  TIOfree (rfnameA); rfnameA = NULL;
  TIOfree (rfnameB); rfnameB = NULL;
}
#endif


  TIOset(writeable, iwrite);

  return (ni);
}




/***************************************************************************************************
 *
 * iTIO_RegisterObject
 *
 **************************************************************************************************/
int
iTIO_RegisterObject( const TIO_File_t   fileID,
                     const TIO_Object_t parentID,
                     TIO_Object_t       *objectID,
                     const iClass_t     class,
                     void               *tobject )
{
  const char *subname = "iTIO_RegisterObject()";

  struct xRegister_t    *ireg;
  struct xObjectCache_t *icache;

  size_t i, pos;    /* Loop & position within object register */



  /* -- Allocate and initialize the object register if it hasn't been already */
  /*    Here we allocate the queue and the first head of each node in the queue
        Successive linked nodes from the heads are malloc-ed as needed */

  if (0 == ObjectTally) {

    ObjectRegisterIndex = 0;  /* May as well re-initialize to prevent overflowing... */

    ObjectRegister = (struct xRegister_t *)TIOmalloc(sizeof(struct xRegister_t) * ObjectRegisterSize);
    if (NULL == ObjectRegister) {
      TIOreturn(ERR_MEM, "Failed to malloc ObjectRegister", -1);
    }

    for (i=0; i<ObjectRegisterSize; ++i) {

      ireg = &ObjectRegister[i];

      ireg->n    = 0;
      ireg->head = NULL;
      ireg->head = (struct xObjectCache_t *)TIOmalloc(sizeof(struct xObjectCache_t));
      if (NULL == ireg->head) {
        TIOreturn(ERR_MEM, "Failed to malloc ObjectRegister[i].head", -2);
      }

      icache = ireg->head;

      icache->prev    = NULL;
      icache->next    = NULL;

#ifndef NDEBUG
      icache->ID      = TIO_NULL;
      icache->class   = CLASS_NULL;
      icache->fileID  = TIO_NULL;
      icache->tobject = NULL;
      icache->ffinfo  = NULL;
#endif

    }
  }


  /* -- Adding a new file to the register, so increment the tally */
  ++ObjectTally;
  ++ObjectRegisterIndex;


  /* -- Next position in the register */
  pos = objectID2pos(ObjectRegisterIndex);


  /* -- Put the entry into the hash-queue register */
  /*    If position in entry already has contents, expand linked-list and place at the end */

  ireg   = &ObjectRegister[pos];
  icache = ireg->head;


  if (ireg->n > 0) {

    for (i=0; i<(ireg->n-1); ++i) {
      icache = icache->next;    /* Traverse to end of linked-list */
    }

    icache->next = TIOmalloc(sizeof(struct xObjectCache_t));
    if (NULL == icache->next) {
      TIOreturn(ERR_MEM, "Failed to malloc icache->next", -3);
    }

    (icache->next)->prev = icache;
    icache               = icache->next;
    icache->next         = NULL;

#ifndef NDEBUG
    icache->ID      = TIO_NULL;
    icache->class   = CLASS_NULL;
    icache->fileID  = TIO_NULL;
    icache->tobject = NULL;
    icache->ffinfo  = NULL;
#endif

  }


  icache->ID       = (TIO_Object_t)ObjectRegisterIndex;
  icache->parentID = parentID;
  icache->fileID   = fileID;
  icache->class    = class;
  icache->tobject  = tobject;

  ++ireg->n;

  /* -- Return the normalized TyphonIO ID, which corresponds to a hash position in the register */
  /*    Even though a position in the register can have >1 entries, objectIDs are still unique */

  *objectID = icache->ID;

  LastObjectCache = icache;


  return (0);
}




/***************************************************************************************************
 *
 * iTIO_DeregisterObject
 *
 **************************************************************************************************/
int
iTIO_DeregisterObject( const TIO_File_t   fileID,
                       const TIO_Object_t objectID )
{
  const char *subname = "iTIO_DeregisterObject()";

  struct xObjectCache_t *ocache;
  size_t pos;
  int    irc;


  irc = xTIO_FindObject(objectID, &ocache);
  if (irc < 0) {
    TIOreturn(ERR_INT, "Could not find object", ITIO_ERR_OBJECTID);
  }

  pos = objectID2pos(objectID);


  if (ObjectRegister[pos].head == ocache) {
    /* First in the LL */

    if (ObjectRegister[pos].n > 1) {

      /* More than one element in the LL */

      /* Free the current head member and point at the next one */
      TIOfree (ObjectRegister[pos].head); ObjectRegister[pos].head = NULL;
      ObjectRegister[pos].head = ocache->next;

    }
    else {

      /* Only element in the LL */

      /* Don't free the head one - will re-use to save hammering dynamic memory too much */
      /* Don't really need to do anything */
#ifndef NDEBUG
      ocache->ID      = TIO_NULL;
      ocache->class   = CLASS_NULL;
      ocache->fileID  = TIO_NULL;
      ocache->tobject = NULL;
      ocache->ffinfo  = NULL;
#endif

    }

  }
  else {

    /* Last in the LL */
    if (NULL == ocache->next) {
      (ocache->prev)->next = NULL;
    }

    else {
      /* Anywhere else */
      (ocache->next)->prev = ocache->prev;
      (ocache->prev)->next = ocache->next;
    }

    TIOfree (ocache); ocache = NULL;
  }


  --ObjectRegister[pos].n;

  --ObjectTally;

  if (0 == ObjectTally) {

    for (pos=0; pos<ObjectRegisterSize; ++pos) {
      TIOfree (ObjectRegister[pos].head);
      ObjectRegister[pos].head = NULL;
    }

    TIOfree (ObjectRegister);
    ObjectRegister = NULL;

    ObjectRegisterIndex   = 0;
  }

  LastObjectCache = NULL;


  return (0);
}




/***************************************************************************************************
 *
 * iTIO_GetObject
 *
 **************************************************************************************************/
int
iTIO_GetObject( const TIO_File_t   fileID,
                TIO_Object_t       *parentID,
                const TIO_Object_t objectID,
                const iClass_t     class,
                void               **tobject )
{
  const char *subname = "iTIO_GetObject()";

  struct xObjectCache_t *ocache;
  int    irc;


  irc = xTIO_FindObject(objectID, &ocache);
  if (irc < 0) {
    TIOreturn(ERR_INT, "Failed to find object", ITIO_ERR_OBJECTID);
  }
  if (ocache->class != class) {
    TIOreturn(ERR_INT, "Object found is of wrong class", ITIO_ERR_CLASS);
  }

  TIOset(parentID, ocache->parentID);
  TIOset(tobject,  ocache->tobject);

  LastObjectCache = ocache;


  return (0);
}




/***************************************************************************************************
 *
 * iTIO_GetClassObject
 *
 **************************************************************************************************/
int
iTIO_GetClassObject( const TIO_File_t   fileID,
                     const TIO_Object_t objectID,
                     iClass_t           *class,
                     void               **tobject)
{
  const char *subname = "iTIO_GetClassObject()";

  struct xObjectCache_t *ocache;
  int    irc;


  irc = xTIO_FindObject(objectID, &ocache);
  if (irc < 0) {
    TIOreturn(ERR_INT, "Failed to find object", ITIO_ERR_OBJECTID);
  }

  TIOset(class,   ocache->class);
  TIOset(tobject, ocache->tobject);

  LastObjectCache = ocache;


  return (0);
}




/***************************************************************************************************
 *
 * iTIO_GetAnyObject
 *
 **************************************************************************************************/
int
iTIO_GetAnyObject( const TIO_File_t fileID,
                   const iClass_t   class,
                   TIO_Object_t     *objectID )
{
  const char *subname = "iTIO_GetAnyObject()";

  struct xObjectCache_t *ocache;
  size_t ri, ni, nobj;
  int    found;


  /* Routine returns the objectID of the first object found in the register that matches the given
     fileID and class */


  if (0 == ObjectTally) {
    return (0);   /* Chances are there wont be any objects when called */
  }

  /* If here then there are still objects in the register - could be objects from multiple files */

  found = FALSE;
  nobj  = 0;        /* Keep track of number of objects parsed - so can quit out when done all */

  for (ri=0; ri<ObjectRegisterSize; ++ri) {

    switch (ObjectRegister[ri].n) {

    case 0:
      continue;
      break;  /* No objects in this position so continue */

    case 1:
      ++nobj;
      ocache = ObjectRegister[ri].head;
      found  = ( (ocache->fileID == fileID) && (ocache->class == class) );
      break;

    default:
      ocache = ObjectRegister[ri].head;
      for (ni=0; ni<ObjectRegister[ri].n; ++ni) {
        ++nobj;
        if ( (ocache->fileID == fileID) && (ocache->class == class) ) {
          found = TRUE;
          break;
        }
        ocache = ocache->next;
      }
      break;

    }

    if (found) {
      break;
    }
    if (nobj == ObjectTally) {
      break;
    }
  }


  if (found) {
    *objectID = ocache->ID;
    return (1);  /* 1 = routine worked fine and found an object */
  }


  return (0);  /* 0 = routine worked fine but didn't find any object. -ve = routine broke */
}



/***************************************************************************************************
 *
 * iTIO_GetChildren
 *
 **************************************************************************************************/
int
iTIO_GetChildren( const TIO_File_t   fileID,
                  const TIO_Object_t parentID,
                  TIO_Size_t         *nchildren,
                  TIO_Object_t       *childrenID )
{
  const char *subname = "iTIO_GetAnyObjectByName()";

  struct xObjectCache_t *ocache;
  size_t ri, ni, lnc;
  int    found;


  /* Routine returns the IDs of the open child objects of the given parent */

  lnc = 0;

  if (0 == ObjectTally) {      /* Chances are there wont be any objects when called */
    TIOset(nchildren, lnc);
    return (0);
  }


  for (ri=0; ri<ObjectRegisterSize; ++ri) {

    switch (ObjectRegister[ri].n) {

    case 0:
      continue;
      break;  /* No objects in this position so continue */

    case 1:
      ocache = ObjectRegister[ri].head;
      if ((ocache->fileID == fileID) && (ocache->parentID == parentID)) {
        if (childrenID != NULL) {
          childrenID[lnc] = ocache->ID;
        }
        ++lnc;
      }
      break;

    default:
      ocache = ObjectRegister[ri].head;
      for (ni=0; ni<ObjectRegister[ri].n; ++ni) {
        if ((ocache->fileID == fileID) && (ocache->parentID == parentID)) {
          if (childrenID != NULL) {
            childrenID[lnc] = ocache->ID;
          }
          ++lnc;
        }
        ocache = ocache->next;
      }
      break;

    }
  }

  TIOset(nchildren, lnc);

  return (0);
}




/***************************************************************************************************
 *
 * iTIO_RegisterFFinfo
 *
 **************************************************************************************************/
int
iTIO_RegisterFFinfo( const TIO_File_t fileID, const TIO_Object_t objectID, void *ffinfo )
{
  const char *subname = "iTIO_RegisterFFinfo()";

  struct xFileCache_t   *fcache;
  struct xObjectCache_t *ocache;
  int irc;


  if (fileID > 0) {

    /* FFinfo for file */

    irc = xTIO_FindFile( fileID, &fcache );
    if (irc < 0) {
      TIOreturn(ERR_INT, "Failed to find file", ITIO_ERR_FILEID);
    }

    fcache->ffinfo = ffinfo;

  }
  else {

    /* FFinfo for object */
    irc = xTIO_FindObject( objectID, &ocache );
    if (irc < 0) {
      TIOreturn (ERR_INT, "Failed to find object", ITIO_ERR_OBJECTID);
    }

    ocache->ffinfo = ffinfo;

  }


  return (0);
}




/***************************************************************************************************
 *
 * iTIO_DeregisterFFinfo
 *
 **************************************************************************************************/
int
iTIO_DeregisterFFinfo( const TIO_File_t fileID, const TIO_Object_t objectID )
{

  /* Don't actually need to do anything! */


#ifndef NDEBUG

  const char *subname = "iTIO_DeregisterFFinfo()";

  struct xFileCache_t   *fcache;
  struct xObjectCache_t *ocache;

  int irc;


  /* Set caches to NULL */

  if (fileID > 0) {
    /* FFinfo for file */

    irc = xTIO_FindFile( fileID, &fcache );
    if (irc < 0) {
      TIOreturn (ERR_INT, "Failed to find file", ITIO_ERR_FILEID);
    }

    fcache->ffinfo = NULL;
  }
  else {
    /* FFinfo for object */

    irc = xTIO_FindObject( objectID, &ocache );
    if (irc < 0) {
      TIOreturn(ERR_INT, "Failed to find object", ITIO_ERR_OBJECTID);
    }

    ocache->ffinfo = NULL;
  }

#endif


  return (0);
}




/***************************************************************************************************
 *
 * iTIO_GetFFinfo
 *
 **************************************************************************************************/
int
iTIO_GetFFinfo( const TIO_File_t fileID, const TIO_Object_t objectID, void **ffinfo)
{
  const char *subname = "iTIO_GetFFinfo()";

  struct xFileCache_t   *fcache;
  struct xObjectCache_t *ocache;

  int irc;


  if (fileID > 0) {
    /* FFinfo for file */

    irc = xTIO_FindFile( fileID, &fcache );
    if (irc < 0) {
      TIOreturn(ERR_INT, "Failed to find file", ITIO_ERR_FILEID);
    }

    *ffinfo = fcache->ffinfo;
  }
  else {
    /* FFinfo for object */

    irc = xTIO_FindObject( objectID, &ocache );
    if (irc < 0) {
      TIOreturn(ERR_INT, "Failed to find object", ITIO_ERR_OBJECTID);
    }

    *ffinfo = ocache->ffinfo;
  }


  return (0);
}




/**************************************************************************************************
 *
 * xTIO_FindFile
 *
 **************************************************************************************************/
static int
xTIO_FindFile(const TIO_Object_t fileID, struct xFileCache_t **fcache)
{
  const char *subname = "xTIO_FindFile()";

  size_t i;


  if (LastFileCache != NULL) {
    if (LastFileCache->ID == fileID) {
      *fcache = LastFileCache;
      return (0);
    }
  }


  i = (size_t)(fileID - 1);

  /* -- Check fileID is valid and return -ve if not */

  if (FileTally < 1) {
    TIOreturn(ERR_INT, "FileTally < 1", -1);
  }

  if (i >= FileRegisterSize) {
    TIOreturn(ERR_INT, "i >= FileRegisterSize", -2);
  }

  TIOassert((0 == FileRegister[i].ID), ERR_INT, "FileRegister[i].ID == 0", -3);

  *fcache = &FileRegister[i];


  return (0);
}




/**************************************************************************************************
 *
 * xTIO_FindObject
 *
 **************************************************************************************************/
static int
xTIO_FindObject(const TIO_Object_t objectID, struct xObjectCache_t **cache)
{
  const char *subname = "xTIO_FindObject()";

  size_t pos, i;
  int    found;


  if (LastObjectCache != NULL) {
    if (LastObjectCache->ID == objectID) {
      *cache = LastObjectCache;
      return (0);
    }
  }


  pos = objectID2pos(objectID);

  if (NULL == ObjectRegister) {
    return (-1);
  }

  switch (ObjectRegister[pos].n) {

  case 0:
    TIOreturn(ERR_INT, "No object in register at calculated position", -2);
    break;

  case 1:
    *cache = ObjectRegister[pos].head;
    if ((*cache)->ID != objectID) {
      TIOreturn(ERR_INT, "Object in register does not have matching ID", -3);
    }
    break;

  default:
    found = FALSE;
    *cache = ObjectRegister[pos].head;
    for (i=0; i<ObjectRegister[pos].n; ++i) {
      if ((*cache)->ID == objectID) {
        found = TRUE;
        break;
      }
      *cache = (*cache)->next;
    }

    if (!found) {
      TIOreturn(ERR_INT, "Couldn't find object in hash at position", -4);
    }

    break;
  }

  return (0);
}






/*
 * EOF
 */
