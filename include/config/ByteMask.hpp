#pragma once

class ByteMask
{
public:
    int width;
    int height;
    unsigned char* data;

    ByteMask() : width(0), height(0), data(nullptr)
    {
    }
    ~ByteMask()
    {
        delete[] data;
    }

    void loadFromImage(const std::string& path)
    {
        Image img = LoadImage(path.c_str());
        width = img.width;
        height = img.height;
        data = new unsigned char[width * height];

        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < width; x++)
            {
                Color c = GetImageColor(img, x, y);
                data[y * width + x] = (c.a > 1) ? 1 : 0;
            }
        }

        UnloadImage(img);
    }

    bool checkPixelCollision(const ByteMask& other, const Vector2& posA, const Vector2& posB,
                             const Rectangle& overlap) const
    {
        for (int y = 0; y < (int)overlap.height; y++)
        {
            int ay = (int)(overlap.y - posA.y + y);
            int by = (int)(overlap.y - posB.y + y);

            for (int x = 0; x < (int)overlap.width; x++)
            {
                int ax = (int)(overlap.x - posA.x + x);
                int bx = (int)(overlap.x - posB.x + x);

                if (ax < 0 || ay < 0 || ax >= width || ay >= height)
                    continue;
                if (bx < 0 || by < 0 || bx >= other.width || by >= other.height)
                    continue;

                if (data[ay * width + ax] && other.data[by * other.width + bx])
                    return true;
            }
        }
        return false;
    }
};
