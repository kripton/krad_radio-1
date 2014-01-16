#!/bin/sh

LIBPATH='lib/'
GEN_COMMON='lib/gen/'
TOOLS_PATH='tools/'

function update_embedded {
  if [ ! -f "tools/code/hextool/hextool" ]; then
    echo "hextool binary ain't existing mate"
    exit
  fi

  rm -f lib/krad_web/gen/embed.h
  cd lib/krad_web/core
  rm -f interface.js
  rm -f *.h
  ../../../tools/code/hextool/hextool api.js
  ../../../tools/code/hextool/hextool index.html
  ../../../tools/code/hextool/hextool dev_interface.js
  cat ../rack/*.js > interface.js
  ../../../tools/code/hextool/hextool interface.js
  cat *.h > ../gen/embed.h
  rm -f interface.js
  rm -f *.h
  cd ../../../
}

function run_codegen_precheck {
  cd tools/code/gen/bootstrap
  rm bootstrapped.c
  rm bootstrapped.h
  make
  cd ../
  make
  cd ../../../
}

function run_codegen {
  echo ./tools/code/gen/codegen_auto $1 $2 $3 $4
  eval tools/code/gen/codegen_auto $1 $2 $3 $4
  echo ./tools/code/gen/codegen_cfg $tools
  eval tools/code/gen/codegen_cfg $tools
}

path=$(realpath $LIBPATH)
pre="''"
suff="''"
gencommon=$(realpath $GEN_COMMON)
tools=$(realpath $TOOLS_PATH)
cgen_script_path="$(pwd)/scripts/codegen.sh"

if [ ! -f $cgen_script_path ]; then
  echo 'You are not running codegen.sh from KR root dir. Please do so.'
  exit
fi

run_codegen_precheck

for dir in $(ls -d $path/krad_*/); do
  prefix=$(basename $dir)
  localpath="${path}/${prefix}"
  gendir="${localpath}/gen"

  mkdir -p $gendir

  rm -rf $gendir/*.c
  rm -rf $gendir/*.h
done

rm -rf $gencommon/*.c
rm -rf $gencommon/*.h
mkdir -p $gencommon

run_codegen $path $pre $suff $gencommon

update_embedded

echo 'All Done!'
