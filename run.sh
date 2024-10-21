#!/bin/bash
sudo docker start TPEARQUI
sudo docker exec -it TPEARQUI make clean -C /root/Toolchain
sudo docker exec -it TPEARQUI make clean -C /root
sudo docker exec -it TPEARQUI make -C /root/Toolchain
sudo docker exec -it TPEARQUI make -C /root/
sudo docker stop TPEARQUI
sudo qemu-system-x86_64 -hda Image/x64BareBonesImage.qcow2 -m 512
