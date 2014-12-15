
#! /bin/sh

# simple awk use to generate some Fortran include file from C include file

echo $1 $2 $3

temp=tempfile.h

awk '/TIO_SUCCESS/ { 
  do {
    printf("integer, parameter :: %s_F = %d\n", $1, $3)
    getline
  }
  while ($1 != "TIO_ERR_INT")
  printf("integer, parameter :: %s_F = %d\n", $1, $3)
}' $1 >$temp


awk '{print "  " $0}' $temp > $2
awk '{print "  ! <DATA> " $0}' $temp > $3

rm $temp
