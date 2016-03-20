#!/bin/sh
cd `dirname $0`
function CleanSetup()
{
 sed 's/.*= "8://' | sed 's/"//' | sort
}
function CleanPath()
{
sed 's|.*/||g' | sort
}
grep "TargetName.*_LadderInfo.xml" Setup.vdproj   | CleanSetup  > f1.txt
ls -1 ../OsirisXML/LadderSpecifications/*_LadderInfo.xml | CleanPath > f2.txt
diff f1.txt f2.txt
find ../OsirisXML/NamedxmlConfigurations -name \*_LabSettings.xml | CleanPath > f3.txt
grep "TargetName.*_LabSettings.xml" Setup.vdproj | CleanSetup > f4.txt
diff f3.txt f4.txt
grep '\\osiris\\' Setup.vdproj