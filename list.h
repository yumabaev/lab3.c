#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <windows.h>
#define WIN32_MEAN_AND_LEAN

void out(BITMAPFILEHEADER file_header_1, BITMAPINFOHEADER info_header_1, FILE* input_1, BITMAPFILEHEADER file_header_2, BITMAPINFOHEADER info_header_2, FILE* input_2, FILE* output);

void read(const char* file_in_1, const char* file_in_2, const char* file_out);