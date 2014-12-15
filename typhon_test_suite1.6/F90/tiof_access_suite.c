/*
 Crown Copyright 2014 AWE.

 This file is part of TyphonIO. 
 Released under the BSD 3-clause license. 
 For more details see license.txt
 */

#include "tunit.h"

TUNIT_TEST( 1, "TIO_Get_LibVersion_f" );
TUNIT_TEST( 2, "TIO_Get_FileVersion_f" );
TUNIT_TEST( 3, "TIO_CheckFile_f" );
TUNIT_TEST( 4, "TIO_Open_f" );
TUNIT_TEST( 5, "TIO_CheckObject_f" );
TUNIT_TEST( 6, "TIO_IsFileOpen_f" );
TUNIT_TEST( 7, "TIO_IsObjectOpen_f" );
TUNIT_TEST( 8, "TIO_Create_f" );
TUNIT_TEST( 9, "TIO_Close_f" );
TUNIT_TEST( 10, "TIO_Flush_f" );
TUNIT_TEST( 11, "TIO_Get_File_Info_f" );

TUNIT_SUITE( "Access" )
{
  TUNIT_REGISTER_TEST( 1 );
  TUNIT_REGISTER_TEST( 2 );
  TUNIT_REGISTER_TEST( 3 );
  TUNIT_REGISTER_TEST( 4 );
  TUNIT_REGISTER_TEST( 5 );
  TUNIT_REGISTER_TEST( 6 );
  TUNIT_REGISTER_TEST( 7 );
  TUNIT_REGISTER_TEST( 8 );
  TUNIT_REGISTER_TEST( 9 );
  TUNIT_REGISTER_TEST( 10 );
  TUNIT_REGISTER_TEST( 11 );
}
