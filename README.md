# Eurorack Development Kit 🎛️

Template repository for developing Eurorack modules

[![Build Status](https://travis-ci.com/cutelabnyc/euro-devkit.svg?branch=main)](https://travis-ci.com/cutelabnyc/euro-devkit)

## The Stack 📚

The repository uses PlatformIO to build and upload code, while also pulling our own 
version of the STM32 HAL library. 

- `lib/` contains submodules for each eurorack module's
code, as well as `cutesynth` which is our internal API for DSP and operational code.
- `include/` contains pin/GPIO mappings that can be added and configured for each module
- `src/` has all the main hardware code according to development on STM32 chips

## The Architecture 🏗️

In order to prototype with this repo, the following hardware is necessary

- **Chip**: STM32F7xx
- **Board**: Nucleo-F767ZIT
- **Codec**: WM8731
- **ADC Pins**: (TODO)
- **GPIO Pins**: (TODO)

## Installation 🔮

Make sure to clone the repo recursively to download all the submodules

```
git clone --recursive https://github.com/cutelabnyc/euro-devkit.git
```

Install PlatformIO

```
# Linux
python3 -c "$(curl -fsSL https://raw.githubusercontent.com/platformio/platformio/master/scripts/get-platformio.py)"

# Mac OSX
brew install platformio
```

In the devkit folder, run the following to install the HAL layer

```
pio lib install
```

### Run with Docker 🐋

Alternatively, you can run the entire devkit in a Docker container...

```
docker build . -t cutelabnyc/euro-devkit
docker run --rm -it cutelabnyc/euro-devkit

# inside the container...
pio test && pio run
```

## Development 🍘

Before working on the repo, checkout a new branch for euro-devkit with the eurorack module's name in the title. Open another branch for [cutesynth](https://github.com/cutelabnyc/cutesynth) too, just incase you end up accidentally writing a Laplace transform and want to refactor it.

The environment for each module exists in `platform.ini`. You can upload code to the Nucleo using the following command

```
pio run -t upload -e <ENV_NAME>
```

where `<ENV_NAME>` is the name of the eurorack module you're working on.

## Testing 🌋

Automatic testing is handled on Travis CI upon pull requests. Local testing can be run with the following command

```
pio test -e <ENV_NAME>
```

## Forking 🍴

This repo can alternatively act as a sort of boiler-plate template for developing a new eurorack module. Simply fork the repo without each Eurorack submodule...

```
git clone https://github.com/cutelabnyc/euro-devkit.git
cd lib
git submodule init cutesynth
```
