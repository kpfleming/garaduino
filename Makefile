ARDMK_DIR=/home/kpfleming/git-personal/Arduino-Makefile
USER_LIB_PATH=/home/kpfleming/git-personal/arduino/libraries

ARDUINO_VERSION=10813

BOARD_TAG=mkrwifi1010
MONITOR_PORT=/dev/ttyACM0

ARDUINO_LIBS=SPI Ethernet ArduinoBLE ArduinoOTA PubSubClient arduino-timer-cpp17-4.4.1

#CPPFLAGS+=-DDEBUG

CXXFLAGS_STD=-std=gnu++17

ARDUINO_OTA_TARGET=garaduino.km6g.us
ARDUINO_OTA_USERNAME=arduino
ARDUINO_OTA_PASSWORD=nOB44lt3GMRGpx68

CPPFLAGS+=-DOTA_USERNAME=\"$(ARDUINO_OTA_USERNAME)\" -DOTA_PASSWORD=\"$(ARDUINO_OTA_PASSWORD)\"

include $(ARDMK_DIR)/Sam.mk

ifndef ARDUINO_OTA
    BUNDLED_ARDUINO_OTA_DIR := $(call dir_if_exists,$(ARDUINO_PACKAGE_DIR)/$(ARDMK_VENDOR)/tools/arduinoOTA)
    # Try Arduino support package first
    ifdef BUNDLED_ARDUINO_OTA_DIR
        ifndef ARDUINO_OTA_VER
            ARDUINO_OTA_VER := $(shell basename $(firstword $(wildcard $(BUNDLED_ARDUINO_OTA_DIR)/*)))
        endif
        ARDUINO_OTA = $(BUNDLED_ARDUINO_OTA_DIR)/$(ARDUINO_OTA_VER)/bin/arduinoOTA
        $(call show_config_variable,ARDUINO_OTA,[AUTODETECTED],(from ARDUINO_PACKAGE_DIR))
    else
        # Otherwise look on user path
        ARDUINO_OTA := $(shell which arduinoOTA 2>/dev/null)
        ifdef ARDUINO_OTA
            $(call show_config_variable,ARDUINO_OTA,[AUTODETECTED],(found in $$PATH))
        endif
    endif
else
    $(call show_config_variable,ARDUINO_OTA,[USER])
endif

ota: $(TARGET_BIN) verify_size
	$(ARDUINO_OTA) -address $(ARDUINO_OTA_TARGET) -port 65280 -username $(ARDUINO_OTA_USERNAME) -password $(ARDUINO_OTA_PASSWORD) -sketch $(TARGET_BIN) -upload /sketch -b

gitversion.hpp:
	@echo "#define GIT_VERSION \"$(shell git describe --long --tags --dirty)\"" > $@

.PHONY: gitversion.hpp
