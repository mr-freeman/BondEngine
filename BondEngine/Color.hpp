// Copyright ⓒ 2020 Valentyn Bondarenko. All rights reserved.

#pragma once

namespace be
{
    namespace utils
    {
        class Color
        {
        public:
            explicit Color(float red = 1.0f, float green = 1.0f, float blue = 1.0f, float alpha = 1.0f) : 
                r(red),
                g(green),
                b(blue),
                a(alpha)
            { }


        public:
            inline float& operator[] (const unsigned int i)
            {
                return *(&r + i);
            }

            inline Color operator+ (const Color& rhs) const
            {
                Color kSum;

                kSum.r = r + rhs.r;
                kSum.g = g + rhs.g;
                kSum.b = b + rhs.b;
                kSum.a = a + rhs.a;

                return kSum;
            }

            inline Color operator- (const Color& rhs) const
            {
                Color kDiff;

                kDiff.r = r - rhs.r;
                kDiff.g = g - rhs.g;
                kDiff.b = b - rhs.b;
                kDiff.a = a - rhs.a;

                return kDiff;
            }

            inline Color operator* (const float rhs) const
            {
                Color kProd;

                kProd.r = rhs * r;
                kProd.g = rhs * g;
                kProd.b = rhs * b;
                kProd.a = rhs * a;

                return kProd;
            }

            inline Color operator* (const Color& rhs) const
            {
                Color kProd;

                kProd.r = rhs.r * r;
                kProd.g = rhs.g * g;
                kProd.b = rhs.b * b;
                kProd.a = rhs.a * a;

                return kProd;
            }

            inline Color operator/ (const Color& rhs) const
            {
                Color kProd;

                kProd.r = rhs.r / r;
                kProd.g = rhs.g / g;
                kProd.b = rhs.b / b;
                kProd.a = rhs.a / a;

                return kProd;
            }

            inline Color operator/ (const float rhs) const
            {
                assert(rhs != 0.0f);

                Color kDiv;

                float fInv = 1.0f / rhs;
                kDiv.r = r * fInv;
                kDiv.g = g * fInv;
                kDiv.b = b * fInv;
                kDiv.a = a * fInv;

                return kDiv;
            }

            inline friend Color operator* (const float lhs, const Color& rhs)
            {
                Color result;

                result.r = lhs * rhs.r;
                result.g = lhs * rhs.g;
                result.b = lhs * rhs.b;
                result.a = lhs * rhs.a;

                return result;
            }

            inline Color& operator+= (const Color& rhs)
            {
                r += rhs.r;
                g += rhs.g;
                b += rhs.b;
                a += rhs.a;

                return *this;
            }

            inline Color& operator-= (const Color& rhs)
            {
                r -= rhs.r;
                g -= rhs.g;
                b -= rhs.b;
                a -= rhs.a;

                return *this;
            }

            inline Color& operator*= (const float rhs)
            {
                r *= rhs;
                g *= rhs;
                b *= rhs;
                a *= rhs;

                return *this;
            }

            inline Color& operator/= (const float rhs)
            {
                assert(rhs != 0.0f);

                float fInv = 1.0f / rhs;

                r *= rhs;
                g *= rhs;
                b *= rhs;
                a *= rhs;

                return *this;
            }

        public:
            float r, g, b, a;
        };
    }
}

