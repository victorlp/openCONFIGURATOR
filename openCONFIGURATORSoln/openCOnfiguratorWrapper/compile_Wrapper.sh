##g++ -shared Tcl_WrapperMain.cpp Tcl_WrapperMain_wrap.cxx ../SharedLibrary/openCONFIGURATORSoln/openCONFIGURATOR/SharedLibrary/openCONFIGURATOR.so -o Tcl_WrapperMain.so

##g++ -shared -isystem /usr/include/tcl8.4 `xml2-config --cflags` Tcl_WrapperMain.cpp Tcl_WrapperMain_wrap.cxx ./openCONFIGURATOR.so -o Tcl_WrapperMain.so `xml2-config --libs`

#g++ -fPIC -shared -isystem /usr/include/tcl8.4 `xml2-config --cflags` openconfiguratorWrapper_wrap.cxx /usr/lib/openCONFIGURATOR.so -o openConfiguratorWrapper.so `xml2-config --libs`

make compile
cp -pv *.so ./../../../../Application/Src/openCONFIGURATOR/
sudo cp -pv /usr/lib/openCONFIGURATOR.so ./../../../../Application/Src/openCONFIGURATOR/
