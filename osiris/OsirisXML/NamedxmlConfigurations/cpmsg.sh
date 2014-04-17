#!/bin/sh
cd `dirname $0`
COPY="cp -pv"
SRC="../../MessageBook/OsirisMessageBookV4.0.xml"
DEST="MessageBookV4.0.xml"
${COPY} ${SRC} ${DEST}
for x in `ls -Al | grep ^d | sed 's/.* //g'` ; do
  echo ${x}
  ${COPY} ${SRC} ${x}/${x}_${DEST}
done
