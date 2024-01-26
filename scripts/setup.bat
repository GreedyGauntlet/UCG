@echo off
PUSHD ..
PUSHD engine
git clone git@github.com:JHeflinger/Flora.git -b linux-sdk --recursive
PUSHD Flora
ren "WORKSPACE.bazel" "dep_WORKSPACE.bazel"
PUSHD Flora
ren "BUILD.bazel" "dep_BUILD.bazel"
POPD
POPD
POPD
POPD
PAUSE