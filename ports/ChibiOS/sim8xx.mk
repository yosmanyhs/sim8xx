# sim8xx files.
SIMSRC = $(SIM8XX)/source/Sim8xx.c \
         $(SIM8XX)/source/Buffer/Buffer.c \
         $(SIM8XX)/source/Common/AtCommand.c \
         $(SIM8XX)/source/Modem/Modem.c \
         $(SIM8XX)/source/Modem/Commands/at.c \
         $(SIM8XX)/source/Modem/Commands/ate.c \
         $(SIM8XX)/source/Modules/Bluetooth/Bluetooth.c \
         $(SIM8XX)/source/Modules/Bluetooth/Commands/btacpt.c \
         $(SIM8XX)/source/Modules/Bluetooth/Commands/btconnect.c \
         $(SIM8XX)/source/Modules/Bluetooth/Commands/btdisconn.c \
         $(SIM8XX)/source/Modules/Bluetooth/Commands/bthost.c \
         $(SIM8XX)/source/Modules/Bluetooth/Commands/btpaircfg.c \
         $(SIM8XX)/source/Modules/Bluetooth/Commands/btpower.c \
         $(SIM8XX)/source/Modules/Bluetooth/Commands/btsppget.c \
         $(SIM8XX)/source/Modules/Bluetooth/Commands/btsppsend.c \
         $(SIM8XX)/source/Modules/GPS.c \
         $(SIM8XX)/source/Modules/GPS/Commands/cgnsinf.c \
         $(SIM8XX)/source/Modules/GPS/Commands/cgnspwr.c \
         $(SIM8XX)/source/Utils/Utils.c \
         $(SIM8XX)/ports/ChibiOS/Os.c

SIMINC = $(SIM8XX)/source

# Shared variables
ALLCSRC += $(SIMSRC)
ALLINC  += $(SIMINC)