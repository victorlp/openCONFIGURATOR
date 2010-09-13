rm ./openCONFIGURATOR.so
sudo rm -v /usr/lib/openCONFIGURATOR.so
##g++ -shared -o ../SharedLibrary/openCONFIGURATOR.so `xml2-config --cflags` *.cpp `xml2-config --libs`
#g++ -ggdb -fPIC -shared -o ./openCONFIGURATOR.so `xml2-config --cflags` *.cpp `xml2-config --libs`
#cp -pv *.so /usr/lib
make
sudo cp -pvf ./openCONFIGURATOR.so /usr/lib/
sudo chmod -v +x /usr/lib/openCONFIGURATOR.so
