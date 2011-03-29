include ../MODULE_CONFIG

CXX_CFLAGS 		= -ldl
CXX_LDFLAGS 	= -lboost_system
LIBRARIES 		= $(CXX_SYSLIBS)

MODULE_NAME		= SslAdmin
OBJECTS			= SslAdmin.o Server.o Session.o
OUTPUT			= $(MODULE_NAME).so

default: $(OUTPUT)
Debug: all
Release: all
all: default


$(OUTPUT): $(SRC_DIR)SslAdmin.o $(SRC_DIR)Server.o $(SRC_DIR)Session.o
	$(CXX_LINK) -o $(LIB_DIR)$@ $(OBJECTS) $(LIBRARIES)

update:
	git pull

cleanDebug: clean
cleanRelease: clean
clean:
	rm -f $(OBJECTS) $(LIB_DIR)$(OUTPUT)

cleanall: clean
