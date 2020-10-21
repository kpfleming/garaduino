ARDMK_DIR=/home/kpfleming/git-personal/Arduino-Makefile
USER_LIB_PATH=/home/kpfleming/git-personal/arduino/libraries

ARDUINO_VERSION=10813

BOARD_TAG=mkrwifi1010
MONITOR_PORT=/dev/ttyACM0

ARDUINO_LIBS=SPI SD SDU Ethernet ArduinoBLE ArduinoOTA PubSubClient arduino-timer-cpp17

CPPFLAGS+=-DDEBUG

CXXFLAGS_STD=-std=gnu++17

include $(ARDMK_DIR)/Sam.mk

gitversion.hpp:
	@echo "#define GIT_VERSION \"$(shell git describe --long --tags --dirty)\"" > $@

.PHONY: gitversion.hpp
