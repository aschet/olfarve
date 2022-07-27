// Ol farve: sRGB color rendering of SRM/EBC beer color ratings
// Copyright 2022 Thomas Ascher <thomas.ascher@gmx.at>
// SPDX-License-Identifier: MIT

// The algorithmic background of this implementation is extensively discussed by A.J. deLange in
// Bamforth's Brewing Materials and Processes (DOI: 10.1016/B978-0-12-799954-8.00011-3).
// Computational methods and tabular data were obtained from Brue Lindbloom's webpage
// (http://www.brucelindbloom.com) and the Colour Science for Python package.

// Spectral shape of the ASBC Tristimulus MOA, 380-780 nm in 5 nm intervals
const LAMBDA = [
    380.0, 385.0, 390.0, 395.0,
    400.0, 405.0, 410.0, 415.0, 420.0, 425.0, 430.0, 435.0, 440.0, 445.0,
    450.0, 455.0, 460.0, 465.0, 470.0, 475.0, 480.0, 485.0, 490.0, 495.0,
    500.0, 505.0, 510.0, 515.0, 520.0, 525.0, 530.0, 535.0, 540.0, 545.0,
    550.0, 555.0, 560.0, 565.0, 570.0, 575.0, 580.0, 585.0, 590.0, 595.0,
    600.0, 605.0, 610.0, 615.0, 620.0, 625.0, 630.0, 635.0, 640.0, 645.0,
    650.0, 655.0, 660.0, 665.0, 670.0, 675.0, 680.0, 685.0, 690.0, 695.0,
    700.0, 705.0, 710.0, 715.0, 720.0, 725.0, 730.0, 735.0, 740.0, 745.0,
    750.0, 755.0, 760.0, 765.0, 770.0, 775.0, 780.0
];

// Interval of the spectral shape above
const DELTA_LAMBDA = 5.0;

// D65 illuminant, adjusted to spectral shape above
const I = [
    49.9755, 52.3118, 54.6482, 68.7015,
    82.7549, 87.1204, 91.486, 92.4589, 93.4318, 90.057, 86.6823, 95.7736, 104.865, 110.936,
    117.008, 117.41, 117.812, 116.336, 114.861, 115.392, 115.923, 112.367, 108.811, 109.082,
    109.354, 108.578, 107.802, 106.296, 104.79, 106.239, 107.689, 106.047, 104.405, 104.225,
    104.046, 102.023, 100.0, 98.1671, 96.3342, 96.0611, 95.788, 92.2368, 88.6856, 89.3459,
    90.0062, 89.8026, 89.5991, 88.6489, 87.6987, 85.4936, 83.2886, 83.4939, 83.6992, 81.863,
    80.0268, 80.1207, 80.2146, 81.2462, 82.2778, 80.281, 78.2842, 74.0027, 69.7213, 70.6652,
    71.6091, 72.979, 74.349, 67.9765, 61.604, 65.7448, 69.8856, 72.4863, 75.087, 69.3398,
    63.5927, 55.0054, 46.4182, 56.6118, 66.8054, 65.0941, 63.3828,
];

// Color matching functions of the CIE 1931 2 degree observer, adjusted to spectral shape above
const X_BAR = [
    0.001368, 0.002236, 0.004243, 0.00765,
    0.01431, 0.02319, 0.04351, 0.07763, 0.13438, 0.21477, 0.2839, 0.3285, 0.34828, 0.34806,
    0.3362, 0.3187, 0.2908, 0.2511, 0.19536, 0.1421, 0.09564, 0.05795001, 0.03201, 0.0147,
    0.0049, 0.0024, 0.0093, 0.0291, 0.06327, 0.1096, 0.1655, 0.2257499, 0.2904, 0.3597,
    0.4334499, 0.5120501, 0.5945, 0.6784, 0.7621, 0.8425, 0.9163, 0.9786, 1.0263, 1.0567,
    1.0622, 1.0456, 1.0026, 0.9384, 0.8544499, 0.7514, 0.6424, 0.5419, 0.4479, 0.3608,
    0.2835, 0.2187, 0.1649, 0.1212, 0.0874, 0.0636, 0.04677, 0.0329, 0.0227, 0.01584,
    0.01135916, 0.008110916, 0.005790346, 0.004109457, 0.002899327, 0.00204919, 0.001439971, 0.0009999493, 0.0006900786, 0.0004760213,
    0.0003323011, 0.0002348261, 0.0001661505, 0.000117413, 0.00008307527, 0.00005870652, 0.00004150994,
];

