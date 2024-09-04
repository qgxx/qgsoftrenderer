@echo off
git submodule update --init external\src\tbb
rm -rf external\build\tbb
mkdir external\build\tbb
pushd external\build\tbb
rm -rf *
cmake -DCMAKE_INSTALL_PREFIX=..\..\ -DCMAKE_INSTALL_RPATH=..\..\ -DCMAKE_BUILD_TYPE=RELEASE ..\..\src\tbb
cmake --build . --config release --target install
popd
echo "Completed build tbb."