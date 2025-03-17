# CSky: ADS-B decoder in Cback to basics

## Requirements
- [Crux](https://github.com/meslab/crux.git)
- `make`
- Standard build tools (`gcc`, `clang`, etc., depending on platform)

## Directory Structure
```
workspace/
├── crux/    # Dependency, must be built before CSky
└── csky/    # The main backend server
```

## Installation
1. Clone this repository and Crux:
   ```sh
   git clone https://github.com/your-repo/csky.git workspace/csky
   git clone https://github.com/meslab/crux.git workspace/crux
   ```

2. Build Crux:
   ```sh
   cd workspace/crux
   make
   ```

3. Build CSky:
   ```sh
   cd ../csky
   make
   ```

## Usage
Once both Crux and CSky are built, you can run the ADS-B decoder:
```sh
./bin/csky_static -t <ip.ad.re.ss>:<port>
```

## License
This project is licensed under the MIT License.

## Contributing
Feel free to open issues or submit pull requests for improvements.


