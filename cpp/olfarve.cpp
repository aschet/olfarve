 // Ol farve: sRGB color rendering of SRM/EBC beer color ratings
 // Copyright 2022 Thomas Ascher <thomas.ascher@gmx.at>
 // SPDX-License-Identifier: MIT

#include "olfarve.h"
#include <cmath>
#include <sstream>
#include <iomanip>

namespace olfarve {

// This data table is composed of the following elements:
// CIE 1931 colour-matching functions (x_bar, y_bar, z_bar), 2 degree observer, 5 nm
// https://cie.co.at/datatable/cie-1931-colour-matching-functions-2-degree-observer-5nm
// CIE standard illuminant D65
// https://cie.co.at/datatable/cie-standard-illuminant-d65
static const std::array<std::array<float, 4>, 81> CIE_DATA = { {
    { 0.001368f, 0.000039f, 0.006450f, 49.9755f },
    { 0.002236f, 0.000064f, 0.010550f, 52.3118f },
    { 0.004243f, 0.000120f, 0.020050f, 54.6482f },
    { 0.007650f, 0.000217f, 0.036210f, 68.7015f },
    { 0.014310f, 0.000396f, 0.067850f, 82.7549f },
    { 0.023190f, 0.000640f, 0.110200f, 87.1204f },
    { 0.043510f, 0.001210f, 0.207400f, 91.486f },
    { 0.077630f, 0.002180f, 0.371300f, 92.4589f },
    { 0.134380f, 0.004000f, 0.645600f, 93.4318f },
    { 0.214770f, 0.007300f, 1.039050f, 90.057f },
    { 0.283900f, 0.011600f, 1.385600f, 86.6823f },
    { 0.328500f, 0.016840f, 1.622960f, 95.7736f },
    { 0.348280f, 0.023000f, 1.747060f, 104.865f },
    { 0.348060f, 0.029800f, 1.782600f, 110.936f },
    { 0.336200f, 0.038000f, 1.772110f, 117.008f },
    { 0.318700f, 0.048000f, 1.744100f, 117.41f },
    { 0.290800f, 0.060000f, 1.669200f, 117.812f },
    { 0.251100f, 0.073900f, 1.528100f, 116.336f },
    { 0.195360f, 0.090980f, 1.287640f, 114.861f },
    { 0.142100f, 0.112600f, 1.041900f, 115.392f },
    { 0.095640f, 0.139020f, 0.812950f, 115.923f },
    { 0.057950f, 0.169300f, 0.616200f, 112.367f },
    { 0.032010f, 0.208020f, 0.465180f, 108.811f },
    { 0.014700f, 0.258600f, 0.353300f, 109.082f },
    { 0.004900f, 0.323000f, 0.272000f, 109.354f },
    { 0.002400f, 0.407300f, 0.212300f, 108.578f },
    { 0.009300f, 0.503000f, 0.158200f, 107.802f },
    { 0.029100f, 0.608200f, 0.111700f, 106.296f },
    { 0.063270f, 0.710000f, 0.078250f, 104.79f },
    { 0.109600f, 0.793200f, 0.057250f, 106.239f },
    { 0.165500f, 0.862000f, 0.042160f, 107.689f },
    { 0.225750f, 0.914850f, 0.029840f, 106.047f },
    { 0.290400f, 0.954000f, 0.020300f, 104.405f },
    { 0.359700f, 0.980300f, 0.013400f, 104.225f },
    { 0.433450f, 0.994950f, 0.008750f, 104.046f },
    { 0.512050f, 1.000000f, 0.005750f, 102.023f },
    { 0.594500f, 0.995000f, 0.003900f, 100.0f },
    { 0.678400f, 0.978600f, 0.002750f, 98.1671f },
    { 0.762100f, 0.952000f, 0.002100f, 96.3342f },
    { 0.842500f, 0.915400f, 0.001800f, 96.0611f },
    { 0.916300f, 0.870000f, 0.001650f, 95.788f },
    { 0.978600f, 0.816300f, 0.001400f, 92.2368f },
    { 1.026300f, 0.757000f, 0.001100f, 88.6856f },
    { 1.056700f, 0.694900f, 0.001000f, 89.3459f },
    { 1.062200f, 0.631000f, 0.000800f, 90.0062f },
    { 1.045600f, 0.566800f, 0.000600f, 89.8026f },
    { 1.002600f, 0.503000f, 0.000340f, 89.5991f },
    { 0.938400f, 0.441200f, 0.000240f, 88.6489f },
    { 0.854450f, 0.381000f, 0.000190f, 87.69871f },
    { 0.751400f, 0.321000f, 0.000100f, 85.4936f },
    { 0.642400f, 0.265000f, 0.000050f, 83.2886f },
    { 0.541900f, 0.217000f, 0.000030f, 83.4939f },
    { 0.447900f, 0.175000f, 0.000020f, 83.6992f },
    { 0.360800f, 0.138200f, 0.000010f, 81.863f },
    { 0.283500f, 0.107000f, 0.000000f, 80.0268f },
    { 0.218700f, 0.081600f, 0.000000f, 80.1207f },
    { 0.164900f, 0.061000f, 0.000000f, 80.2146f },
    { 0.121200f, 0.044580f, 0.000000f, 81.2462f },
    { 0.087400f, 0.032000f, 0.000000f, 82.2778f },
    { 0.063600f, 0.023200f, 0.000000f, 80.281f },
    { 0.046770f, 0.017000f, 0.000000f, 78.2842f },
    { 0.032900f, 0.011920f, 0.000000f, 74.0027f },
    { 0.022700f, 0.008210f, 0.000000f, 69.7213f },
    { 0.015840f, 0.005723f, 0.000000f, 70.6652f },
    { 0.011359f, 0.004102f, 0.000000f, 71.6091f },
    { 0.008111f, 0.002929f, 0.000000f, 72.979f },
    { 0.005790f, 0.002091f, 0.000000f, 74.349f },
    { 0.004109f, 0.001484f, 0.000000f, 67.9765f },
    { 0.002899f, 0.001047f, 0.000000f, 61.604f },
    { 0.002049f, 0.000740f, 0.000000f, 65.7448f },
    { 0.001440f, 0.000520f, 0.000000f, 69.8856f },
    { 0.001000f, 0.000361f, 0.000000f, 72.4863f },
    { 0.000690f, 0.000249f, 0.000000f, 75.087f },
    { 0.000476f, 0.000172f, 0.000000f, 69.3398f },
    { 0.000332f, 0.000120f, 0.000000f, 63.5927f },
    { 0.000235f, 0.000085f, 0.000000f, 55.0054f },
    { 0.000166f, 0.000060f, 0.000000f, 46.4182f },
    { 0.000117f, 0.000042f, 0.000000f, 56.6118f },
    { 0.000083f, 0.000030f, 0.000000f, 66.8054f },
    { 0.000059f, 0.000021f, 0.000000f, 65.0941f },
    { 0.000042f, 0.000015f, 0.000000f, 63.3828f }
} };

static float calcScaleFactor() {
    float k = 0.0f;
    for (size_t i = 0; i < CIE_DATA.size(); ++i)
        k += CIE_DATA[i][3] * CIE_DATA[i][1];
    return 1.0f / k;
}

static const float K = calcScaleFactor();

static float correctGamma(float t) {
    if (t <= 0.0031308f)
        t = t * 12.92f;
    else
        t = 1.055f * pow(t, 1.0f / 2.4f) - 0.055f;
    return std::max(0.0f, std::min(1.0f, t));
}

// Implemented according to A. J. de Lange, "Color," in Brewing Materials and Processes, Elsevier, 2016, pp. 199-249.
// Color space is mapped to sRGB, which requires a scaling of 1.0. For sRGB related transformations
// see C. Poynton, Digital Video and HD: Algorithms and Interfaces, 2nd ed. Morgan Kaufmann, 2014.
static RGB beerSDToSRGB(const float& a430, const float& l) {
    float x = 0.0;
    float y = 0.0;
    float z = 0.0;
    float w = 380.0;
    for (size_t i = 0; i < CIE_DATA.size(); ++i) {
        float t = pow(10.0f, -a430 * l * (0.02465f * exp(-(w - 430.0f) / 17.591f) + 0.97535f * exp(-(w - 430.0f) / 82.122f)));
        float d65 = CIE_DATA[i][3];
        x += d65 * t * CIE_DATA[i][0];
        y += d65 * t * CIE_DATA[i][1];
        z += d65 * t * CIE_DATA[i][2];
        w += 5.0f;
    }

    x *= K;
    y *= K;
    z *= K;  

    float r = correctGamma(x * 3.240479f + y * -1.537150f + z * -0.498535f);
    float g = correctGamma(x * -0.969256f + y * 1.875992f + z * 0.041556f);
    float b = correctGamma(x * 0.055648f + y * -0.204043f + z * 1.057311f);
    return { r, g, b };
}

RGB srmToSRGB(const float& srm, const float& pathCM) {
    return beerSDToSRGB(srm / 12.7f, pathCM);
}

RGB ebcToSRGB(const float& ebc, const float& pathCM) {
    return beerSDToSRGB(ebc / 25.0f, pathCM);
}

std::string rgbToHex(const RGB& rgb) {
    std::stringstream text;
    text << "#";
    for (const float& n : rgb)
        text << std::setfill('0') << std::setw(2) << std::hex << lround(n * 255.0f);
    return text.str();
}

}
