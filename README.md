## simply rewrite of libev in cxx

event loop using epoll for linux, some c++ playground

do some experiments on
- event loop
- network io
- coroutine/goroutine
- memory allocation
- data structures

prerequisites:
- CMAKE 3.6
- unittest tool:https://github.com/google/googletest
- gcc supports c++11 std

## usage

```
git clone https://github.com/cfzjywxk/aev
cd aev
mkdir build
cd build
cmake ..
make
```
some simple unittest samples in unittest dir
