#/bin/bash

while [ $# -gt 0 ]; do
  case "$1" in
    --code=*)
      code="${1#*=}"
      ;;
    --proc=*)
      proc="${1#*=}"
      ;;
    --in=*)
      in="${1#*=}"
      ;;
    --out=*)
      out="${1#*=}"
      ;;
    --len=*)
      len="${1#*=}"
      ;;
    *)
      printf "***************************\n"
      printf "* Error: Invalid argument.*\n"
      printf "***************************\n"
      exit 1
  esac
  shift
done


if [ ! -z $code ]; then

  if [ -z $proc ]; then
    proc=4
  fi

  if [ -z $in ]; then
    in="../File_to_order.txt"
  fi

  if [ -z $out ]; then
    out="output.txt"
  fi

  if [ -z $len ]; then
    len=16
  fi

  echo "code: $code, num_procs: $proc, in: $in, out: $out, len: $len"

  rm a.out
  mpic++ -std=c++11 $code     #compila
  mpirun -np $proc ./a.out $in $out $len        # eseguo

else
  echo "No code"
fi
