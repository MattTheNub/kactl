# find keycode
xev | grep keycode

# view key names
less /usr/include/X11/keysymdef.h

# remap key
xmodmap -e "keycode <number> = <name>"

# example
xmodmap -e "keycode 94 = Shift_L"
