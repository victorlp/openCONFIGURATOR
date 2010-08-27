####################################################################################################
#
#
#  NAME:     Makefile
#
#  PURPOSE:  Used to produce Tcl_WrapperMain.so files for the wrapper
#
#  AUTHOR:   Kalycito Powerlink Team
#
#  Copyright :(c) Kalycito Infotech Private Limited
#
#***************************************************************************************************
#  COPYRIGHT NOTICE: 
#
#  Project:      openCONFIGURATOR 
#
#  License:
#
#    Redistribution and use in source and binary forms, with or without
#    modification, are permitted provided that the following conditions
#    are met:
#
#    1. Redistributions of source code must retain the above copyright
#       notice, this list of conditions and the following disclaimer.
#
#    2. Redistributions in binary form must reproduce the above copyright
#       notice, this list of conditions and the following disclaimer in the
#       documentation and/or other materials provided with the distribution.
#
#    3. Neither the name of Kalycito Infotech Private Limited nor the names of 
#       its contributors may be used to endorse or promote products derived
#       from this software without prior written permission. For written
#       permission, please contact info@kalycito.com.
#
#    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
#    "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
#    LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
#    FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
#    COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
#    INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
#    BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
#    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
#    CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
#    LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
#    ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
#    POSSIBILITY OF SUCH DAMAGE.
#
#    Severability Clause:
#
#        If a provision of this License is or becomes illegal, invalid or
#        unenforceable in any jurisdiction, that shall not affect:
#        1. the validity or enforceability in that jurisdiction of any other
#           provision of this License; or
#        2. the validity or enforceability in other jurisdictions of that or
#           any other provision of this License.
#
#***************************************************************************************************
#
#  REVISION HISTORY:
#  
####################################################################################################
TCL = tcl
SWG = swig
CPP = c++

CC = g++
SHARE = shared
SYSTEM = isystem
DIR = /usr/include/tcl8.4
XMLFLAG1 = `xml2-config --cflags`
XMLFLAG2 = `xml2-config --libs`
INPUT = openconfiguratorWrapper_wrap.cxx
SOURCE = /usr/lib/openCONFIGURATOR.so
TARGET = openConfiguratorWrapper.so
INPUT1 = openconfiguratorWrapper.i

$(TARGET):	$(INPUT)
		$(CC) -$(SHARE) -$(SYSTEM) $(DIR) $(XMLFLAG1) $(INPUT) $(SOURCE) -o $(TARGET) $(XMLFLAG2)
		@echo "Successfully created TCL So File"

$(INPUT): $(INPUT1)
		$(SWG) -$(TCL) -$(CPP) $(INPUT1)
		@echo "Created .cxx file"
		
		
.PHONY: clean

clean:
	rm -f *.cxx
	rm -f openConfiguratorWrapper.so

