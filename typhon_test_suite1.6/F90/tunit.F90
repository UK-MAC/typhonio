!
! Crown Copyright 2014 AWE.
!
! This file is part of TyphonIO. 
! Released under the BSD 3-clause license. 
! For more details see license.txt
!


#define TUNIT_FORTRAN
#include "tunit.h"

module tunit
  use ISO_C_BINDING
  implicit none
  private

  type, bind(c) :: TestInfo
    character(kind=C_CHAR, len=200) :: name
    integer(kind=C_INT) :: num
    integer(kind=C_SIZE_T) :: comm
    integer(kind=C_SIZE_T) :: info
    integer(kind=4) :: fcomm
    integer(kind=4) :: finfo
    integer(kind=C_INT) :: rank
    integer(kind=C_INT) :: size
    type(C_PTR) :: exit_func
  end type TestInfo

  type, bind(c) :: TestResult
    type(C_PTR) :: messages
    integer(kind=C_SIZE_T) :: num_messages
    integer(kind=C_INT) ::  passed
  end type TestResult

  interface
    subroutine tunit_abort() bind(c, name='tunit_abort')
    end subroutine tunit_abort
  end interface

  interface
    subroutine append_message(messages, num_messages, msg, msg_len) bind(c, name='append_message')
      use ISO_C_BINDING
      type(C_PTR) :: messages
      integer(kind=C_SIZE_T) :: num_messages
      character(kind=C_CHAR) :: msg
      integer(kind=C_SIZE_T) :: msg_len
    end subroutine append_message
  end interface



  interface assertEqual
    module procedure assertEqual_int4_int4
    module procedure assertEqual_int8_int8
    module procedure assertEqual_log4_log4
    module procedure assertEqual_log8_log8
    module procedure assertEqual_real4_real4
    module procedure assertEqual_real8_real8
    module procedure assertEqual_string_string

    module procedure assertEqual_int4_1d_int4_1d
    module procedure assertEqual_int4_2d_int4_2d
    module procedure assertEqual_int4_3d_int4_3d

    module procedure assertEqual_int8_1d_int8_1d
    module procedure assertEqual_int8_2d_int8_2d
    module procedure assertEqual_int8_3d_int8_3d

    module procedure assertEqual_log4_1d_log4_1d
    module procedure assertEqual_log4_2d_log4_2d
    module procedure assertEqual_log4_3d_log4_3d

    module procedure assertEqual_log8_1d_log8_1d
    module procedure assertEqual_log8_2d_log8_2d
    module procedure assertEqual_log8_3d_log8_3d

    module procedure assertEqual_real4_1d_real4_1d
    module procedure assertEqual_real4_2d_real4_2d
    module procedure assertEqual_real4_3d_real4_3d

    module procedure assertEqual_real8_1d_real8_1d
    module procedure assertEqual_real8_2d_real8_2d
    module procedure assertEqual_real8_3d_real8_3d

    module procedure assertEqual_string_1d_string_1d
    module procedure assertEqual_string_2d_string_2d
    module procedure assertEqual_string_3d_string_3d
  end interface assertEqual

  interface assertNotEqual
    module procedure assertNotEqual_int4_int4
    module procedure assertNotEqual_int8_int8
    module procedure assertNotEqual_log4_log4
    module procedure assertNotEqual_log8_log8
    module procedure assertNotEqual_real4_real4
    module procedure assertNotEqual_real8_real8
    module procedure assertNotEqual_string_string
  end interface assertNotEqual

  interface assertClose
    module procedure assertClose_real4_real4_tol4
    module procedure assertClose_real4_real4_tol8

    module procedure assertClose_real8_real8_tol4
    module procedure assertClose_real8_real8_tol8

    module procedure assertClose_real4_1d_real4_1d_tol4
    module procedure assertClose_real4_2d_real4_2d_tol4
    module procedure assertClose_real4_3d_real4_3d_tol4

    module procedure assertClose_real4_1d_real4_1d_tol8
    module procedure assertClose_real4_2d_real4_2d_tol8
    module procedure assertClose_real4_3d_real4_3d_tol8

    module procedure assertClose_real8_1d_real8_1d_tol4
    module procedure assertClose_real8_2d_real8_2d_tol4
    module procedure assertClose_real8_3d_real8_3d_tol4

    module procedure assertClose_real8_1d_real8_1d_tol8
    module procedure assertClose_real8_2d_real8_2d_tol8
    module procedure assertClose_real8_3d_real8_3d_tol8
  end interface assertClose

  public :: TestInfo, TestResult, assertEqual, assertNotEqual, assertClose

