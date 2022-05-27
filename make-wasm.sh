#!/bin/sh -e

./autogen.sh
export CXXFLAGS="-Oz -flto -s USE_BOOST_HEADERS=1"
export LDFLAGS="-s MALLOC=emmalloc -s EVAL_CTORS=1 -s FILESYSTEM=1 -s EXPORTED_RUNTIME_METHODS=callMain,FS -s INVOKE_RUN=0  -s MODULARIZE=1 -s EXPORT_NAME=createDssp -s SINGLE_FILE=0 -s ASSERTIONS=1 -s EXPORT_ES6=1 -s ENVIRONMENT=web --oformat js"
emconfigure ./configure --without-boost-date-time --without-boost-thread --without-boost-system --without-boost-unit-test-framework --without-boost-iostreams --without-boost-program-options --host=wasm32
emmake make -j
mv mkdssp mkdssp.js
