cmd_/home/tung/Documents/usbEncrypt/usbDriver.mod := printf '%s\n'   usbDriver.o | awk '!x[$$0]++ { print("/home/tung/Documents/usbEncrypt/"$$0) }' > /home/tung/Documents/usbEncrypt/usbDriver.mod
