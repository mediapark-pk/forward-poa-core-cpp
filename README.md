# Forwardblock core with proof of authority

## Directory structure

- `3rdparty/` contains the third party libs
- `config/` contains the code for reading config file
- `entities/` contain objects that represent database tables
- `network/` contains code that uses the underlying `p2p network`
- `p2p/` contains the p2p network code.
- `tests/` contain the unit tests
- `utils/` contains various things like time functions, encoding/decoding

## Dependencies

1. spdlog
2. Catch2
3. Sqlite3
4. Libmicrohttpd (for MAC and Linux)

## Build

Make sure you have `cmake` installed and it is available in the path.

First clone the repo:

```
git clone https://github.com/mediapark-pk/forward-poa-core-cpp.git
```

Build **Unix/Mac**:

```bash
mkdir build && cd build && cmake .. && make -j4
```

Build on **Windows**:

Currently only TDM-MinGW64 is supported. MSVC has not been tested but should work.

```bash
mkdir build && cd build
cmake -G"MinGW Makefiles" ..
mingw32-make
```

Run:

```bash
cd src
./netmain
```

**Note**: Make sure `config.ini` is present in `build/`. You can modify the config as necessary.
