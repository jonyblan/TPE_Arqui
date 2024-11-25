#!/bin/bash
docker start TPEARQUI
docker exec -it TPEARQUI make clean -C /root/Toolchain
docker exec -it TPEARQUI make clean -C /root
docker exec -it TPEARQUI make -C /root/Toolchain
docker exec -it TPEARQUI make -C /root/
docker stop TPEARQUI
qemu-system-x86_64 -hda Image/x64BareBonesImage.qcow2 -m 512
