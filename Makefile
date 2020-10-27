############################################################
# Configuration settings in this section

ARDMK_DIR=<path>

ARDUINO_VERSION=10813

BOARD_TAG=<tag>
MONITOR_PORT=<port>

ARDUINO_LIBS=arduino-timer-cpp17-4.5.0

ARDUINO_OTA_TARGET=ip.ip.ip.ip-or-hostname
ARDUINO_OTA_PORT=65280
ARDUINO_OTA_USERNAME=arduino
ARDUINO_OTA_PASSWORD=password

SERIAL_DEBUG=
OPTIMIZATION_LEVEL=s

# End of configuration settings
############################################################

ARDUINO_LIBS+=SPI Ethernet ArduinoBLE ArduinoOTA PubSubClient

ifdef SERIAL_DEBUG
CPPFLAGS+=-DSERIAL_DEBUG
endif

CXXFLAGS_STD=-std=gnu++17

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
	$(ARDUINO_OTA) -address $(ARDUINO_OTA_TARGET) -port $(ARDUINO_OTA_PORT) -username $(ARDUINO_OTA_USERNAME) -password $(ARDUINO_OTA_PASSWORD) -sketch $(TARGET_BIN) -upload /sketch -b

$(OBJDIR)/garaduino.ino.o: gitversion.hpp

gitversion.hpp:
	@echo "#define GIT_VERSION \"$(shell git describe --long --tags)\"" > $@

clean::
	$(REMOVE) gitversion.hpp

.PHONY: gitversion.hpp
