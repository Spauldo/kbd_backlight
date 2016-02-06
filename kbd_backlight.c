/*
 * Copyright (c) 2016 Jeff Spaulding <sarnet@gmail.com>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <err.h>

void
print_help()
{
	printf("Usage:\n");
	printf("kbd_backlight [-e | -d | -l<level>]\n");
	printf("  where <level> is a number between 0 and 100.\n");
}

int
main(int argc, char *argv[])
{
	int ch;
	int eflg, dflg, lflg;
	int level = -1;
	char level_str[4] = "";
	int fd;
	const char *kbd_file = "/sys/class/leds/kbd_backlight/brightness";

	eflg = dflg = lflg = 0;

	while ((ch = getopt(argc, argv, "edhl:")) != -1) {
		switch (ch) {
		case 'e':
			eflg = 1;
			if (dflg || lflg) {
				print_help();
				return EXIT_FAILURE;
			}

			level = 100;
			break;
		case 'd':
			dflg = 1;
			if (eflg || lflg) {
				print_help();
				return EXIT_FAILURE;
			}

			level = 0;
			break;
		case 'l':
			lflg = 1;
			if (dflg || eflg) {
				print_help();
				return EXIT_FAILURE;
			}

			level = atoi(optarg);
			if (level < 0 || level > 100) {
				print_help();
				return EXIT_FAILURE;
			}
			break;
		case '?':
		case 'h':
		default:
			print_help();
			return EXIT_FAILURE;
		}
	}

	if (level == -1) {
		print_help();
		return EXIT_FAILURE;
	} else {
		snprintf(level_str, sizeof level_str, "%d", level);
	}

	fd = open(kbd_file, O_TRUNC | O_WRONLY);

	if (fd == -1) {
		err(1, "%s", kbd_file);
	}

	write(fd, level_str, strlen(level_str));

	close(fd);
}
