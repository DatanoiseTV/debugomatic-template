# debugomatic-template
A project template for the Debug-o-Matic using the Raspberry Pico SDK

## Building

You will need the pico-sdk. Extract it to e.g. ~/dev/pico-sdk

```
$ export PICO_SDK_PATH=~/dev/pico-sdk
$ cd build
$ cmake ..
$ make

```

This will give you an *.uf2 file, which you can copy to the RP2_BOOT media.
