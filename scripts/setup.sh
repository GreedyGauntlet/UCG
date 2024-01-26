cd ../engine
git clone git@github.com:JHeflinger/Flora.git -b linux-sdk --recursive
cd Flora
mv "WORKSPACE.bazel" "dep_WORKSPACE.bazel"
cd Flora
mv "BUILD.bazel" "dep_BUILD.bazel"
cd ../../../scripts

