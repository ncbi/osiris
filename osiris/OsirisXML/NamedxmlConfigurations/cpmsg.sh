#!/bin/sh
cd `dirname $0`
COPY="cp -puv"
SRC="../../MessageBook/OsirisMessageBookV4.0.xml"
DEST="MessageBookV4.0.xml"
${COPY} ${SRC} ${DEST}
for x in \
  Cofiler \
  GF \
  GFHID \
  ID \
  ID_HID \
  IDNO250 \
  IDNO250_HID \
  IDplus \
  IDplusNO250 \
  NGM \
  NGMSElect \
  PP12 \
  PP16 \
  PP16_HID \
  PP18D \
  PP21 \
  PPFusion \
  PPFusion_HID \
  PPY \
  PPY23 \
  PPY23_HID \
  Profiler \
  SEfilerPlus \
  SGMPlus \
  Yfiler ; do

  echo ${x}
  ${COPY} ${SRC} ${x}/${x}_${DEST}
done
