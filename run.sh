#!/bin/bash
docker start TPEARQUI && docker exec -ti TPEARQUI bash -c "cd /root && make clean all" && sudo qemu-system-x86_64 -hda Image/x64BareBonesImage.qcow2 -m 512 
