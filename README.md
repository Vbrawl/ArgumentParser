# Usage

```C
#include <ArgumentParser/argument_parser.h>

int main(int argc, const char* argv[]) {
  OWID owid_offset = OWID_USER_DEFINED;
  owid_offset = ArgumentParser_Init(owid_offset);
  // ...
}
```


# Build from source

## Linux

```bash
# Clone git
git clone https://github.com/Vbrawl/ArgumentParser.git
cd ArgumentParser

# Build
cmake -B build -DCMAKE_BUILD_TYPE=Release 
cd build
make

# Install
sudo make install
```

## Windows

```pwsh
# Clone git
git clone https://github.com/Vbrawl/ArgumentParser.git
cd ArgumentParser

# Build
cmake -B build -DOW_PREFIX="C:/Path/To/ObjectWrapper/installation/directory"
cd build
cmake --build . --config Release

# Install
cpack -G NSIS
# Run the generated executable
```

# License
This project is licensed under the MIT License.
