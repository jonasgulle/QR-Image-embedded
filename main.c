#include "QR_Encode.h"
#include <stdio.h>
#include <stdint.h>
#include <ncurses.h>

static WINDOW *mainwin;

inline int get_bit(size_t pos, uint8_t const *data)
{
	uint8_t bitmask[] = { 128, 64, 32, 16, 8, 4, 2, 1 };
	ldiv_t res = ldiv(pos, 8);
	return (data[res.quot] & bitmask[res.rem]) ? 1 : 0;
}

void init_colors(void)
{
	int i;
	int colors[4][3] = {
		{ 1, COLOR_BLACK, COLOR_WHITE },
		{ 2, COLOR_BLACK, COLOR_BLACK },
		{ 3, COLOR_BLUE, COLOR_GREEN },
		{ 4, COLOR_RED, COLOR_WHITE }
	};

	start_color();
	use_default_colors();
	noecho();
	cbreak();

	for (i = 0; i != 4; ++i)
		init_pair(colors[i][0], colors[i][1], colors[i][2]);
}

void init_ncurses(void)
{
	mainwin = initscr();
	if (!mainwin)
	{
		fprintf(stderr, "Error initializing ncurses.\n");
		exit(EXIT_FAILURE);
	}

	/* Paint background */
	bkgd(COLOR_PAIR(1));

	init_colors();

}

void exit_ncurses(void)
{
	delwin(mainwin);
	endwin();
	refresh();
}

void print_qr_code(uint8_t const *qr_data, size_t width)
{
	size_t x, y;

	/* Hide cursor */
	curs_set(0);

	for (y = 0; y < width; ++y)
	{
		for (x = 0; x < width; ++x)
		{
			int bit = get_bit(x * width + y, qr_data);
			color_set(bit ? 2 : 1, NULL);

			/* Print a 1x2 block */
			mvaddch(4 + x, 8 + y * 2, ' ');
			mvaddch(4 + x, 8 + y * 2 + 1, ' ');

			color_set(3, NULL);
			mvaddch(4 + x, width * 2 + 20 + y, bit ? '1' : '0');
		}
	}
}

int main(int argc, char *argv[])
{
	/* 177 * 177 bits is maximum size */
	BYTE QR_m_data[3917] = { 0 };
	int QR_width, size;
	size_t i;
	FILE *output;
	BYTE *p;

	if (argc < 2)
	{
		fprintf(stderr, "Usage: %s [data]\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	/* Max possible bits resolution 177 * 177 / 8 + 1 */
	p = QR_m_data;
	QR_width = QR_EncodeData(QR_LEVEL_Q, 0, argv[1], 0, p);
	if (QR_width < 0)
	{
		fprintf(stderr, "Could not encode data.\n");
		exit(EXIT_FAILURE);
	}

	output = fopen("file.tmp","w");
	size = ((QR_width * QR_width) / 8) + (((QR_width * QR_width) % 8) ? 1 : 0);
	fwrite(QR_m_data, size, 1,output);
	fclose(output);

	init_ncurses();

	{
		char debug[256];
		color_set(4, NULL);
		sprintf(debug, "QR_width = %d, size = %d bytes", QR_width, size);
		mvaddstr(0, 0, debug);

		debug[0] = '\0';
		for (i = 0; i != size; ++i)
		{
			char tmp[3];
			sprintf(tmp, "%02x", QR_m_data[i]);
			mvaddstr(1, i * strlen(tmp), tmp);
			strcat(debug, tmp);
		}
	}

	print_qr_code(QR_m_data, QR_width);

	refresh();
	getch();

	exit_ncurses();

	return EXIT_SUCCESS;
}

