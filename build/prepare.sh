rm -rf ./submitty;
mkdir submitty;
cd submitty;
find ../../src | grep -e "\.[h,cpp]" | grep -wv "cache" | xargs -I {} cp {} .;
sed -i ".bak" "s/include \".*\/\(.*\)\"/include \"\1\"/" *;
rm *.bak;
