OS = LINUX

PROG = knub_soft

TARGET = $(PROG)
FINAL_TARGET = $(TARGET)
CXX = g++
STRIP = strip
WXCONFIG = wx-config
CPPFLAGS = -O0 -g -Wall -Wno-strict-aliasing `$(WXCONFIG) --cppflags` -D$(OS)
LIBS = `$(WXCONFIG) --libs` $(shell pkg-config --libs liblo)

MAKEFLAGS = --jobs=4

OBJS = knub_soft.o

all: $(FINAL_TARGET)

$(PROG): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LIBS)

$(PROG).exe: $(PROG)
	cp $(PROG) $@ 
	-upx -q $@
	-signcode -spc $(KEY_SPC) -v $(KEY_PVK) -t $(KEY_TS) $@
	-cp_win32.sh $@

$(PROG).app: $(PROG) Info.plist
	mkdir -p $(PROG).app/Contents/MacO
	mkdir -p $(PROG).app/Contents/Resources/English.lproj
	cp Info.plist $(PROG).app/Contents/Info.plist
	/bin/echo -n 'APPL????' > $(PROG).app/Contents/PkgInfo
	cp $(PROG) $(PROG).app/Contents/MacOS/
	touch $(PROG).app

$(PROG).dmg: $(PROG).app
	mkdir dmg_tmpdir
	cp -r $(PROG).app dmg_tmpdir
	hdiutil create -ov -srcfolder dmg_tmpdir -format UDBZ -volname "Knub soft" $(PROG).dmg

clean:
	rm -f $(PROG) $(PROG).exe $(PROG).exe.bak $(PROG).dmg *.o
	rm -rf $(PROG).app dmg_tmpdir