#!/bin/bash
EXAMPLE="$1"
TARGET="$2"
TOOLCHAIN="$3"

if [[ "$EXAMPLE" == "help" || -z "$EXAMPLE" ]]; then
echo "Usage: compile.sh <example> <target> <toolchain>"
exit 1
fi

echo "Building mbed example $EXAMPLE for target $TARGET using toolchain $TOOLCHAIN"

cp -r Notecard /mbed-ci/
cp -r src/Notecard/* /mbed-ci/Notecard
cp -r $EXAMPLE/* /mbed-ci

pushd /mbed-ci
mbed compile --target=$TARGET --toolchain=$TOOLCHAIN
result=$?
popd
exit $result
