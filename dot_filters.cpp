#include "dot_filters.h"

QColor InvertFilter::calcNewPixelColor(const QImage& img, int x, int y) const
{
	QColor color = img.pixelColor(x, y);
	color.setRgb(255 - color.red(), 255 - color.green(), 255 - color.blue());
	return color;
}

QColor GrayScaleFilter::calcNewPixelColor(const QImage& img, int x, int y) const
{
	QColor color = img.pixelColor(x, y);
	float intensity = 0.299 * color.red() + 0.587 * color.green() + 0.114 * color.blue();
	color.setRgb(intensity, intensity, intensity);
	return color;
}

QColor SepiaFilter::calcNewPixelColor(const QImage& img, int x, int y) const
{
	QColor color = img.pixelColor(x, y);
	float intensity = 0.299 * color.red() + 0.587 * color.green() + 0.114 * color.blue();
	int k = 5;
    color.setRgb(
                clamp(intensity + 2.0 * k, 255.0, 0.0),
                clamp(intensity + 0.5 * k, 255.0, 0.0),
                clamp(intensity - 1.0 * k, 255.0, 0.0)
                );
	return color;
}

QColor BrightenFilter::calcNewPixelColor(const QImage& img, int x, int y) const
{
	QColor color = img.pixelColor(x, y);
	int k = 50;
	int returnR = clamp(color.red() + k, 255, 0);
	int returnG = clamp(color.green() + k, 255, 0);
	int returnB = clamp(color.blue() + k, 255, 0);
	color.setRgb(returnR, returnG, returnB);
	return color;
}

QImage GreyWorldFilter::process(const QImage& img) const
{
	QImage result(img);

	N = img.height() * img.width();

	for (int x = 0; x < img.width(); x++)
		for (int y = 0; y < img.height(); y++)
		{
			QColor color = img.pixelColor(x, y);

			AvgR += color.red();
			AvgG += color.green();
			AvgB += color.blue();
		}

	AvgR /= N; AvgG /= N; AvgB /= N;
	Avg = (AvgR + AvgG + AvgB) / 3;

	for (int x = 0; x < img.width(); x++)
		for (int y = 0; y < img.height(); y++)
		{
			QColor color = calcNewPixelColor(img, x, y);
			result.setPixelColor(x, y, color);
		}
	return result;
}

QColor GreyWorldFilter::calcNewPixelColor(const QImage& img, int x, int y) const
{
	QColor color = img.pixelColor(x, y);
	int returnR = clamp(int(color.red() * (Avg / AvgR)), 255, 0);
	int returnG = clamp(int(color.green() * (Avg / AvgG)), 255, 0);
	int returnB = clamp(int(color.blue() * (Avg / AvgB)), 255, 0);
	color.setRgb(returnR, returnG, returnB);
	return color;
}

QColor GlassFilter::calcNewPixelColor(const QImage& img, int x, int y) const
{
	int k = 0, l = 0;
	k = clamp(int(x + (rand() / float(RAND_MAX) - 0.5) * 10), img.width() - 1, 0);
	l = clamp(int(y + (rand() / float(RAND_MAX) - 0.5) * 10), img.height() - 1, 0);
	QColor color = img.pixelColor(k, l);
	return color;
}

QColor LinearCorrectionFilter::calcNewPixelColor(const QImage& img, int x, int y) const
{
	QColor color = img.pixelColor(x, y);
	int val = 0;
	if (max - min)
		val = (color.value() - min) * (255.0 / (max - min));
	return QColor::fromHsv(color.hue(), color.saturation(), val);
}

QImage LinearCorrectionFilter::process(const QImage& img) const
{
	QImage result(img);

	for (int x = 0; x < img.width(); x++)
		for (int y = 0; y < img.height(); y++)
		{
			QColor color = img.pixelColor(x, y);

			if (color.value() > max)
				max = color.value();
			if (color.value() < min)
				min = color.value();
		}

	for (int x = 0; x < img.width(); x++)
		for (int y = 0; y < img.height(); y++)
		{
			QColor color = calcNewPixelColor(img, x, y);
			result.setPixelColor(x, y, color);
		}
	return result;
}

QColor MedianFilter::calcNewPixelColor(const QImage& img, int x, int y) const
{
	int returnR = 0;
	int returnG = 0;
	int returnB = 0;
	short int size = 2 * radius + 1;

	short int** data = new short int* [3];
	for (int i = 0; i < 3; i++)
		data[i] = new short int[size * size];


	for (int i = -radius; i <= radius; i++)
		for (int j = -radius; j <= radius; j++)
		{
			int idx = (i + radius) * size + j + radius;
			data[0][idx] = img.pixelColor(clamp(x + j, img.width() - 1, 0), clamp(y + i, img.height() - 1, 0)).red();
			data[1][idx] = img.pixelColor(clamp(x + j, img.width() - 1, 0), clamp(y + i, img.height() - 1, 0)).green();
			data[2][idx] = img.pixelColor(clamp(x + j, img.width() - 1, 0), clamp(y + i, img.height() - 1, 0)).blue();
		}

	std::sort(data[0], data[0] + size * size);
	returnR = data[0][(size * size - 1) / 2];
	std::sort(data[1], data[1] + size * size);
	returnG = data[1][(size * size - 1) / 2];
	std::sort(data[2], data[2] + size * size);
	returnB = data[2][(size * size - 1) / 2];

	return QColor(
		clamp(returnR, 255, 0),
		clamp(returnG, 255, 0),
		clamp(returnB, 255, 0));
}
