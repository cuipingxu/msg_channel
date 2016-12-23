
CC = g++ -DLINUX -D_REENTRANT
FLAGS = -W -g

BASE_ROOT=../../../../sdk/base/
JSON_ROOT=../../../../3rd_party/sdk/json-c/
TINY_XML_ROOT=../../../../3rd_party/sdk/tinyxml/
CURL_ROOT=../../../../3rd_party/sdk/curl/
OPENSSL_ROOT=../../../../3rd_party/sdk/openssl/

INCLUDES=-I./ -I../ -I$(BASE_ROOT)/include/ -I$(JSON_ROOT)/include/ \
-I$(TINY_XML_ROOT)/include/ -I$(CURL_ROOT)/include/ -I$(OPENSSL_ROOT)/include/

LIB_PATH=

SHARE_LIBS=$(BASE_ROOT)/lib/libbase.a \
$(JSON_ROOT)/lib/libjson-c.a $(TINY_XML_ROOT)/lib/libtinyxml.a \
$(CURL_ROOT)/lib/libcurl.a $(OPENSSL_ROOT)/lib/libssl.a $(OPENSSL_ROOT)/lib/libcrypto.a \
-lpthread -ldl -lrt -lz

TARGET=msg_channel.bin

OBJS = $(patsubst %.cpp,obj/%.o,$(wildcard *.cpp))

obj/%.o: %.h
obj/%.o: %.cpp
	$(CC) $(FLAGS) $(INCLUDES) -c $< -o $@

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(LIB_PATH) $(OBJS) $(SHARE_LIBS);
	cp $(TARGET)  ../../release/bin/$(TARGET)

clean:
	rm obj/*.o
	rm $(TARGET)

init:
	mkdir -p obj
	
