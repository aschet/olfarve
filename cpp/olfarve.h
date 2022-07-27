 // Ol farve: sRGB color rendering of SRM/EBC beer color ratings
 // Copyright 2022 Thomas Ascher <thomas.ascher@gmx.at>
 // SPDX-License-Identifier: MIT

#ifndef OLFARVE_H
#define OLFARVE_H

#include <array>
#include <string>

namespace olfarve {
    typedef std::array<double, 3> RGB;

    // Default transmission path in cm, typical beer glass diameter as specified by the BJCP color guide
    const double DEFAULT_PATH = 5.0;

    // Determine a color in the sRGB space in relative intensity for a given SRM rating and transmission path in cm (glass diameter)
    RGB srmToSRGB(const double& srm, const double& pathCM = DEFAULT_PATH);

    // Determine a color in the sRGB space in relative intensity for a given EBC rating and transmission path in cm (glass diameter)
    RGB ebcToSRGB(const double& ebc, const double& pathCM = DEFAULT_PATH);

    // Convert a relative intensity RGB triplet into textual hex representation
    std::string rgbToHex(const RGB& rgb);
}

#endif
