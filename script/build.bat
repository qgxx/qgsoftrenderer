mkdir build
pushd build
rm -rf *
cmake -G "Visual Studio 17 2022" -Thost=x64 ..
cmake --build . --config release
popd
copy build\test\Release build\Release
