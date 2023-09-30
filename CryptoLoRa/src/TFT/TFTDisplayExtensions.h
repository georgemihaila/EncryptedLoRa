#ifndef TFTDisplayExtensions_h_
#define TFTDisplayExtensions_h_

#include <TFT_eSPI.h>
#include <SPI.h>

#define _width TFT_WIDTH
#define _left 0
#define _top 120
#define _height 120

#define ENTRIES _width

class TFTDisplayExtensions
{
private:
    inline static int _latestEntries[ENTRIES] = {};
    inline static unsigned long _totalEntries = 0;

public:
    static void dualLineGraphCentered(TFT_eSPI *tft, int data1[TFT_WIDTH], int data2[TFT_WIDTH], int height)
    {
        int absoluteMax = max(max(data1, TFT_WIDTH), max(data2, TFT_WIDTH));
        int yOffset = TFT_HEIGHT - height;
        graph(tft, data1, TFT_WIDTH, yOffset - height, absoluteMax);
    }
    static String formatLargeNumber(unsigned long n)
    {
        if (n < 1000)
            return String(n);
        if (n < 1000000)
            return String(n / 1000) + "." + String(((n * 100) % 1000) / 100) + "k";
        return String((n / 1000000)) + "." + String(((n * 100000) % 1000000) / 100000) + "m";
    }

    static int max(int a, int b)
    {
        if (a >= b)
            return a;
        return b;
    }

    static int minimum(int a, int b)
    {
        if (a <= b)
            return a;
        return b;
    }

    static int max(int data[], int count)
    {
        int max = data[0];
        for (int i = 0; i < count; i++)
        {
            if (data[i] > max)
            {
                max = data[i];
            }
        }
        return max;
    }

    static int min(int data[], int count)
    {
        int min = data[0];
        for (int i = 0; i < count; i++)
        {
            if (data[i] < min)
            {
                min = data[i];
            }
        }
        return min;
    }

    static void graph(TFT_eSPI *tft)
    {
        graph(tft, _latestEntries, ENTRIES, _top, false, min(_latestEntries, ENTRIES), max(_latestEntries, ENTRIES), _height);
    }

    static void graph(TFT_eSPI *tft, int data[], int count, int yOffset, bool invertVertically)
    {
        graph(tft, data, count, yOffset, invertVertically, min(_latestEntries, ENTRIES), max(data, count), _height);
    }

    static void graph(TFT_eSPI *tft, int data[], int count, int yOffset, int max, bool invertVertically)
    {
        graph(tft, data, count, yOffset, invertVertically, min(_latestEntries, ENTRIES), max, _height);
    }

    static double customMap(double x, double in_min, double in_max, double out_min, double out_max)
    {
        return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
    }

    static void graph(TFT_eSPI *tft, int data[], int count, int yOffset, bool invertVertically, int min, int max, int height)
    {
        if (min == max) // no data, don't try do do anything
            return;

        int graphBottom = yOffset + height;
        int minIndex = count - _totalEntries;
        if (minIndex < 0)
        {
            minIndex = 0;
        }
        tft->fillRect(0, yOffset, _width + _left, yOffset + height, TFT_BLACK); // Clear chart area
        int rectWidth = TFTDisplayExtensions::max(TFT_WIDTH / minimum(ENTRIES, _totalEntries), 1);
        for (int i = count - 1, ri = 0; i >= minIndex; i--, ri++)
        {
            int h1 = (int)customMap(data[i], min, max, 0, height);
            int y = graphBottom - h1;
            if (invertVertically)
            {
                y = graphBottom + h1;
            }
            int x1 = _width - (ri + 0) * rectWidth;
            tft->fillRect(x1, y, rectWidth, h1, TFT_WHITE);
            if (rectWidth >= 4)
            {
                tft->drawLine(x1, _top, x1, graphBottom, TFT_BLACK);
                tft->drawLine(x1 + rectWidth, _top, x1 + rectWidth, graphBottom, TFT_BLACK);
            }
        }
    }

    static void lineGraph(TFT_eSPI *tft)
    {
        graph(tft, _latestEntries, ENTRIES, _top, false, min(_latestEntries, ENTRIES), max(_latestEntries, ENTRIES), _height);
    }

    static void lineGraph(TFT_eSPI *tft, int data[], int count, int yOffset, bool invertVertically, int min, int max, int height)
    {
        if (min == max) // no data, don't try to do anything
            return;

        int graphBottom = yOffset + height;
        int minIndex = count - _totalEntries;
        if (minIndex < 0)
        {
            minIndex = 0;
        }

        // Clear chart area
        tft->fillRect(0, yOffset, _width + _left, yOffset + height, TFT_BLACK);

        // Calculate width between each data point
        int pointWidth = TFTDisplayExtensions::max(_width / minimum(ENTRIES, _totalEntries), 1);

        int prevX = 0, prevY = 0;

        for (int i = count - 1, ri = 0; i >= minIndex; i--, ri++)
        {
            int h1 = (int)customMap(data[i], min, max, 0, height);
            int y = graphBottom - h1;
            if (invertVertically)
            {
                y = graphBottom + h1;
            }
            int x = _width - (ri * pointWidth);

            if (ri > 0)
            {
                // Draw line from previous point to current point
                tft->drawLine(prevX, prevY, x, y, TFT_WHITE);
            }

            prevX = x;
            prevY = y;
        }
    }

    static void printAt(TFT_eSPI *tft, String text, int x, int y, int size, uint16_t color, uint16_t backgroundColor)
    {
        tft->setTextSize(size);
        tft->setTextColor(color, backgroundColor);
        tft->setCursor(x, y, 2);
        tft->println(text);
    }

    static void printAt(TFT_eSPI *tft, String text, int x, int y, int size)
    {
        printAt(tft, text, x, y, size, TFT_WHITE, TFT_BLACK);
    }

    static void printAt(TFT_eSPI *tft, String text, int x, int y)
    {
        printAt(tft, text, x, y, 1);
    }

    static void appendLastElement(int entry)
    {
        _totalEntries++;
        _latestEntries[ENTRIES - 1] = entry;
        for (int i = 0; i < ENTRIES - 1; i++)
        {
            _latestEntries[i] = _latestEntries[i + 1];
        }
    }
};

#endif
