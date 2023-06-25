#!/usr/bin/env bash

if [ ! -d "skia" ] ; then
    git clone https://skia.googlesource.com/skia.git skia
else
    pushd skia
        git reset --hard HEAD
        git checkout main
        git pull origin
    popd
fi

pushd skia > /dev/null
    python tools/git-sync-deps
    bin/gn gen out/Static --args="\
        is_debug = false \
        is_official_build = true \
        skia_use_system_libjpeg_turbo = false \
        skia_use_system_zlib = false \
        skia_use_system_harfbuzz = false \
        skia_use_system_libpng = false \
        skia_use_system_expat = false \
        skia_use_system_libwebp = false \
        skia_use_system_icu = false \
        skia_enable_ganesh = true \
        skia_enable_gpu = true \
        extra_cflags = [ \"-MDd\" ]"
    ninja -C out/Static
popd