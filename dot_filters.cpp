#include "dot_filters.h"
#include <QDebug>

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

QColor PerfectReflectorFilter::calcNewPixelColor(const QImage& img, int x, int y) const
{
    QColor color = img.pixelColor(x, y);
    return QColor(color.red()*255/Rmax, color.green()*255/Gmax, color.blue()*255/Bmax);
}

QImage PerfectReflectorFilter::process(const QImage& img) const
{
    QImage result(img);

    for (int x = 0; x < img.width(); x++)
        for (int y = 0; y < img.height(); y++)
        {
            QColor color = img.pixelColor(x, y);

            if (color.red() > Rmax)
                Rmax = color.red();
            if (color.green() > Gmax)
                Gmax = color.green();
            if (color.blue() > Bmax)
                Bmax = color.blue();
        }

    for (int x = 0; x < img.width(); x++)
        for (int y = 0; y < img.height(); y++)
        {
            QColor color = calcNewPixelColor(img, x, y);
            result.setPixelColor(x, y, color);
        }
    qDebug() << Rmax;
    qDebug() << Gmax;
    qDebug() << Bmax;
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

QColor StaticCorrectionFilter::calcNewPixelColor(const QImage& img, int x, int y) const
{
    QColor color = img.pixelColor(x, y);

    int c1 = clamp<float>(Es[0] + (color.red() - Et[0]) * ((float)Ds[0] / Dt[0]), 254.9, 0.1);
    int c2 = clamp<float>(Es[1] + (color.green() - Et[1]) * ((float)Ds[1] / Dt[1]), 254.9, 0.1);
    int c3 = clamp<float>(Es[2] + (color.blue() - Et[2]) * ((float)Ds[2] / Dt[2]), 254.9, 0.1);

    return QColor(c1, c2, c3);
}

QImage StaticCorrectionFilter::process(const QImage& img) const
{
    QImage result(img);

    /*---------------------------------------------------------------------*/

    double t1 = 0, t2 = 0, t3 = 0;

    for (int x = 0; x < source.width(); x++)
        for (int y = 0; y < source.height(); y++)
        {
            QColor color = source.pixelColor(x, y);

            Es[0] += color.red();
            Es[1] += color.green();
            Es[2] += color.blue();
        }

    Es[0] /= (source.width() * source.height());
    Es[1] /= (source.width() * source.height());
    Es[2] /= (source.width() * source.height());

    for (int x = 0; x < source.width(); x++)
        for (int y = 0; y < source.height(); y++)
        {
            QColor color = source.pixelColor(x, y);

            t1 += ((color.red() - Es[0]) * (color.red() - Es[0]));
            t2 += ((color.green() - Es[1]) * (color.green() - Es[1]));
            t3 += ((color.blue() - Es[2]) * (color.blue() - Es[2]));
        }

    Ds[0] = sqrt(t1 / (source.width() * source.height()));
    Ds[1] = sqrt(t2 / (source.width() * source.height()));
    Ds[2] = sqrt(t3 / (source.width() * source.height()));

    /*---------------------------------------------------------------------*/

    /*---------------------------------------------------------------------*/

    t1 = 0, t2 = 0, t3 = 0;

    for (int x = 0; x < img.width(); x++)
        for (int y = 0; y < img.height(); y++)
        {
            QColor color = img.pixelColor(x, y);

            Et[0] += color.red();
            Et[1] += color.green();
            Et[2] += color.blue();
        }

    Et[0] /= img.width() * img.height();
    Et[1] /= img.width() * img.height();
    Et[2] /= img.width() * img.height();

    for (int x = 0; x < img.width(); x++)
        for (int y = 0; y < img.height(); y++)
        {
            QColor color = img.pixelColor(x, y);

            t1 += (color.red() - Et[0]) * (color.red() - Et[0]);
            t2 += (color.green() - Et[1]) * (color.green() - Et[1]);
            t3 += (color.blue() - Et[2]) * (color.blue() - Et[2]);
        }

    Dt[0] = sqrt(t1 / (img.width() * img.height()));
    Dt[1] = sqrt(t2 / (img.width() * img.height()));
    Dt[2] = sqrt(t3 / (img.width() * img.height()));

    /*---------------------------------------------------------------------*/


    for (int x = 0; x < img.width(); x++)
        for (int y = 0; y < img.height(); y++)
        {
            QColor color = calcNewPixelColor(img, x, y);
            result.setPixelColor(x, y, color);
        }

    return result;
}