contains




  logical function assertEqual_int4_int4(exp, act, exp_str, act_str, res, line, file) result(passed)
    integer(kind=4), intent(in) :: exp
    integer(kind=4), intent(in) :: act
    character(len=*), intent(in) :: exp_str
    character(len=*), intent(in) :: act_str
    type(TestResult), intent(inout) :: res
    integer, intent(in) :: line
    character(len=*), intent(in) :: file

    character(len=MSG_LEN) :: msg
    integer(kind=C_SIZE_T) :: msg_len

    passed = .true.
    msg(:) = ' '

    if (exp .ne. act) then
      write (msg,'(5a,i0,a,i0,3a,i0,a)') "Assertion { ", exp_str , " == ", act_str, &
      &             " } failed for value { ", exp, " == ", act, " } (", file, ":", line, ")"
      msg_len = len_trim(msg)

      call append_message(res%messages, res%num_messages, msg, msg_len)

      res%passed = 0
      passed = .false.
    end if
  end function assertEqual_int4_int4

  logical function assertEqual_int8_int8(exp, act, exp_str, act_str, res, line, file) result(passed)
    integer(kind=8), intent(in) :: exp
    integer(kind=8), intent(in) :: act
    character(len=*), intent(in) :: exp_str
    character(len=*), intent(in) :: act_str
    type(TestResult), intent(inout) :: res
    integer, intent(in) :: line
    character(len=*), intent(in) :: file

    character(len=MSG_LEN) :: msg
    integer(kind=C_SIZE_T) :: msg_len

    passed = .true.
    msg(:) = ' '

    if (exp .ne. act) then
      write (msg,'(5a,i0,a,i0,3a,i0,a)') "Assertion { ", exp_str , " == ", act_str, &
      &             " } failed for value { ", exp, " == ", act, " } (", file, ":", line, ")"
      msg_len = len_trim(msg)

      call append_message(res%messages, res%num_messages, msg, msg_len)

      res%passed = 0
      passed = .false.
    end if
  end function assertEqual_int8_int8

  logical function assertEqual_log4_log4(exp, act, exp_str, act_str, res, line, file) result(passed)
    logical(kind=4), intent(in) :: exp
    logical(kind=4), intent(in) :: act
    character(len=*), intent(in) :: exp_str
    character(len=*), intent(in) :: act_str
    type(TestResult), intent(inout) :: res
    integer, intent(in) :: line
    character(len=*), intent(in) :: file

    character(len=MSG_LEN) :: msg
    integer(kind=C_SIZE_T) :: msg_len

    passed = .true.
    msg(:) = ' '

    if (exp .ne. act) then
      write (msg,'(5a,i0,a,i0,3a,i0,a)') "Assertion { ", exp_str , " == ", act_str, &
      &             " } failed for value { ", exp, " == ", act, " } (", file, ":", line, ")"
      msg_len = len_trim(msg)

      call append_message(res%messages, res%num_messages, msg, msg_len)

      res%passed = 0
      passed = .false.
    end if
  end function assertEqual_log4_log4

  logical function assertEqual_log8_log8(exp, act, exp_str, act_str, res, line, file) result(passed)
    logical(kind=8), intent(in) :: exp
    logical(kind=8), intent(in) :: act
    character(len=*), intent(in) :: exp_str
    character(len=*), intent(in) :: act_str
    type(TestResult), intent(inout) :: res
    integer, intent(in) :: line
    character(len=*), intent(in) :: file

    character(len=MSG_LEN) :: msg
    integer(kind=C_SIZE_T) :: msg_len

    passed = .true.
    msg(:) = ' '

    if (exp .ne. act) then
      write (msg,'(5a,i0,a,i0,3a,i0,a)') "Assertion { ", exp_str , " == ", act_str, &
      &             " } failed for value { ", exp, " == ", act, " } (", file, ":", line, ")"
      msg_len = len_trim(msg)

      call append_message(res%messages, res%num_messages, msg, msg_len)

      res%passed = 0
      passed = .false.
    end if
  end function assertEqual_log8_log8

  logical function assertEqual_real4_real4(exp, act, exp_str, act_str, res, line, file) result(passed)
    real(kind=4), intent(in) :: exp
    real(kind=4), intent(in) :: act
    character(len=*), intent(in) :: exp_str
    character(len=*), intent(in) :: act_str
    type(TestResult), intent(inout) :: res
    integer, intent(in) :: line
    character(len=*), intent(in) :: file

    character(len=MSG_LEN) :: msg
    integer(kind=C_SIZE_T) :: msg_len

    passed = .true.
    msg(:) = ' '

    if (exp .ne. act) then
      write (msg,'(5a,g12.3,a,g12.3,3a,i0,a)') "Assertion { ", exp_str , " == ", act_str, &
      &             " } failed for value { ", exp, " == ", act, " } (", file, ":", line, ")"
      msg_len = len_trim(msg)

      call append_message(res%messages, res%num_messages, msg, msg_len)

      res%passed = 0
      passed = .false.
    end if
  end function assertEqual_real4_real4

  logical function assertEqual_real8_real8(exp, act, exp_str, act_str, res, line, file) result(passed)
    real(kind=8), intent(in) :: exp
    real(kind=8), intent(in) :: act
    character(len=*), intent(in) :: exp_str
    character(len=*), intent(in) :: act_str
    type(TestResult), intent(inout) :: res
    integer, intent(in) :: line
    character(len=*), intent(in) :: file

    character(len=MSG_LEN) :: msg
    integer(kind=C_SIZE_T) :: msg_len

    passed = .true.
    msg(:) = ' '

    if (exp .ne. act) then
      write (msg,'(5a,g12.3,a,g12.3,3a,i0,a)') "Assertion { ", exp_str , " == ", act_str, &
      &             " } failed for value { ", exp, " == ", act, " } (", file, ":", line, ")"
      msg_len = len_trim(msg)

      call append_message(res%messages, res%num_messages, msg, msg_len)

      res%passed = 0
      passed = .false.
    end if
  end function assertEqual_real8_real8

  logical function assertEqual_string_string(exp, act, exp_str, act_str, res, line, file) result(passed)
    character(len=*), intent(in) :: exp
    character(len=*), intent(in) :: act
    character(len=*), intent(in) :: exp_str
    character(len=*), intent(in) :: act_str
    type(TestResult), intent(inout) :: res
    integer, intent(in) :: line
    character(len=*), intent(in) :: file

    character(len=MSG_LEN) :: msg
    integer(kind=C_SIZE_T) :: msg_len

    passed = .true.
    msg(:) = ' '

    if (exp .ne. act) then
      write (msg,'(5a,a,a,a,3a,i0,a)') "Assertion { ", exp_str , " == ", act_str, &
      &             " } failed for value { ", exp, " == ", act, " } (", file, ":", line, ")"
      msg_len = len_trim(msg)

      call append_message(res%messages, res%num_messages, msg, msg_len)

      res%passed = 0
      passed = .false.
    end if
  end function assertEqual_string_string




  logical function assertNotEqual_int4_int4(exp, act, exp_str, act_str, res, line, file) result(passed)
    integer(kind=4), intent(in) :: exp
    integer(kind=4), intent(in) :: act
    character(len=*), intent(in) :: exp_str
    character(len=*), intent(in) :: act_str
    type(TestResult), intent(inout) :: res
    integer, intent(in) :: line
    character(len=*), intent(in) :: file

    character(len=MSG_LEN) :: msg
    integer(kind=C_SIZE_T) :: msg_len

    passed = .true.
    msg(:) = ' '

    if (exp .eq. act) then
      write (msg,'(5a,i0,a,i0,3a,i0,a)') "Assertion { ", exp_str , " <> ", act_str, &
      &             " } failed for value { ", exp, " <> ", act, " } (", file, ":", line, ")"
      msg_len = len_trim(msg)

      call append_message(res%messages, res%num_messages, msg, msg_len)

      res%passed = 0
      passed = .false.
    end if
  end function assertNotEqual_int4_int4

  logical function assertNotEqual_int8_int8(exp, act, exp_str, act_str, res, line, file) result(passed)
    integer(kind=8), intent(in) :: exp
    integer(kind=8), intent(in) :: act
    character(len=*), intent(in) :: exp_str
    character(len=*), intent(in) :: act_str
    type(TestResult), intent(inout) :: res
    integer, intent(in) :: line
    character(len=*), intent(in) :: file

    character(len=MSG_LEN) :: msg
    integer(kind=C_SIZE_T) :: msg_len

    passed = .true.
    msg(:) = ' '

    if (exp .eq. act) then
      write (msg,'(5a,i0,a,i0,3a,i0,a)') "Assertion { ", exp_str , " <> ", act_str, &
      &             " } failed for value { ", exp, " <> ", act, " } (", file, ":", line, ")"
      msg_len = len_trim(msg)

      call append_message(res%messages, res%num_messages, msg, msg_len)

      res%passed = 0
      passed = .false.
    end if
  end function assertNotEqual_int8_int8

  logical function assertNotEqual_log4_log4(exp, act, exp_str, act_str, res, line, file) result(passed)
    logical(kind=4), intent(in) :: exp
    logical(kind=4), intent(in) :: act
    character(len=*), intent(in) :: exp_str
    character(len=*), intent(in) :: act_str
    type(TestResult), intent(inout) :: res
    integer, intent(in) :: line
    character(len=*), intent(in) :: file

    character(len=MSG_LEN) :: msg
    integer(kind=C_SIZE_T) :: msg_len

    passed = .true.
    msg(:) = ' '

    if (exp .eq. act) then
      write (msg,'(5a,i0,a,i0,3a,i0,a)') "Assertion { ", exp_str , " <> ", act_str, &
      &             " } failed for value { ", exp, " <> ", act, " } (", file, ":", line, ")"
      msg_len = len_trim(msg)

      call append_message(res%messages, res%num_messages, msg, msg_len)

      res%passed = 0
      passed = .false.
    end if
  end function assertNotEqual_log4_log4

  logical function assertNotEqual_log8_log8(exp, act, exp_str, act_str, res, line, file) result(passed)
    logical(kind=8), intent(in) :: exp
    logical(kind=8), intent(in) :: act
    character(len=*), intent(in) :: exp_str
    character(len=*), intent(in) :: act_str
    type(TestResult), intent(inout) :: res
    integer, intent(in) :: line
    character(len=*), intent(in) :: file

    character(len=MSG_LEN) :: msg
    integer(kind=C_SIZE_T) :: msg_len

    passed = .true.
    msg(:) = ' '

    if (exp .eq. act) then
      write (msg,'(5a,i0,a,i0,3a,i0,a)') "Assertion { ", exp_str , " <> ", act_str, &
      &             " } failed for value { ", exp, " <> ", act, " } (", file, ":", line, ")"
      msg_len = len_trim(msg)

      call append_message(res%messages, res%num_messages, msg, msg_len)

      res%passed = 0
      passed = .false.
    end if
  end function assertNotEqual_log8_log8

  logical function assertNotEqual_real4_real4(exp, act, exp_str, act_str, res, line, file) result(passed)
    real(kind=4), intent(in) :: exp
    real(kind=4), intent(in) :: act
    character(len=*), intent(in) :: exp_str
    character(len=*), intent(in) :: act_str
    type(TestResult), intent(inout) :: res
    integer, intent(in) :: line
    character(len=*), intent(in) :: file

    character(len=MSG_LEN) :: msg
    integer(kind=C_SIZE_T) :: msg_len

    passed = .true.
    msg(:) = ' '

    if (exp .eq. act) then
      write (msg,'(5a,g12.3,a,g12.3,3a,i0,a)') "Assertion { ", exp_str , " <> ", act_str, &
      &             " } failed for value { ", exp, " <> ", act, " } (", file, ":", line, ")"
      msg_len = len_trim(msg)

      call append_message(res%messages, res%num_messages, msg, msg_len)

      res%passed = 0
      passed = .false.
    end if
  end function assertNotEqual_real4_real4

  logical function assertNotEqual_real8_real8(exp, act, exp_str, act_str, res, line, file) result(passed)
    real(kind=8), intent(in) :: exp
    real(kind=8), intent(in) :: act
    character(len=*), intent(in) :: exp_str
    character(len=*), intent(in) :: act_str
    type(TestResult), intent(inout) :: res
    integer, intent(in) :: line
    character(len=*), intent(in) :: file

    character(len=MSG_LEN) :: msg
    integer(kind=C_SIZE_T) :: msg_len

    passed = .true.
    msg(:) = ' '

    if (exp .eq. act) then
      write (msg,'(5a,g12.3,a,g12.3,3a,i0,a)') "Assertion { ", exp_str , " <> ", act_str, &
      &             " } failed for value { ", exp, " <> ", act, " } (", file, ":", line, ")"
      msg_len = len_trim(msg)

      call append_message(res%messages, res%num_messages, msg, msg_len)

      res%passed = 0
      passed = .false.
    end if
  end function assertNotEqual_real8_real8

  logical function assertNotEqual_string_string(exp, act, exp_str, act_str, res, line, file) result(passed)
    character(len=*), intent(in) :: exp
    character(len=*), intent(in) :: act
    character(len=*), intent(in) :: exp_str
    character(len=*), intent(in) :: act_str
    type(TestResult), intent(inout) :: res
    integer, intent(in) :: line
    character(len=*), intent(in) :: file

    character(len=MSG_LEN) :: msg
    integer(kind=C_SIZE_T) :: msg_len

    passed = .true.
    msg(:) = ' '

    if (exp .eq. act) then
      write (msg,'(5a,a,a,a,3a,i0,a)') "Assertion { ", exp_str , " <> ", act_str, &
      &             " } failed for value { ", exp, " <> ", act, " } (", file, ":", line, ")"
      msg_len = len_trim(msg)

      call append_message(res%messages, res%num_messages, msg, msg_len)

      res%passed = 0
      passed = .false.
    end if
  end function assertNotEqual_string_string




  logical function assertClose_real4_real4_tol4(exp, act, tol, exp_str, act_str, res, line, file) result(passed)
    real(kind=4), intent(in) :: exp
    real(kind=4), intent(in) :: act
    real(kind=4), intent(in) :: tol
    character(len=*), intent(in) :: exp_str
    character(len=*), intent(in) :: act_str
    type(TestResult), intent(inout) :: res
    integer, intent(in) :: line
    character(len=*), intent(in) :: file

    character(len=MSG_LEN) :: msg
    integer(kind=C_SIZE_T) :: msg_len

    passed = .true.
    msg(:) = ' '

    if ((exp - act) > tol) then
      write (msg,'(5a,g12.3,a,g12.3,3a,i0,a)') "Assertion { ", exp_str , " == ", act_str, &
      &             " } failed for value { ", exp, " == ", act, " } (", file, ":", line, ")"
      msg_len = len_trim(msg)

      call append_message(res%messages, res%num_messages, msg, msg_len)

      res%passed = 0
      passed = .false.
    end if
  end function assertClose_real4_real4_tol4

  logical function assertClose_real4_real4_tol8(exp, act, tol, exp_str, act_str, res, line, file) result(passed)
    real(kind=4), intent(in) :: exp
    real(kind=4), intent(in) :: act
    real(kind=8), intent(in) :: tol
    character(len=*), intent(in) :: exp_str
    character(len=*), intent(in) :: act_str
    type(TestResult), intent(inout) :: res
    integer, intent(in) :: line
    character(len=*), intent(in) :: file

    character(len=MSG_LEN) :: msg
    integer(kind=C_SIZE_T) :: msg_len

    passed = .true.
    msg(:) = ' '

    if ((exp - act) > tol) then
      write (msg,'(5a,g12.3,a,g12.3,3a,i0,a)') "Assertion { ", exp_str , " == ", act_str, &
      &             " } failed for value { ", exp, " == ", act, " } (", file, ":", line, ")"
      msg_len = len_trim(msg)

      call append_message(res%messages, res%num_messages, msg, msg_len)

      res%passed = 0
      passed = .false.
    end if
  end function assertClose_real4_real4_tol8


  logical function assertClose_real8_real8_tol4(exp, act, tol, exp_str, act_str, res, line, file) result(passed)
    real(kind=8), intent(in) :: exp
    real(kind=8), intent(in) :: act
    real(kind=4), intent(in) :: tol
    character(len=*), intent(in) :: exp_str
    character(len=*), intent(in) :: act_str
    type(TestResult), intent(inout) :: res
    integer, intent(in) :: line
    character(len=*), intent(in) :: file

    character(len=MSG_LEN) :: msg
    integer(kind=C_SIZE_T) :: msg_len

    passed = .true.
    msg(:) = ' '

    if ((exp - act) > tol) then
      write (msg,'(5a,g12.3,a,g12.3,3a,i0,a)') "Assertion { ", exp_str , " == ", act_str, &
      &             " } failed for value { ", exp, " == ", act, " } (", file, ":", line, ")"
      msg_len = len_trim(msg)

      call append_message(res%messages, res%num_messages, msg, msg_len)

      res%passed = 0
      passed = .false.
    end if
  end function assertClose_real8_real8_tol4

  logical function assertClose_real8_real8_tol8(exp, act, tol, exp_str, act_str, res, line, file) result(passed)
    real(kind=8), intent(in) :: exp
    real(kind=8), intent(in) :: act
    real(kind=8), intent(in) :: tol
    character(len=*), intent(in) :: exp_str
    character(len=*), intent(in) :: act_str
    type(TestResult), intent(inout) :: res
    integer, intent(in) :: line
    character(len=*), intent(in) :: file

    character(len=MSG_LEN) :: msg
    integer(kind=C_SIZE_T) :: msg_len

    passed = .true.
    msg(:) = ' '

    if ((exp - act) > tol) then
      write (msg,'(5a,g12.3,a,g12.3,3a,i0,a)') "Assertion { ", exp_str , " == ", act_str, &
      &             " } failed for value { ", exp, " == ", act, " } (", file, ":", line, ")"
      msg_len = len_trim(msg)

      call append_message(res%messages, res%num_messages, msg, msg_len)

      res%passed = 0
      passed = .false.
    end if
  end function assertClose_real8_real8_tol8




  logical function assertEqual_int4_1d_int4_1d(exp, act, exp_str, act_str, res, line, file) result(passed)
    integer(kind=4), dimension(:), intent(in) :: exp
    integer(kind=4), dimension(:), intent(in) :: act
    character(len=*), intent(in) :: exp_str
    character(len=*), intent(in) :: act_str
    type(TestResult), intent(inout) :: res
    integer, intent(in) :: line
    character(len=*), intent(in) :: file

    character(len=MSG_LEN) :: msg
    integer(kind=C_SIZE_T) :: msg_len

    passed = .true.

    if (any(exp .ne. act)) then
      write (msg,'(5a,i0,a,i0,3a,i0,a)') "Assertion { ", exp_str , " == ", act_str, &
      &             " } failed for value { ", exp(1), "... == ", act(1), "... } (", file, ":", line, ")"
      msg_len = len_trim(msg)

      call append_message(res%messages, res%num_messages, msg, msg_len)

      res%passed = 0
      passed = .false.
    end if
  end function assertEqual_int4_1d_int4_1d

  logical function assertEqual_int4_2d_int4_2d(exp, act, exp_str, act_str, res, line, file) result(passed)
    integer(kind=4), dimension(:,:), intent(in) :: exp
    integer(kind=4), dimension(:,:), intent(in) :: act
    character(len=*), intent(in) :: exp_str
    character(len=*), intent(in) :: act_str
    type(TestResult), intent(inout) :: res
    integer, intent(in) :: line
    character(len=*), intent(in) :: file

    character(len=MSG_LEN) :: msg
    integer(kind=C_SIZE_T) :: msg_len

    passed = .true.

    if (any(exp .ne. act)) then
      write (msg,'(5a,i0,a,i0,3a,i0,a)') "Assertion { ", exp_str , " == ", act_str, &
      &             " } failed for value { ", exp(1,1), "... == ", act(1,1), "... } (", file, ":", line, ")"
      msg_len = len_trim(msg)

      call append_message(res%messages, res%num_messages, msg, msg_len)

      res%passed = 0
      passed = .false.
    end if
  end function assertEqual_int4_2d_int4_2d

  logical function assertEqual_int4_3d_int4_3d(exp, act, exp_str, act_str, res, line, file) result(passed)
    integer(kind=4), dimension(:,:,:), intent(in) :: exp
    integer(kind=4), dimension(:,:,:), intent(in) :: act
    character(len=*), intent(in) :: exp_str
    character(len=*), intent(in) :: act_str
    type(TestResult), intent(inout) :: res
    integer, intent(in) :: line
    character(len=*), intent(in) :: file

    character(len=MSG_LEN) :: msg
    integer(kind=C_SIZE_T) :: msg_len

    passed = .true.

    if (any(exp .ne. act)) then
      write (msg,'(5a,i0,a,i0,3a,i0,a)') "Assertion { ", exp_str , " == ", act_str, &
      &             " } failed for value { ", exp(1,1,1), "... == ", act(1,1,1), "... } (", file, ":", line, ")"
      msg_len = len_trim(msg)

      call append_message(res%messages, res%num_messages, msg, msg_len)

      res%passed = 0
      passed = .false.
    end if
  end function assertEqual_int4_3d_int4_3d


  logical function assertEqual_int8_1d_int8_1d(exp, act, exp_str, act_str, res, line, file) result(passed)
    integer(kind=8), dimension(:), intent(in) :: exp
    integer(kind=8), dimension(:), intent(in) :: act
    character(len=*), intent(in) :: exp_str
    character(len=*), intent(in) :: act_str
    type(TestResult), intent(inout) :: res
    integer, intent(in) :: line
    character(len=*), intent(in) :: file

    character(len=MSG_LEN) :: msg
    integer(kind=C_SIZE_T) :: msg_len

    passed = .true.

    if (any(exp .ne. act)) then
      write (msg,'(5a,i0,a,i0,3a,i0,a)') "Assertion { ", exp_str , " == ", act_str, &
      &             " } failed for value { ", exp(1), "... == ", act(1), "... } (", file, ":", line, ")"
      msg_len = len_trim(msg)

      call append_message(res%messages, res%num_messages, msg, msg_len)

      res%passed = 0
      passed = .false.
    end if
  end function assertEqual_int8_1d_int8_1d

  logical function assertEqual_int8_2d_int8_2d(exp, act, exp_str, act_str, res, line, file) result(passed)
    integer(kind=8), dimension(:,:), intent(in) :: exp
    integer(kind=8), dimension(:,:), intent(in) :: act
    character(len=*), intent(in) :: exp_str
    character(len=*), intent(in) :: act_str
    type(TestResult), intent(inout) :: res
    integer, intent(in) :: line
    character(len=*), intent(in) :: file

    character(len=MSG_LEN) :: msg
    integer(kind=C_SIZE_T) :: msg_len

    passed = .true.

    if (any(exp .ne. act)) then
      write (msg,'(5a,i0,a,i0,3a,i0,a)') "Assertion { ", exp_str , " == ", act_str, &
      &             " } failed for value { ", exp(1,1), "... == ", act(1,1), "... } (", file, ":", line, ")"
      msg_len = len_trim(msg)

      call append_message(res%messages, res%num_messages, msg, msg_len)

      res%passed = 0
      passed = .false.
    end if
  end function assertEqual_int8_2d_int8_2d

  logical function assertEqual_int8_3d_int8_3d(exp, act, exp_str, act_str, res, line, file) result(passed)
    integer(kind=8), dimension(:,:,:), intent(in) :: exp
    integer(kind=8), dimension(:,:,:), intent(in) :: act
    character(len=*), intent(in) :: exp_str
    character(len=*), intent(in) :: act_str
    type(TestResult), intent(inout) :: res
    integer, intent(in) :: line
    character(len=*), intent(in) :: file

    character(len=MSG_LEN) :: msg
    integer(kind=C_SIZE_T) :: msg_len

    passed = .true.

    if (any(exp .ne. act)) then
      write (msg,'(5a,i0,a,i0,3a,i0,a)') "Assertion { ", exp_str , " == ", act_str, &
      &             " } failed for value { ", exp(1,1,1), "... == ", act(1,1,1), "... } (", file, ":", line, ")"
      msg_len = len_trim(msg)

      call append_message(res%messages, res%num_messages, msg, msg_len)

      res%passed = 0
      passed = .false.
    end if
  end function assertEqual_int8_3d_int8_3d


  logical function assertEqual_log4_1d_log4_1d(exp, act, exp_str, act_str, res, line, file) result(passed)
    logical(kind=4), dimension(:), intent(in) :: exp
    logical(kind=4), dimension(:), intent(in) :: act
    character(len=*), intent(in) :: exp_str
    character(len=*), intent(in) :: act_str
    type(TestResult), intent(inout) :: res
    integer, intent(in) :: line
    character(len=*), intent(in) :: file

    character(len=MSG_LEN) :: msg
    integer(kind=C_SIZE_T) :: msg_len

    passed = .true.

    if (any(exp .neqv. act)) then
      write (msg,'(5a,i0,a,i0,3a,i0,a)') "Assertion { ", exp_str , " == ", act_str, &
      &             " } failed for value { ", exp(1), "... == ", act(1), "... } (", file, ":", line, ")"
      msg_len = len_trim(msg)

      call append_message(res%messages, res%num_messages, msg, msg_len)

      res%passed = 0
      passed = .false.
    end if
  end function assertEqual_log4_1d_log4_1d

  logical function assertEqual_log4_2d_log4_2d(exp, act, exp_str, act_str, res, line, file) result(passed)
    logical(kind=4), dimension(:,:), intent(in) :: exp
    logical(kind=4), dimension(:,:), intent(in) :: act
    character(len=*), intent(in) :: exp_str
    character(len=*), intent(in) :: act_str
    type(TestResult), intent(inout) :: res
    integer, intent(in) :: line
    character(len=*), intent(in) :: file

    character(len=MSG_LEN) :: msg
    integer(kind=C_SIZE_T) :: msg_len

    passed = .true.

    if (any(exp .neqv. act)) then
      write (msg,'(5a,i0,a,i0,3a,i0,a)') "Assertion { ", exp_str , " == ", act_str, &
      &             " } failed for value { ", exp(1,1), "... == ", act(1,1), "... } (", file, ":", line, ")"
      msg_len = len_trim(msg)

      call append_message(res%messages, res%num_messages, msg, msg_len)

      res%passed = 0
      passed = .false.
    end if
  end function assertEqual_log4_2d_log4_2d

  logical function assertEqual_log4_3d_log4_3d(exp, act, exp_str, act_str, res, line, file) result(passed)
    logical(kind=4), dimension(:,:,:), intent(in) :: exp
    logical(kind=4), dimension(:,:,:), intent(in) :: act
    character(len=*), intent(in) :: exp_str
    character(len=*), intent(in) :: act_str
    type(TestResult), intent(inout) :: res
    integer, intent(in) :: line
    character(len=*), intent(in) :: file

    character(len=MSG_LEN) :: msg
    integer(kind=C_SIZE_T) :: msg_len

    passed = .true.

    if (any(exp .neqv. act)) then
      write (msg,'(5a,i0,a,i0,3a,i0,a)') "Assertion { ", exp_str , " == ", act_str, &
      &             " } failed for value { ", exp(1,1,1), "... == ", act(1,1,1), "... } (", file, ":", line, ")"
      msg_len = len_trim(msg)

      call append_message(res%messages, res%num_messages, msg, msg_len)

      res%passed = 0
      passed = .false.
    end if
  end function assertEqual_log4_3d_log4_3d


  logical function assertEqual_log8_1d_log8_1d(exp, act, exp_str, act_str, res, line, file) result(passed)
    logical(kind=8), dimension(:), intent(in) :: exp
    logical(kind=8), dimension(:), intent(in) :: act
    character(len=*), intent(in) :: exp_str
    character(len=*), intent(in) :: act_str
    type(TestResult), intent(inout) :: res
    integer, intent(in) :: line
    character(len=*), intent(in) :: file

    character(len=MSG_LEN) :: msg
    integer(kind=C_SIZE_T) :: msg_len

    passed = .true.

    if (any(exp .neqv. act)) then
      write (msg,'(5a,i0,a,i0,3a,i0,a)') "Assertion { ", exp_str , " == ", act_str, &
      &             " } failed for value { ", exp(1), "... == ", act(1), "... } (", file, ":", line, ")"
      msg_len = len_trim(msg)

      call append_message(res%messages, res%num_messages, msg, msg_len)

      res%passed = 0
      passed = .false.
    end if
  end function assertEqual_log8_1d_log8_1d

  logical function assertEqual_log8_2d_log8_2d(exp, act, exp_str, act_str, res, line, file) result(passed)
    logical(kind=8), dimension(:,:), intent(in) :: exp
    logical(kind=8), dimension(:,:), intent(in) :: act
    character(len=*), intent(in) :: exp_str
    character(len=*), intent(in) :: act_str
    type(TestResult), intent(inout) :: res
    integer, intent(in) :: line
    character(len=*), intent(in) :: file

    character(len=MSG_LEN) :: msg
    integer(kind=C_SIZE_T) :: msg_len

    passed = .true.

    if (any(exp .neqv. act)) then
      write (msg,'(5a,i0,a,i0,3a,i0,a)') "Assertion { ", exp_str , " == ", act_str, &
      &             " } failed for value { ", exp(1,1), "... == ", act(1,1), "... } (", file, ":", line, ")"
      msg_len = len_trim(msg)

      call append_message(res%messages, res%num_messages, msg, msg_len)

      res%passed = 0
      passed = .false.
    end if
  end function assertEqual_log8_2d_log8_2d

  logical function assertEqual_log8_3d_log8_3d(exp, act, exp_str, act_str, res, line, file) result(passed)
    logical(kind=8), dimension(:,:,:), intent(in) :: exp
    logical(kind=8), dimension(:,:,:), intent(in) :: act
    character(len=*), intent(in) :: exp_str
    character(len=*), intent(in) :: act_str
    type(TestResult), intent(inout) :: res
    integer, intent(in) :: line
    character(len=*), intent(in) :: file

    character(len=MSG_LEN) :: msg
    integer(kind=C_SIZE_T) :: msg_len

    passed = .true.

    if (any(exp .neqv. act)) then
      write (msg,'(5a,i0,a,i0,3a,i0,a)') "Assertion { ", exp_str , " == ", act_str, &
      &             " } failed for value { ", exp(1,1,1), "... == ", act(1,1,1), "... } (", file, ":", line, ")"
      msg_len = len_trim(msg)

      call append_message(res%messages, res%num_messages, msg, msg_len)

      res%passed = 0
      passed = .false.
    end if
  end function assertEqual_log8_3d_log8_3d


  logical function assertEqual_real4_1d_real4_1d(exp, act, exp_str, act_str, res, line, file) result(passed)
    real(kind=4), dimension(:), intent(in) :: exp
    real(kind=4), dimension(:), intent(in) :: act
    character(len=*), intent(in) :: exp_str
    character(len=*), intent(in) :: act_str
    type(TestResult), intent(inout) :: res
    integer, intent(in) :: line
    character(len=*), intent(in) :: file

    character(len=MSG_LEN) :: msg
    integer(kind=C_SIZE_T) :: msg_len

    passed = .true.

    if (any(exp .ne. act)) then
      write (msg,'(5a,g12.3,a,g12.3,3a,i0,a)') "Assertion { ", exp_str , " == ", act_str, &
      &             " } failed for value { ", exp(1), "... == ", act(1), "... } (", file, ":", line, ")"
      msg_len = len_trim(msg)

      call append_message(res%messages, res%num_messages, msg, msg_len)

      res%passed = 0
      passed = .false.
    end if
  end function assertEqual_real4_1d_real4_1d

  logical function assertEqual_real4_2d_real4_2d(exp, act, exp_str, act_str, res, line, file) result(passed)
    real(kind=4), dimension(:,:), intent(in) :: exp
    real(kind=4), dimension(:,:), intent(in) :: act
    character(len=*), intent(in) :: exp_str
    character(len=*), intent(in) :: act_str
    type(TestResult), intent(inout) :: res
    integer, intent(in) :: line
    character(len=*), intent(in) :: file

    character(len=MSG_LEN) :: msg
    integer(kind=C_SIZE_T) :: msg_len

    passed = .true.

    if (any(exp .ne. act)) then
      write (msg,'(5a,g12.3,a,g12.3,3a,i0,a)') "Assertion { ", exp_str , " == ", act_str, &
      &             " } failed for value { ", exp(1,1), "... == ", act(1,1), "... } (", file, ":", line, ")"
      msg_len = len_trim(msg)

      call append_message(res%messages, res%num_messages, msg, msg_len)

      res%passed = 0
      passed = .false.
    end if
  end function assertEqual_real4_2d_real4_2d

  logical function assertEqual_real4_3d_real4_3d(exp, act, exp_str, act_str, res, line, file) result(passed)
    real(kind=4), dimension(:,:,:), intent(in) :: exp
    real(kind=4), dimension(:,:,:), intent(in) :: act
    character(len=*), intent(in) :: exp_str
    character(len=*), intent(in) :: act_str
    type(TestResult), intent(inout) :: res
    integer, intent(in) :: line
    character(len=*), intent(in) :: file

    character(len=MSG_LEN) :: msg
    integer(kind=C_SIZE_T) :: msg_len

    passed = .true.

    if (any(exp .ne. act)) then
      write (msg,'(5a,g12.3,a,g12.3,3a,i0,a)') "Assertion { ", exp_str , " == ", act_str, &
      &             " } failed for value { ", exp(1,1,1), "... == ", act(1,1,1), "... } (", file, ":", line, ")"
      msg_len = len_trim(msg)

      call append_message(res%messages, res%num_messages, msg, msg_len)

      res%passed = 0
      passed = .false.
    end if
  end function assertEqual_real4_3d_real4_3d


  logical function assertEqual_real8_1d_real8_1d(exp, act, exp_str, act_str, res, line, file) result(passed)
    real(kind=8), dimension(:), intent(in) :: exp
    real(kind=8), dimension(:), intent(in) :: act
    character(len=*), intent(in) :: exp_str
    character(len=*), intent(in) :: act_str
    type(TestResult), intent(inout) :: res
    integer, intent(in) :: line
    character(len=*), intent(in) :: file

    character(len=MSG_LEN) :: msg
    integer(kind=C_SIZE_T) :: msg_len

    passed = .true.

    if (any(exp .ne. act)) then
      write (msg,'(5a,g12.3,a,g12.3,3a,i0,a)') "Assertion { ", exp_str , " == ", act_str, &
      &             " } failed for value { ", exp(1), "... == ", act(1), "... } (", file, ":", line, ")"
      msg_len = len_trim(msg)

      call append_message(res%messages, res%num_messages, msg, msg_len)

      res%passed = 0
      passed = .false.
    end if
  end function assertEqual_real8_1d_real8_1d

  logical function assertEqual_real8_2d_real8_2d(exp, act, exp_str, act_str, res, line, file) result(passed)
    real(kind=8), dimension(:,:), intent(in) :: exp
    real(kind=8), dimension(:,:), intent(in) :: act
    character(len=*), intent(in) :: exp_str
    character(len=*), intent(in) :: act_str
    type(TestResult), intent(inout) :: res
    integer, intent(in) :: line
    character(len=*), intent(in) :: file

    character(len=MSG_LEN) :: msg
    integer(kind=C_SIZE_T) :: msg_len

    passed = .true.

    if (any(exp .ne. act)) then
      write (msg,'(5a,g12.3,a,g12.3,3a,i0,a)') "Assertion { ", exp_str , " == ", act_str, &
      &             " } failed for value { ", exp(1,1), "... == ", act(1,1), "... } (", file, ":", line, ")"
      msg_len = len_trim(msg)

      call append_message(res%messages, res%num_messages, msg, msg_len)

      res%passed = 0
      passed = .false.
    end if
  end function assertEqual_real8_2d_real8_2d

  logical function assertEqual_real8_3d_real8_3d(exp, act, exp_str, act_str, res, line, file) result(passed)
    real(kind=8), dimension(:,:,:), intent(in) :: exp
    real(kind=8), dimension(:,:,:), intent(in) :: act
    character(len=*), intent(in) :: exp_str
    character(len=*), intent(in) :: act_str
    type(TestResult), intent(inout) :: res
    integer, intent(in) :: line
    character(len=*), intent(in) :: file

    character(len=MSG_LEN) :: msg
    integer(kind=C_SIZE_T) :: msg_len

    passed = .true.

    if (any(exp .ne. act)) then
      write (msg,'(5a,g12.3,a,g12.3,3a,i0,a)') "Assertion { ", exp_str , " == ", act_str, &
      &             " } failed for value { ", exp(1,1,1), "... == ", act(1,1,1), "... } (", file, ":", line, ")"
      msg_len = len_trim(msg)

      call append_message(res%messages, res%num_messages, msg, msg_len)

      res%passed = 0
      passed = .false.
    end if
  end function assertEqual_real8_3d_real8_3d


  logical function assertEqual_string_1d_string_1d(exp, act, exp_str, act_str, res, line, file) result(passed)
    character(len=*), dimension(:), intent(in) :: exp
    character(len=*), dimension(:), intent(in) :: act
    character(len=*), intent(in) :: exp_str
    character(len=*), intent(in) :: act_str
    type(TestResult), intent(inout) :: res
    integer, intent(in) :: line
    character(len=*), intent(in) :: file

    character(len=MSG_LEN) :: msg
    integer(kind=C_SIZE_T) :: msg_len

    passed = .true.

    if (any(exp .ne. act)) then
      write (msg,'(5a,a,a,a,3a,i0,a)') "Assertion { ", exp_str , " == ", act_str, &
      &             " } failed for value { ", exp(1), "... == ", act(1), "... } (", file, ":", line, ")"
      msg_len = len_trim(msg)

      call append_message(res%messages, res%num_messages, msg, msg_len)

      res%passed = 0
      passed = .false.
    end if
  end function assertEqual_string_1d_string_1d

  logical function assertEqual_string_2d_string_2d(exp, act, exp_str, act_str, res, line, file) result(passed)
    character(len=*), dimension(:,:), intent(in) :: exp
    character(len=*), dimension(:,:), intent(in) :: act
    character(len=*), intent(in) :: exp_str
    character(len=*), intent(in) :: act_str
    type(TestResult), intent(inout) :: res
    integer, intent(in) :: line
    character(len=*), intent(in) :: file

    character(len=MSG_LEN) :: msg
    integer(kind=C_SIZE_T) :: msg_len

    passed = .true.

    if (any(exp .ne. act)) then
      write (msg,'(5a,a,a,a,3a,i0,a)') "Assertion { ", exp_str , " == ", act_str, &
      &             " } failed for value { ", exp(1,1), "... == ", act(1,1), "... } (", file, ":", line, ")"
      msg_len = len_trim(msg)

      call append_message(res%messages, res%num_messages, msg, msg_len)

      res%passed = 0
      passed = .false.
    end if
  end function assertEqual_string_2d_string_2d

  logical function assertEqual_string_3d_string_3d(exp, act, exp_str, act_str, res, line, file) result(passed)
    character(len=*), dimension(:,:,:), intent(in) :: exp
    character(len=*), dimension(:,:,:), intent(in) :: act
    character(len=*), intent(in) :: exp_str
    character(len=*), intent(in) :: act_str
    type(TestResult), intent(inout) :: res
    integer, intent(in) :: line
    character(len=*), intent(in) :: file

    character(len=MSG_LEN) :: msg
    integer(kind=C_SIZE_T) :: msg_len

    passed = .true.

    if (any(exp .ne. act)) then
      write (msg,'(5a,a,a,a,3a,i0,a)') "Assertion { ", exp_str , " == ", act_str, &
      &             " } failed for value { ", exp(1,1,1), "... == ", act(1,1,1), "... } (", file, ":", line, ")"
      msg_len = len_trim(msg)

      call append_message(res%messages, res%num_messages, msg, msg_len)

      res%passed = 0
      passed = .false.
    end if
  end function assertEqual_string_3d_string_3d




  logical function assertClose_real4_1d_real4_1d_tol4(exp, act, tol, exp_str, act_str, res, line, file) result(passed)
    real(kind=4), dimension(:), intent(in) :: exp
    real(kind=4), dimension(:), intent(in) :: act
    real(kind=4), intent(in) :: tol
    character(len=*), intent(in) :: exp_str
    character(len=*), intent(in) :: act_str
    type(TestResult), intent(inout) :: res
    integer, intent(in) :: line
    character(len=*), intent(in) :: file

    character(len=MSG_LEN) :: msg
    integer(kind=C_SIZE_T) :: msg_len

    passed = .true.

    if (any((exp - act) > tol)) then
      write (msg,'(5a,g12.3,a,g12.3,3a,i0,a)') "Assertion { ", exp_str , " == ", act_str, &
      &             " } failed for value { ", exp(1), "... == ", act(1), "... } (", file, ":", line, ")"
      msg_len = len_trim(msg)

      call append_message(res%messages, res%num_messages, msg, msg_len)

      res%passed = 0
      passed = .false.
    end if
  end function assertClose_real4_1d_real4_1d_tol4

  logical function assertClose_real4_2d_real4_2d_tol4(exp, act, tol, exp_str, act_str, res, line, file) result(passed)
    real(kind=4), dimension(:,:), intent(in) :: exp
    real(kind=4), dimension(:,:), intent(in) :: act
    real(kind=4), intent(in) :: tol
    character(len=*), intent(in) :: exp_str
    character(len=*), intent(in) :: act_str
    type(TestResult), intent(inout) :: res
    integer, intent(in) :: line
    character(len=*), intent(in) :: file

    character(len=MSG_LEN) :: msg
    integer(kind=C_SIZE_T) :: msg_len

    passed = .true.

    if (any((exp - act) > tol)) then
      write (msg,'(5a,g12.3,a,g12.3,3a,i0,a)') "Assertion { ", exp_str , " == ", act_str, &
      &             " } failed for value { ", exp(1,1), "... == ", act(1,1), "... } (", file, ":", line, ")"
      msg_len = len_trim(msg)

      call append_message(res%messages, res%num_messages, msg, msg_len)

      res%passed = 0
      passed = .false.
    end if
  end function assertClose_real4_2d_real4_2d_tol4

  logical function assertClose_real4_3d_real4_3d_tol4(exp, act, tol, exp_str, act_str, res, line, file) result(passed)
    real(kind=4), dimension(:,:,:), intent(in) :: exp
    real(kind=4), dimension(:,:,:), intent(in) :: act
    real(kind=4), intent(in) :: tol
    character(len=*), intent(in) :: exp_str
    character(len=*), intent(in) :: act_str
    type(TestResult), intent(inout) :: res
    integer, intent(in) :: line
    character(len=*), intent(in) :: file

    character(len=MSG_LEN) :: msg
    integer(kind=C_SIZE_T) :: msg_len

    passed = .true.

    if (any((exp - act) > tol)) then
      write (msg,'(5a,g12.3,a,g12.3,3a,i0,a)') "Assertion { ", exp_str , " == ", act_str, &
      &             " } failed for value { ", exp(1,1,1), "... == ", act(1,1,1), "... } (", file, ":", line, ")"
      msg_len = len_trim(msg)

      call append_message(res%messages, res%num_messages, msg, msg_len)

      res%passed = 0
      passed = .false.
    end if
  end function assertClose_real4_3d_real4_3d_tol4


  logical function assertClose_real4_1d_real4_1d_tol8(exp, act, tol, exp_str, act_str, res, line, file) result(passed)
    real(kind=4), dimension(:), intent(in) :: exp
    real(kind=4), dimension(:), intent(in) :: act
    real(kind=8), intent(in) :: tol
    character(len=*), intent(in) :: exp_str
    character(len=*), intent(in) :: act_str
    type(TestResult), intent(inout) :: res
    integer, intent(in) :: line
    character(len=*), intent(in) :: file

    character(len=MSG_LEN) :: msg
    integer(kind=C_SIZE_T) :: msg_len

    passed = .true.

    if (any((exp - act) > tol)) then
      write (msg,'(5a,g12.3,a,g12.3,3a,i0,a)') "Assertion { ", exp_str , " == ", act_str, &
      &             " } failed for value { ", exp(1), "... == ", act(1), "... } (", file, ":", line, ")"
      msg_len = len_trim(msg)

      call append_message(res%messages, res%num_messages, msg, msg_len)

      res%passed = 0
      passed = .false.
    end if
  end function assertClose_real4_1d_real4_1d_tol8

  logical function assertClose_real4_2d_real4_2d_tol8(exp, act, tol, exp_str, act_str, res, line, file) result(passed)
    real(kind=4), dimension(:,:), intent(in) :: exp
    real(kind=4), dimension(:,:), intent(in) :: act
    real(kind=8), intent(in) :: tol
    character(len=*), intent(in) :: exp_str
    character(len=*), intent(in) :: act_str
    type(TestResult), intent(inout) :: res
    integer, intent(in) :: line
    character(len=*), intent(in) :: file

    character(len=MSG_LEN) :: msg
    integer(kind=C_SIZE_T) :: msg_len

    passed = .true.

    if (any((exp - act) > tol)) then
      write (msg,'(5a,g12.3,a,g12.3,3a,i0,a)') "Assertion { ", exp_str , " == ", act_str, &
      &             " } failed for value { ", exp(1,1), "... == ", act(1,1), "... } (", file, ":", line, ")"
      msg_len = len_trim(msg)

      call append_message(res%messages, res%num_messages, msg, msg_len)

      res%passed = 0
      passed = .false.
    end if
  end function assertClose_real4_2d_real4_2d_tol8

  logical function assertClose_real4_3d_real4_3d_tol8(exp, act, tol, exp_str, act_str, res, line, file) result(passed)
    real(kind=4), dimension(:,:,:), intent(in) :: exp
    real(kind=4), dimension(:,:,:), intent(in) :: act
    real(kind=8), intent(in) :: tol
    character(len=*), intent(in) :: exp_str
    character(len=*), intent(in) :: act_str
    type(TestResult), intent(inout) :: res
    integer, intent(in) :: line
    character(len=*), intent(in) :: file

    character(len=MSG_LEN) :: msg
    integer(kind=C_SIZE_T) :: msg_len

    passed = .true.

    if (any((exp - act) > tol)) then
      write (msg,'(5a,g12.3,a,g12.3,3a,i0,a)') "Assertion { ", exp_str , " == ", act_str, &
      &             " } failed for value { ", exp(1,1,1), "... == ", act(1,1,1), "... } (", file, ":", line, ")"
      msg_len = len_trim(msg)

      call append_message(res%messages, res%num_messages, msg, msg_len)

      res%passed = 0
      passed = .false.
    end if
  end function assertClose_real4_3d_real4_3d_tol8


  logical function assertClose_real8_1d_real8_1d_tol4(exp, act, tol, exp_str, act_str, res, line, file) result(passed)
    real(kind=8), dimension(:), intent(in) :: exp
    real(kind=8), dimension(:), intent(in) :: act
    real(kind=4), intent(in) :: tol
    character(len=*), intent(in) :: exp_str
    character(len=*), intent(in) :: act_str
    type(TestResult), intent(inout) :: res
    integer, intent(in) :: line
    character(len=*), intent(in) :: file

    character(len=MSG_LEN) :: msg
    integer(kind=C_SIZE_T) :: msg_len

    passed = .true.

    if (any((exp - act) > tol)) then
      write (msg,'(5a,g12.3,a,g12.3,3a,i0,a)') "Assertion { ", exp_str , " == ", act_str, &
      &             " } failed for value { ", exp(1), "... == ", act(1), "... } (", file, ":", line, ")"
      msg_len = len_trim(msg)

      call append_message(res%messages, res%num_messages, msg, msg_len)

      res%passed = 0
      passed = .false.
    end if
  end function assertClose_real8_1d_real8_1d_tol4

  logical function assertClose_real8_2d_real8_2d_tol4(exp, act, tol, exp_str, act_str, res, line, file) result(passed)
    real(kind=8), dimension(:,:), intent(in) :: exp
    real(kind=8), dimension(:,:), intent(in) :: act
    real(kind=4), intent(in) :: tol
    character(len=*), intent(in) :: exp_str
    character(len=*), intent(in) :: act_str
    type(TestResult), intent(inout) :: res
    integer, intent(in) :: line
    character(len=*), intent(in) :: file

    character(len=MSG_LEN) :: msg
    integer(kind=C_SIZE_T) :: msg_len

    passed = .true.

    if (any((exp - act) > tol)) then
      write (msg,'(5a,g12.3,a,g12.3,3a,i0,a)') "Assertion { ", exp_str , " == ", act_str, &
      &             " } failed for value { ", exp(1,1), "... == ", act(1,1), "... } (", file, ":", line, ")"
      msg_len = len_trim(msg)

      call append_message(res%messages, res%num_messages, msg, msg_len)

      res%passed = 0
      passed = .false.
    end if
  end function assertClose_real8_2d_real8_2d_tol4

  logical function assertClose_real8_3d_real8_3d_tol4(exp, act, tol, exp_str, act_str, res, line, file) result(passed)
    real(kind=8), dimension(:,:,:), intent(in) :: exp
    real(kind=8), dimension(:,:,:), intent(in) :: act
    real(kind=4), intent(in) :: tol
    character(len=*), intent(in) :: exp_str
    character(len=*), intent(in) :: act_str
    type(TestResult), intent(inout) :: res
    integer, intent(in) :: line
    character(len=*), intent(in) :: file

    character(len=MSG_LEN) :: msg
    integer(kind=C_SIZE_T) :: msg_len

    passed = .true.

    if (any((exp - act) > tol)) then
      write (msg,'(5a,g12.3,a,g12.3,3a,i0,a)') "Assertion { ", exp_str , " == ", act_str, &
      &             " } failed for value { ", exp(1,1,1), "... == ", act(1,1,1), "... } (", file, ":", line, ")"
      msg_len = len_trim(msg)

      call append_message(res%messages, res%num_messages, msg, msg_len)

      res%passed = 0
      passed = .false.
    end if
  end function assertClose_real8_3d_real8_3d_tol4


  logical function assertClose_real8_1d_real8_1d_tol8(exp, act, tol, exp_str, act_str, res, line, file) result(passed)
    real(kind=8), dimension(:), intent(in) :: exp
    real(kind=8), dimension(:), intent(in) :: act
    real(kind=8), intent(in) :: tol
    character(len=*), intent(in) :: exp_str
    character(len=*), intent(in) :: act_str
    type(TestResult), intent(inout) :: res
    integer, intent(in) :: line
    character(len=*), intent(in) :: file

    character(len=MSG_LEN) :: msg
    integer(kind=C_SIZE_T) :: msg_len

    passed = .true.

    if (any((exp - act) > tol)) then
      write (msg,'(5a,g12.3,a,g12.3,3a,i0,a)') "Assertion { ", exp_str , " == ", act_str, &
      &             " } failed for value { ", exp(1), "... == ", act(1), "... } (", file, ":", line, ")"
      msg_len = len_trim(msg)

      call append_message(res%messages, res%num_messages, msg, msg_len)

      res%passed = 0
      passed = .false.
    end if
  end function assertClose_real8_1d_real8_1d_tol8

  logical function assertClose_real8_2d_real8_2d_tol8(exp, act, tol, exp_str, act_str, res, line, file) result(passed)
    real(kind=8), dimension(:,:), intent(in) :: exp
    real(kind=8), dimension(:,:), intent(in) :: act
    real(kind=8), intent(in) :: tol
    character(len=*), intent(in) :: exp_str
    character(len=*), intent(in) :: act_str
    type(TestResult), intent(inout) :: res
    integer, intent(in) :: line
    character(len=*), intent(in) :: file

    character(len=MSG_LEN) :: msg
    integer(kind=C_SIZE_T) :: msg_len

    passed = .true.

    if (any((exp - act) > tol)) then
      write (msg,'(5a,g12.3,a,g12.3,3a,i0,a)') "Assertion { ", exp_str , " == ", act_str, &
      &             " } failed for value { ", exp(1,1), "... == ", act(1,1), "... } (", file, ":", line, ")"
      msg_len = len_trim(msg)

      call append_message(res%messages, res%num_messages, msg, msg_len)

      res%passed = 0
      passed = .false.
    end if
  end function assertClose_real8_2d_real8_2d_tol8

  logical function assertClose_real8_3d_real8_3d_tol8(exp, act, tol, exp_str, act_str, res, line, file) result(passed)
    real(kind=8), dimension(:,:,:), intent(in) :: exp
    real(kind=8), dimension(:,:,:), intent(in) :: act
    real(kind=8), intent(in) :: tol
    character(len=*), intent(in) :: exp_str
    character(len=*), intent(in) :: act_str
    type(TestResult), intent(inout) :: res
    integer, intent(in) :: line
    character(len=*), intent(in) :: file

    character(len=MSG_LEN) :: msg
    integer(kind=C_SIZE_T) :: msg_len

    passed = .true.

    if (any((exp - act) > tol)) then
      write (msg,'(5a,g12.3,a,g12.3,3a,i0,a)') "Assertion { ", exp_str , " == ", act_str, &
      &             " } failed for value { ", exp(1,1,1), "... == ", act(1,1,1), "... } (", file, ":", line, ")"
      msg_len = len_trim(msg)

      call append_message(res%messages, res%num_messages, msg, msg_len)

      res%passed = 0
      passed = .false.
    end if
  end function assertClose_real8_3d_real8_3d_tol8

end module tunit
