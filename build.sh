#!/bin/bash
#设置环境
echo " "
echo "***Setting environment...***"
rm -rf out/arch/arm64/boot/Image.gz
export PATH=$PATH:~/aarch64-linux-android-4.9/bin
export CROSS_COMPILE=aarch64-linux-android-
export GCC_COLORS=auto
export ARCH=arm64
if [ ! -d "out" ];
then
	mkdir out
fi

#输入盘古内核版本号
printf "Please enter Pangu Kernel version number: "
read v
echo " "
echo "Setting EXTRAVERSION"
export EV=EXTRAVERSION=_Kirin980_Pangu_V$v

#构建内核
echo " "
echo "***Building default version kernel...***"
echo " "
make ARCH=arm64 O=out $EV Pangu_Kirin980_defconfig
make ARCH=arm64 O=out $EV -j64

#打包内核
if [ -f out/arch/arm64/boot/Image.gz ];
then
	echo "***Packing default version kernel...***"
	tools/mkbootimg --kernel out/arch/arm64/boot/Image.gz --base 0x0 --cmdline "loglevel=4 initcall_debug=n page_tracker=on ssbd=kernel ssbd=kernel printktimer=0xfff0a000,0x534,0x538 androidboot.selinux=enforcing unmovable_isolate1=2:192M,3:224M,4:256M buildvariant=user" --tags_offset 0x07A00000 --kernel_offset 0x00080000 --ramdisk_offset 0x07c00000 --header_version 1 --os_version 9 --os_patch_level 2020-09-05  --output PK_V"$v"_9.0_Kirin980.img
	tools/mkbootimg --kernel out/arch/arm64/boot/Image.gz --base 0x0 --cmdline "loglevel=4 initcall_debug=n page_tracker=on ssbd=kernel ssbd=kernel printktimer=0xfff0a000,0x534,0x538 androidboot.selinux=permissive unmovable_isolate1=2:192M,3:224M,4:256M buildvariant=user" --tags_offset 0x07A00000 --kernel_offset 0x00080000 --ramdisk_offset 0x07c00000 --header_version 1 --os_version 9 --os_patch_level 2020-09-05  --output PK_V"$v"_9.0_Kirin980_PM.img
	echo " "
	echo "***Success!...***"
	echo " "
else
	echo " "
	echo "***Failed!***"
	exit 0
fi

