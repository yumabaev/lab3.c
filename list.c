#include"list.h"

void out(BITMAPFILEHEADER file_header_1, BITMAPINFOHEADER info_header_1, FILE* input_1, BITMAPFILEHEADER file_header_2, BITMAPINFOHEADER info_header_2, FILE* input_2, FILE* output) {
	RGBTRIPLE pixel_1;
	size_t Width_1 = info_header_1.biWidth;
	size_t Height_1 = info_header_1.biHeight;

	RGBTRIPLE pixel_2;
	size_t Width_2 = info_header_2.biWidth;
	size_t Height_2 = info_header_2.biHeight;
	RGBTRIPLE pixel_rez;

	unsigned char padding = 0;
	if ((Width_1 * 3) % 4)
		padding = 4 - (Width_1 * 3) % 4;
	//выделяем память для массива данных о пикселях//
	RGBTRIPLE** img_1 = new RGBTRIPLE * [Height_1];
	for (size_t i = 0; i < Height_1; i++)
		img_1[i] = new RGBTRIPLE[Width_1];

	RGBTRIPLE** img_2 = new RGBTRIPLE * [Height_2];
	for (size_t i = 0; i < Height_2; i++)
		img_2[i] = new RGBTRIPLE[Width_2];

	//чтение пикселей 1 файла//
	for (size_t i = 0; i < Height_1; i++) {
		for (size_t j = 0; j < Width_1; j++) {
			fread(&pixel_1, sizeof(RGBTRIPLE), 1, input_1);
			img_1[i][j] = pixel_1;
		}
		fseek(input_1, padding, SEEK_CUR); // пропуск "выравнивающих" байтов
	}
	fclose(input_1);

	/*чтение пикселей 2 файла*/
	for (size_t i = 0; i < Height_2; i++) {
		for (size_t j = 0; j < Width_2; j++) {
			fread(&pixel_2, sizeof(RGBTRIPLE), 1, input_2);
			img_2[i][j] = pixel_2;
		}
		fseek(input_2, padding, SEEK_CUR); // пропуск "выравнивающих" байтов
	}
	fclose(input_2);

	/*запись нового файла*/
	info_header_1.biHeight = Height_1;
	info_header_1.biWidth = Width_1;
	fwrite(&file_header_1, sizeof(BITMAPFILEHEADER), 1, output);
	fwrite(&info_header_1, sizeof(BITMAPINFOHEADER), 1, output);

	//расчет нового значения padding 
	padding = 0;
	if ((Height_1 * 3) % 4)
		padding = 4 - (Height_1 * 3) % 4;

	for (size_t i = 0; i < Height_1; i++) {
		for (size_t j = 0; j < Width_1; j++) {

			int check = 0;
			check = img_1[i][j].rgbtBlue * 0.5 + img_2[i][j].rgbtBlue * 0.5;
			if (check < 0)
				pixel_rez.rgbtBlue = 0;
			else
				pixel_rez.rgbtBlue = check;

			check = img_1[i][j].rgbtGreen * 0.5 + img_2[i][j].rgbtGreen * 0.5;
			if (check < 0)
				pixel_rez.rgbtGreen = 0;
			else
				pixel_rez.rgbtGreen = check;

			check = img_1[i][j].rgbtRed * 0.5 + img_2[i][j].rgbtRed * 0.5;
			if (check < 0)
				pixel_rez.rgbtRed = 0;
			else
				pixel_rez.rgbtRed = check;

			fwrite(&pixel_rez, sizeof(RGBTRIPLE), 1, output);
		}
		if (padding != 0) {
			BYTE filler = 0;
			fwrite(&filler, sizeof(BYTE), padding, output);
		}
	}
}

void read(const char* file_in_1, const char* file_in_2, const char* file_out) {
	//заголовки необходимые для работы с bmp
	BITMAPFILEHEADER file_header_1;
	BITMAPFILEHEADER file_header_2;
	BITMAPINFOHEADER info_header_1;
	BITMAPINFOHEADER info_header_2;
	FILE* input_1, * input_2, * output;

	//открываем файлы для чтения
	input_1 = fopen(file_in_1, "rb");
	input_2 = fopen(file_in_2, "rb");
	output = fopen(file_out, "wb");
	if (!input_1 || !input_2) {
		printf("File bmp not found\n");
		system("pause");
		return;
	}
	//чтение BMPFILEHEADER и  BMPINFOHEADER//
	fread(&file_header_1, sizeof(BITMAPFILEHEADER), 1, input_1);
	fread(&info_header_1, sizeof(BITMAPINFOHEADER), 1, input_1);
	fread(&file_header_2, sizeof(BITMAPFILEHEADER), 1, input_2);
	fread(&info_header_2, sizeof(BITMAPINFOHEADER), 1, input_2);

	if (info_header_1.biCompression != 0 || info_header_1.biBitCount != 24 || info_header_1.biPlanes != 1 || info_header_2.biCompression != 0 || info_header_2.biBitCount != 24 || info_header_2.biPlanes != 1) {
		printf("File not a BMP24\n");
		system("pause");
		return;
	}
	out(file_header_1, info_header_1, input_1, file_header_2, info_header_2, input_2, output);
}