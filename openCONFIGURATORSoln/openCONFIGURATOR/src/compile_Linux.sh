rm ./openCONFIGURATOR.so
rm -v /usr/lib/openCONFIGURATOR.so
#g++ -shared -o ../SharedLibrary/openCONFIGURATOR.so `xml2-config --cflags` *.cpp `xml2-config --libs`
g++ -shared -o ./openCONFIGURATOR.so `xml2-config --cflags` *.cpp `xml2-config --libs`
cp -pv *.so /usr/lib