const Y_BAR = [
    0.000039, 0.000064, 0.00012, 0.000217,
    0.000396, 0.00064, 0.00121, 0.00218, 0.004, 0.0073, 0.0116, 0.01684, 0.023, 0.0298,
    0.038, 0.048, 0.06, 0.0739, 0.09098, 0.1126, 0.13902, 0.1693, 0.20802, 0.2586,
    0.323, 0.4073, 0.503, 0.6082, 0.71, 0.7932, 0.862, 0.9148501, 0.954, 0.9803,
    0.9949501, 1.0, 0.995, 0.9786, 0.952, 0.9154, 0.87, 0.8163, 0.757, 0.6949,
    0.631, 0.5668, 0.503, 0.4412, 0.381, 0.321, 0.265, 0.217, 0.175, 0.1382,
    0.107, 0.0816, 0.061, 0.04458, 0.032, 0.0232, 0.017, 0.01192, 0.00821, 0.005723,
    0.004102, 0.002929, 0.002091, 0.001484, 0.001047, 0.00074, 0.00052, 0.0003611, 0.0002492, 0.0001719,
    0.00012, 0.0000848, 0.00006, 0.0000424, 0.00003, 0.0000212, 0.00001499,
];

const Z_BAR = [
    0.006450001, 0.01054999, 0.02005001, 0.03621,
    0.06785001, 0.1102, 0.2074, 0.3713, 0.6456, 1.0390501, 1.3856, 1.62296, 1.74706, 1.7826,
    1.77211, 1.7441, 1.6692, 1.5281, 1.28764, 1.0419, 0.8129501, 0.6162, 0.46518, 0.3533,
    0.272, 0.2123, 0.1582, 0.1117, 0.07824999, 0.05725001, 0.04216, 0.02984, 0.0203, 0.0134,
    0.008749999, 0.005749999, 0.0039, 0.002749999, 0.0021, 0.0018, 0.001650001, 0.0014, 0.0011, 0.001,
    0.0008, 0.0006, 0.00034, 0.00024, 0.00019, 0.0001, 0.00004999999, 0.00003, 0.00002, 0.00001,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
];

// Default transmission path in cm, typical beer glass diameter as specified by the BJCP color guide
const DEFAULT_PATH = 5.0;

function summate(n, cmf, s) {
    let sum = 0.0;
    for (let i = 0; i < cmf.length; ++i)
        sum += cmf[i] * s[i] * I[i] * DELTA_LAMBDA;
    return 1.0 / n * sum;
}

function correctGamma(l) {
    if (l <= 0.0031308)
        l = l * 12.92;
    else
        l = 1.055 * Math.pow(l, 1.0 / 2.4) - 0.055;
    return Math.max(0.0, Math.min(1.0, l));
}

function beerSDToSRGB(a430, pathCM) {
    // Calculate the transmission spectrum distribution from the average of the absorption spectra of the ensemble of 99 beers
    let s = new Array(LAMBDA.length);
    for (let i = 0; i < LAMBDA.length; ++i)
        s[i] = Math.pow(10.0, -a430 * (0.02465 * Math.exp(-(LAMBDA[i] - 430.0) / 17.591) + 0.97535 * Math.exp(-(LAMBDA[i] - 430.0) / 82.122)) * pathCM);

    // Transform the spectrum distribution into the CIE XYZ color space with integration over 5 nm intervals
    let n = 0.0;
    for (let i = 0; i < Y_BAR.length; ++i)
        n += Y_BAR[i] * I[i] * DELTA_LAMBDA;
    let x = summate(n, X_BAR, s);
    let y = summate(n, Y_BAR, s);
    let z = summate(n, Z_BAR, s);

    // Transform color space to sRGB
    let r = correctGamma(x * 3.2406 + y * -1.5372 + z * -0.4986);
    let g = correctGamma(x * -0.9689 + y * 1.8758 + z * 0.0415);
    let b = correctGamma(x * 0.0557 + y * -0.2040 + z * 1.0570);
    return [r, g, b];
}

// Determine a color in the sRGB space in relative intensity for a given SRM rating and transmission path in cm (glass diameter)
function srmToSRGB(srm, pathCM = DEFAULT_PATH) {
    return beerSDToSRGB(srm / 12.7, pathCM);
}

// Determine a color in the sRGB space in relative intensity for a given EBC rating and transmission path in cm (glass diameter)
function ebcToSRGB(ebc, pathCM = DEFAULT_PATH) {
    return beerSDToSRGB(ebc / 25.0, pathCM);
}

// Convert a relative intensity RGB triplet into textual hex representation
function rgbToHex(rgb) {
    let text = '#';
    for (let i = 0; i < rgb.length; ++i) {
        let textPart = Math.round(rgb[i] * 255.0).toString(16);
        if (textPart.length < 2)
            text += '0';
        text += textPart;
    }
    return text;
}
