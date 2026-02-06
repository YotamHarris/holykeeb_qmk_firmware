VIA_ENABLE = yes
OLED_ENABLE = yes
MOUSEKEY_ENABLE = yes
EXTRAKEY_ENABLE = yes
SERIAL_DRIVER = vendor

# Trackpoint on right half
POINTING_DEVICE_ENABLE = yes
POINTING_DEVICE_DRIVER = ps2
PS2_ENABLE = yes
PS2_DRIVER = vendor

OPT_DEFS += -DHK_POINTING_DEVICE_RIGHT_TRACKPOINT
OPT_DEFS += -DPOINTING_DEVICE_POSITION_RIGHT
OPT_DEFS += -DHK_MASTER_RIGHT

# HolyKeebs userspace source files
SRC += users/holykeebs/holykeebs.c
SRC += users/holykeebs/hk_debug.c
SRC += users/holykeebs/rpc.c
SRC += users/holykeebs/pimoroni.c
SRC += users/holykeebs/trackpoint.c
