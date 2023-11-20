 // Ol farve: sRGB color rendering of SRM/EBC beer color ratings
 // Copyright 2022 Thomas Ascher <thomas.ascher@gmx.at>
 // SPDX-License-Identifier: MIT

#ifndef OLFARVE_H
#define OLFARVE_H

#include <array>
#include <string>

namespace olfarve {
    typedef std::array<float, 3> RGB;

    // The default transmission path in cm. Set to typical sample glass width as specified by the BJCP color guide.
    // https://www.bjcp.org/education-training/education-resources/color-guide
    const float DEFAULT_PATH = 5.0;

    // Determine a color in the sRGB space in relative intensity for a given SRM rating and transmission path in cm (e.g. glass width)
    RGB srmToSRGB(const float& srm, const float& pathCM = DEFAULT_PATH);

    // Determine a color in the sRGB space in relative intensity for a given EBC rating and transmission path in cm (e.g. glass width)
    RGB ebcToSRGB(const float& ebc, const float& pathCM = DEFAULT_PATH);

    // Convert a relative intensity RGB triplet into textual hex representation
    std::string rgbToHex(const RGB& rgb);
}

#endif
