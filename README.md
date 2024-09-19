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

# License
This project is licensed under the MIT License.
