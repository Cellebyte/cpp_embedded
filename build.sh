#! /usr/bin/env bash
#   author: Cellebyte alias Marcel Fest
# --> Fixed issue mentioned by @desyon
# ----> Script is returning Error Code <----
# ------> local build and test is also possible <------


# For building and testing the whole folder:
# -> INSTALL
#   sudo apt-get install build-essential software-properties-common -y && \
#   sudo add-apt-repository ppa:ubuntu-toolchain-r/test -y && \
#   sudo apt-get update && \
#   sudo apt-get install gcc-6 g++-6 -y && \
#   gcc -v
#
# -> Run ./build.sh


touch Makefile
touch config.mk

if [ -d ./bin ]
    then
        rm -rf ./bin
fi

COUNTER=1

echo 'CXX=g++-6' > config.mk
echo 'CXXFLAGS=-Werror -Wall -Wextra -fno-exceptions -Wno-format -fno-rtti -std=c++1y' >> config.mk
echo 'OUTPUT="./bin"' >> config.mk
echo 'SILENT_MKDIR=' >> config.mk


echo 'include config.mk' > Makefile
echo 'all: $(OUTPUT) build' >> Makefile
echo 'build:' >> Makefile

for i in $(ls -d */); do echo '	@$(CXX) $(CXXFLAGS) -o $(OUTPUT)/'"${i%%/}"' '"${i%%/}"'/src/*.cpp -I '"${i%%/}"'/include/' >> Makefile \
&& echo '	@echo' "$COUNTER"'. CC '"${i%%/}" >> Makefile \
&& (( COUNTER++ )) \
; done

echo 'start:' >> Makefile
echo '	$(SILENT_MKDIR)mkdir $(OUTPUT)' >> Makefile

make start
make build

for i in $(ls bin/); do ./bin/$i; done

if [ $? -eq 0 ]
then
    echo "Successfully build and tested!"
    exit 0
else
    echo "Error in build or test!!" >&2
    exit 1
fi

