#!/bin/sh

LIBPATH='lib/'
GEN_COMMON='lib/gen/'
TOOLS_PATH='tools/'

function run_codegen_precheck {
  if [ ! -f "tools/code/gen/codegen_auto" ]; then
    cd tools/code/gen/
    make
    cd ../../../
  fi
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

sh scripts/update_embeded.sh

echo 'All Done!'
