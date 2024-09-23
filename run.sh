#!/bin/bash
if [ "$1" == "nosound" ]; then
    qemu-system-x86_64 -hda Image/x64BareBonesImage.qcow2 -m 512
else
    qemu-system-x86_64 -hda Image/x64BareBonesImage.qcow2 -m 512 -audiodev pa,id=speaker -machine pcspk-audiodev=speaker
fi