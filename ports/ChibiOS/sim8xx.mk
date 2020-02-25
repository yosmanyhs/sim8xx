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
         $(SIM8XX)/source/Modules/Bluetooth/Commands/btpair.c \
         $(SIM8XX)/source/Modules/Bluetooth/Commands/btpaircfg.c \
         $(SIM8XX)/source/Modules/Bluetooth/Commands/btpower.c \
         $(SIM8XX)/source/Modules/Bluetooth/Commands/btsppget.c \
         $(SIM8XX)/source/Modules/Bluetooth/Commands/btsppsend.c \
         $(SIM8XX)/source/Modules/Bluetooth/Commands/btsppurc.c \
         $(SIM8XX)/source/Modules/GSM/Gsm.c \
         $(SIM8XX)/source/Modules/GSM/Call.c \
         $(SIM8XX)/source/Modules/GSM/Sms.c \
         $(SIM8XX)/source/Modules/GSM/Gprs.c \
         $(SIM8XX)/source/Modules/GSM/Commands/cfun.c \
         $(SIM8XX)/source/Modules/GSM/Commands/cifsr.c \
         $(SIM8XX)/source/Modules/GSM/Commands/ciicr.c \
         $(SIM8XX)/source/Modules/GSM/Commands/cipclose.c \
         $(SIM8XX)/source/Modules/GSM/Commands/cipsend.c \
         $(SIM8XX)/source/Modules/GSM/Commands/cipshut.c \
         $(SIM8XX)/source/Modules/GSM/Commands/cipstart.c \
         $(SIM8XX)/source/Modules/GSM/Commands/cpin.c \
         $(SIM8XX)/source/Modules/GSM/Commands/cstt.c \
         $(SIM8XX)/source/Modules/GPS/Gps.c \
         $(SIM8XX)/source/Modules/GPS/Commands/cgnsinf.c \
         $(SIM8XX)/source/Modules/GPS/Commands/cgnspwr.c \
         $(SIM8XX)/source/Utils/Utils.c \
         $(SIM8XX)/ports/ChibiOS/Os.c

SIMINC = $(SIM8XX)/source

# Shared variables
ALLCSRC += $(SIMSRC)
ALLINC  += $(SIMINC)