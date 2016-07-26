#Set these variables if needed
C = gcc
CC = g++
FLAGS = -O3 -D_NOSQLITE -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64 -DGCC -Wno-unused-result -Wno-write-strings

#Paths to supporting software
MSTOOLKITPATH = ../MSToolkit

#Do not touch these variables
LIBPATH = -L$(MSTOOLKITPATH)
LIBS = -lSDL2 -lSDL2_ttf -lpng -lpthread
INCLUDE = -I$(MSTOOLKITPATH)/include -I./PepXMLParser -I./SDL_SavePNG


#Do not touch these variables
KSV = CAbout.o CButton.o CDisplay.o CCheckbox.o CDataSet.o CDropDown.o CEditBox.o CFileOpen.o CFilterManager.o CFont.o CFragmentLists.o CGraphic.o CGfxCollection.o CInput.o CList.o CPeptideBox.o CPlusMinus.o CResults.o CSettings.o CSlider.o CSortFilter.o CSpectrumGraph.o CTable.o CTextBox.o CToolbar.o CViewer.o

XML = PepXMLParser/CPepXMLPeptide.o PepXMLParser/CPepXMLPSM.o PepXMLParser/CPepXMLSpectrum.o PepXMLParser/PepXMLParser3.o

PNG = SDL_SavePNG/savepng.o

#Make statements
ksv : KojakSpectrumViewer.cpp $(XML) $(PNG) $(KSV)
	$(CC) $(FLAGS) $(INCLUDE) $(PNG) $(KSV) $(XML) $(MSTOOLKITPATH)/libmstoolkitlite.a KojakSpectrumViewer.cpp $(LIBS) -o KojakSpectrumViewer

clean:
	rm *.o SDL_SavePNG/*.o PepXMLParser/*.o KojakSpectrumViewer

SDL_SavePNG/%.o : SDL_SavePNG/%.c
	$(C) $(FLAGS) $(INCLUDE) $< -c -o $@

PepXMLParser/%.o : PepXMLParser/%.cpp
	$(CC) $(FLAGS) $(INCLUDE) $< -c -o $@

%.o : %.cpp
	$(CC) $(FLAGS) $(INCLUDE) $< -c

