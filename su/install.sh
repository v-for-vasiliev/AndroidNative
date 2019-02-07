#!/bin/sh
adb root on
sleep 2
adb remount
sleep 2
# disabling OTA survival mode
adb shell '/data/local/bin/busybox chattr -i /system/xbin/su'
adb push libs/armeabi-v7a/su /system/xbin/su
adb shell 'chown root:root /system/xbin/su'
adb shell 'chmod 6755 /system/xbin/su'
#adb shell '/data/local/bin/busybox chattr +i /system/xbin/su'
