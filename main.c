#include "QR_Encode.h"
#include <stdio.h>
#include <stdint.h>
#include <ncurses.h>

void printblock(int bit)
{
	putchar(bit ? 'H' : ' ');
}

int get_bit(size_t pos, uint8_t const *data)
{
	uint8_t bitmask[] = { 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01 };
	ldiv_t res = ldiv(pos, 8);
	return data[res.quot] & bitmask[res.rem] ? 1 : 0;
}

int main(int argc, char *argv[])
{
	WINDOW *mainwin;

	/* 177 * 177 bits is maximum size */
	BYTE QR_m_data[3917];
	int QR_width, size;
	size_t x, y;
	FILE *output;
	BYTE *p;

	if (argc < 2)
	{
		fprintf(stderr, "Usage: %s [data]\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	mainwin = initscr();
	if (!mainwin)
	{
		fprintf(stderr, "Error initializing ncurses.\n");
		exit(EXIT_FAILURE);
	}

	start_color();
	use_default_colors();
	noecho();
	cbreak();

	init_pair(1, COLOR_BLACK, COLOR_WHITE);
	init_pair(2, COLOR_BLACK, COLOR_BLACK);
	init_pair(3, COLOR_BLUE, COLOR_GREEN);

	/* Max possible bits resolution 177 * 177 / 8 + 1 */
	p = QR_m_data;
	QR_width = EncodeData(3, 0, argv[1], 0, p);

	output = fopen("file.tmp","w");
	size = ((QR_width * QR_width) / 8) + (((QR_width * QR_width) % 8) ? 1 : 0);
	fwrite(QR_m_data, size, 1,output);
	fclose(output);

	/* Paint background */
	bkgd(COLOR_PAIR(1));

	/* Hide cursor */
	curs_set(0);
	for (y = 0; y < QR_width; ++y)
	{
		for (x = 0; x < QR_width; ++x)
		{
			color_set(get_bit((y * QR_width) + x, QR_m_data) ? 2 : 1, NULL);
			mvaddch(4 + x, 8 + y * 2, ' ');
			mvaddch(4 + x, 8 + y * 2 + 1, ' ');
		}
	}

	refresh();
	getch();

	delwin(mainwin);
	endwin();
	refresh();

	return EXIT_SUCCESS;
}

