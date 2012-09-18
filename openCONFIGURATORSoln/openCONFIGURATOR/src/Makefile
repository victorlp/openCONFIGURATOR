####################################################################################################
#
#
#  NAME:     Makefile
#
#  PURPOSE:  Used to produce openCONFIGURATOR.so for the source files
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


GCC	=	g++ 
FLAGS	=	-fPIC -shared `xml2-config --cflags` `xml2-config --libs`  

#BUG FIX #42
LINKERFLAGS	=	-lxml -lm

INCLUDE	= 	../Include
TARGET	= 	openCONFIGURATOR.so



SOURCE 		=   APIOperations.cpp ApplicationProcess.cpp BaseIndex.cpp \
			ComplexDataType.cpp DataTypeCollection.cpp Error.cpp \
			Exception.cpp IndexCollection.cpp Index.cpp \
			NetworkManagement.cpp NodeCollection.cpp Node.cpp \
			ObjectDictionary.cpp ProcessImage.cpp ProjectSettings.cpp \
			SubIndex.cpp Utility.cpp Validation.cpp XdcOperations.cpp


OBJECTS		=   APIOperations.o ApplicationProcess.o BaseIndex.o \
			ComplexDataType.o DataTypeCollection.o Error.o \
			Exception.o IndexCollection.o Index.o \
			NetworkManagement.o NodeCollection.o Node.o \
			ObjectDictionary.o ProcessImage.o ProjectSettings.o \
			SubIndex.o Utility.o Validation.o XdcOperations.o


$(TARGET):	$(OBJECTS)
		$(GCC) $(FLAGS) -I $(INCLUDE) -o $(TARGET) $(OBJECTS)
		@echo 'Finished building target: $@'
	

%.o: %.cpp
		#BUG FIX #42
		$(GCC) $(FLAGS) -I $(INCLUDE) -c $< $(LINKERFLAGS)
		@echo 'Finished building: $<'

.PHONY:     clean

clean:
	rm -f *.o
	rm -f *.so







