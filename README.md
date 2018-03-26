# Extrace
Enhanced tracing tool for Android derived from atrace

## Build
* Download Android source with patches defined in littlewhywhat/local_manifests
 Refer to http://source.android.com/source/downloading.html
 ```
 repo init -u https://android.googlesource.com/platform/manifest -b android-7.1.2_r19
 git clone -b extrace https://github.com/littlewhywhat/local_manifests .repo/local_manifests
 repo sync -j8
 ```
* add to device/brcm/rpi3/rpi3.mk lines:
```
PRODUCT_PACKAGES += \
    extrace
```

## Build for Raspberry Pi3
 https://github.com/android-rpi/device_brcm_rpi3

Use -j[n] option on sync & build steps, if build host has a good number of CPU cores.

Graphics HAL of this build : https://github.com/anholt/mesa/wiki/VC4

## Build and run unit tests

* on target
```
source build/envsetup.sh
lunch rpi3-eng
python ./development/testrunner/runtest.py -x frameworks/native/cmds/extrace/
```

* on host linux x86-64
```
make extrace_test_host
./out/host/linux-86/bin/extrace_test64
```
