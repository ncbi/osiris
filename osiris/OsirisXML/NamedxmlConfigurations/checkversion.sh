cd $(dirname "$0")
echo "Checking version, user id and last update"
grep -E "<Version>|<UserID>|<LastUpdate>" */*LabSettings.xml
echo ""
echo ""
echo "Checking Notes"
grep -L "<Notes>This operating procedure is provided" */*LabSettings.xml
