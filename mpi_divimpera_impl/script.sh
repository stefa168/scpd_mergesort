#/bin/bash

while [ $# -gt 0 ]; do
  case "$1" in
    --flags=*)
      flags="${1#*=}"
      ;;
    --code=*)
      code="${1#*=}"
      ;;
    --proc=*)
      proc="${1#*=}"
      ;;
    --seed=*)
      seed="${1#*=}"
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

  if [ -z $len ]; then
    len=16
  fi

  echo "code: \"$code\", flags: \"$flags\", procs: \"$proc\", len: \"$len\", seed: \"$seed\""

  rm a.out 2> /dev/null

  # g++ -O3 ../common/data_generation.cpp -o data.o
  # g++ -O3 ../common/merge_implementations.cpp -o merge.o
  # mpic++ -O3 data.o merge.o -std=c++11 $code     #compila
  set -x
  mpic++ $flags -O3 -std=c++11 $code

  if [ -f "a.out" ]; then
    mpirun -np $proc ./a.out $len $seed       # eseguo
  fi
else
  echo "No code"
fi
