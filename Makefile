CC=cc

all: kbd_backlight

kbd_backlight: kbd_backlight.c
	$(CC) -o kbd_backlight kbd_backlight.c
