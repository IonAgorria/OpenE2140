//
// Created by Ion Agorria on 10/11/18
//
#ifndef OPENE2140_COLOR_H
#define OPENE2140_COLOR_H

#include "SDL_pixels.h"
#include "engine/core/common.h"

/**
 * Struct for RG color
 */
struct ColorRG {
    byte_t r = 0;
    byte_t g = 0;

    /**
     * Sets this color from other color
     */
    template<typename T>
    void setRG(const T& color) {
        r = color.r;
        g = color.g;
    }

    /**
     * @return string version of this color
     */
    std::string toString() const {
        return " R " + std::to_string(r) + " G " + std::to_string(g);
    }
};

/**
 * Struct for RGB color
 */
struct ColorRGB {
    byte_t r = 0;
    byte_t g = 0;
    byte_t b = 0;

    /**
     * Sets this color from other color
     */
    template<typename T>
    void setRGB(const T& color) {
        r = color.r;
        g = color.g;
        b = color.b;
    }

    /**
     * @return string version of this color
     */
    std::string toString() const {
        return " R " + std::to_string(r) + " G " + std::to_string(g) + " B " + std::to_string(b);
    }
};

/**
 * Struct for RGBA color
 */
struct ColorRGBA: public SDL_Color {
    /**
     * Sets this color from other color
     */
    template<typename T>
    void setRGB(const T& color) {
        r = color.r;
        g = color.g;
        b = color.b;
    }

    /**
     * Sets this color from other color
     */
    template<typename T>
    void setRGBA(const T& color) {
        r = color.r;
        g = color.g;
        b = color.b;
        a = color.a;
    }

    /**
     * @return string version of this color
     */
    std::string toString() const {
        return " R " + std::to_string(r) + " G " + std::to_string(g)
             + " B " + std::to_string(b) + " A " + std::to_string(a);
    }
};

/**
 * Struct for HSV color
 *
 * https://www.cs.rit.edu/~ncs/color/t_convert.html
 */
struct ColorHSV {
    float h = 0;
    float s = 0;
    float v = 0;

    /**
     * Sets this color from RGBA
     */
    template<typename T>
    void fromRGB(const T& color) {
        h = s = v = 0;
        float r = static_cast<float>(color.r) / 255.0f;
        float g = static_cast<float>(color.g) / 255.0f;
        float b = static_cast<float>(color.b) / 255.0f;
        float min = std::min(r, std::min(g, b));
        float max = std::max(r, std::max(g, b));
        v = max;
        float delta = max - min;

        if (max != 0) {
            s = delta / max;
        } else {
            // r = g = b = 0		// s = 0, v is undefined
            return;
        }

        if (0 < delta) {
            if (r == max) {
                h = (g - b) / delta;        // between yellow & magenta
            } else if (g == max) {
                h = 2 + (b - r) / delta;    // between cyan & yellow
            } else {
                h = 4 + (r - g) / delta;    // between magenta & cyan
            }
        }

        h *= 60;
        if (h < 0) {
            h += 360;
        }
    }

    /**
     * Set this color from HSV
     * https://www.cs.rit.edu/~ncs/color/t_convert.html
     */
    template<typename T>
    void toRGB(T& color) {
        int i;
        float f, p, q, t;

        if (s == 0) {
            // achromatic (grey)
            color.r = static_cast<byte_t>(255 * v);
            color.g = static_cast<byte_t>(255 * v);
            color.b = static_cast<byte_t>(255 * v);
            return;
        }

        h /= 60;			// sector 0 to 5
        i = std::floor(h);
        f = h - static_cast<float>(i);			// factorial part of h
        p = v * (1 - s);
        q = v * (1 - s * f);
        t = v * (1 - s * (1 - f));

        switch (i) {
            case 0:
                color.r = static_cast<byte_t>(255 * v);
                color.g = static_cast<byte_t>(255 * t);
                color.b = static_cast<byte_t>(255 * p);
                break;
            case 1:
                color.r = static_cast<byte_t>(255 * q);
                color.g = static_cast<byte_t>(255 * v);
                color.b = static_cast<byte_t>(255 * p);
                break;
            case 2:
                color.r = static_cast<byte_t>(255 * p);
                color.g = static_cast<byte_t>(255 * v);
                color.b = static_cast<byte_t>(255 * t);
                break;
            case 3:
                color.r = static_cast<byte_t>(255 * p);
                color.g = static_cast<byte_t>(255 * q);
                color.b = static_cast<byte_t>(255 * v);
                break;
            case 4:
                color.r = static_cast<byte_t>(255 * t);
                color.g = static_cast<byte_t>(255 * p);
                color.b = static_cast<byte_t>(255 * v);
                break;
            default:		// case 5:
                color.r = static_cast<byte_t>(255 * v);
                color.g = static_cast<byte_t>(255 * p);
                color.b = static_cast<byte_t>(255 * q);
                break;
        }
    }

    /**
     * @return string version of this color
     */
    std::string toString() const {
        return " H " + std::to_string(h) + " S " + std::to_string(s) + " V " + std::to_string(v);
    }
};

/**
 * Namespace for color constants
 */
namespace Color {
    const ColorRGBA CLEAR           {0x00, 0x00, 0x00, 0x00};
    const ColorRGBA BLACK           {0x00, 0x00, 0x00, 0xFF};
    const ColorRGBA RED             {0xFF, 0x00, 0x00, 0xFF};
    const ColorRGBA GREEN           {0x00, 0xFF, 0x00, 0xFF};
    const ColorRGBA BLUE            {0x00, 0x00, 0xFF, 0xFF};
    const ColorRGBA WHITE           {0xFF, 0xFF, 0xFF, 0xFF};
    const ColorRGBA CYAN            {0x00, 0xFF, 0xFF, 0xFF};
    const ColorRGBA GREY            {0x80, 0x80, 0x80, 0xFF};
    const ColorRGBA DEBUG_WORLD     {0xA0, 0xA0, 0xA0, 0x20};
    const ColorRGBA DEBUG_ENTITIES  {0xFF, 0x00, 0x00, 0x40};
}

#endif //OPENE2140_COLOR_H
