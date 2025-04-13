#include <iostream>

using namespace System;
using namespace System::Drawing;
using namespace System::Windows::Forms;

// CONSTANTS
constexpr int SIZE_TABLE = 10;
constexpr float WIDTH_OFFSET = 1.5;
constexpr int MAX_WIDTH = 350;
constexpr char TABLE[SIZE_TABLE] = { '.',',',':','+','*','?','%','S','#','@' };

// Functions
float Map(float valueToMap, float start1, float stop1, float start2, float stop2)
{
	return ((valueToMap - start1) / (stop1 - start1) * (stop2 - start2) + start2);
}

void freeChars(char** c, int count)
{
	for (size_t i = 0; i < count; i++)
	{
		delete[] c[i];
	}
	delete[] c;
}

char** ToAsciiConvert(Bitmap^ bmp)
{
	const int Y = bmp->Height;
	const int X = bmp->Width;
	char** res = new char* [Y];

	for (size_t y = 0; y < Y; y++)
	{
		res[y] = new char[X];
		for (size_t x = 0; x < X; x++)
		{
			int index = (int)Map(bmp->GetPixel(x, y).R, 0, 255, 0, SIZE_TABLE - 1);
			res[y][x] = TABLE[index];
		}
	}
	return res;
}

Bitmap^ ToGrayScale(Bitmap^ bmp)
{
	for (size_t y = 0; y < bmp->Height; y++)
	{
		for (size_t x = 0; x < bmp->Width; x++)
		{
			Color pixel = bmp->GetPixel(x, y);
			int avg = (pixel.R + pixel.G, pixel.B) / 3;
			bmp->SetPixel(x, y, Color::FromArgb(pixel.A, avg, avg, avg));
		}
	}
	return bmp;
}

Bitmap^ ResizeBitmap(Bitmap^ bmp)
{
	int newHeight = bmp->Height / WIDTH_OFFSET * MAX_WIDTH / bmp->Width;
	if (bmp->Width > MAX_WIDTH || bmp->Height > newHeight)
	{
		bmp = gcnew Bitmap(bmp, MAX_WIDTH, (int)newHeight);
	}
	return bmp;
}

[STAThreadAttribute]
int main()
{
	OpenFileDialog^ file = gcnew OpenFileDialog();
	bool isRun = true;

	file->Filter = "Images (*.bmp, *jpg, *.png, *.JPEG) | *.bmp; *.jpg; *.png; *.JPEG";
	std::cout << "Press enter to start";

	while (isRun)
	{
		system("pause");
		if (file->ShowDialog() != DialogResult::OK)
		{
			continue;
		}
		system("cls");
		Bitmap^ bmp = gcnew Bitmap(file->FileName);
		bmp = ResizeBitmap(bmp);
		bmp = ToGrayScale(bmp);

		char** res = ToAsciiConvert(bmp);

		for (int i = 0; i < bmp->Height; ++i)
		{
			for (int j = 0; j < bmp->Width; ++j)
			{
				std::cout << res[i][j];
			}
			std::cout << std::endl;
		}

		freeChars(res, bmp->Height);
	}

	system("pause");
	return 0;
}