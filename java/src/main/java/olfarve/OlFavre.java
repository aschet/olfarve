﻿// Ol farve: sRGB color rendering of SRM/EBC beer color ratings
// Copyright 2022 Thomas Ascher <thomas.ascher@gmx.at>
// SPDX-License-Identifier: MIT

package olfarve;

import java.lang.Math;

public final class OlFavre {
	// This data table is composed of the following elements:
	// CIE 1931 colour-matching functions (x_bar, y_bar, z_bar), 2 degree observer, 5 nm
	// https://cie.co.at/datatable/cie-1931-colour-matching-functions-2-degree-observer-5nm
	// CIE standard illuminant D65
	// https://cie.co.at/datatable/cie-standard-illuminant-d65
	private static final double[][] CIE_DATA = {
		{ 380.0, 0.001368, 0.000039, 0.006450, 49.9755 },
		{ 385.0, 0.002236, 0.000064, 0.010550, 52.3118 },
		{ 390.0, 0.004243, 0.000120, 0.020050, 54.6482 },
		{ 395.0, 0.007650, 0.000217, 0.036210, 68.7015 },
		{ 400.0, 0.014310, 0.000396, 0.067850, 82.7549 },
		{ 405.0, 0.023190, 0.000640, 0.110200, 87.1204 },
		{ 410.0, 0.043510, 0.001210, 0.207400, 91.486 },
		{ 415.0, 0.077630, 0.002180, 0.371300, 92.4589 },
		{ 420.0, 0.134380, 0.004000, 0.645600, 93.4318 },
		{ 425.0, 0.214770, 0.007300, 1.039050, 90.057 },
		{ 430.0, 0.283900, 0.011600, 1.385600, 86.6823 },
		{ 435.0, 0.328500, 0.016840, 1.622960, 95.7736 },
		{ 440.0, 0.348280, 0.023000, 1.747060, 104.865 },
		{ 445.0, 0.348060, 0.029800, 1.782600, 110.936 },
		{ 450.0, 0.336200, 0.038000, 1.772110, 117.008 },
		{ 455.0, 0.318700, 0.048000, 1.744100, 117.41 },
		{ 460.0, 0.290800, 0.060000, 1.669200, 117.812 },
		{ 465.0, 0.251100, 0.073900, 1.528100, 116.336 },
		{ 470.0, 0.195360, 0.090980, 1.287640, 114.861 },
		{ 475.0, 0.142100, 0.112600, 1.041900, 115.392 },
		{ 480.0, 0.095640, 0.139020, 0.812950, 115.923 },
		{ 485.0, 0.057950, 0.169300, 0.616200, 112.367 },
		{ 490.0, 0.032010, 0.208020, 0.465180, 108.811 },
		{ 495.0, 0.014700, 0.258600, 0.353300, 109.082 },
		{ 500.0, 0.004900, 0.323000, 0.272000, 109.354 },
		{ 505.0, 0.002400, 0.407300, 0.212300, 108.578 },
		{ 510.0, 0.009300, 0.503000, 0.158200, 107.802 },
		{ 515.0, 0.029100, 0.608200, 0.111700, 106.296 },
		{ 520.0, 0.063270, 0.710000, 0.078250, 104.79 },
		{ 525.0, 0.109600, 0.793200, 0.057250, 106.239 },
		{ 530.0, 0.165500, 0.862000, 0.042160, 107.689 },
		{ 535.0, 0.225750, 0.914850, 0.029840, 106.047 },
		{ 540.0, 0.290400, 0.954000, 0.020300, 104.405 },
		{ 545.0, 0.359700, 0.980300, 0.013400, 104.225 },
		{ 550.0, 0.433450, 0.994950, 0.008750, 104.046 },
		{ 555.0, 0.512050, 1.000000, 0.005750, 102.023 },
		{ 560.0, 0.594500, 0.995000, 0.003900, 100.0 },
		{ 565.0, 0.678400, 0.978600, 0.002750, 98.1671 },
		{ 570.0, 0.762100, 0.952000, 0.002100, 96.3342 },
		{ 575.0, 0.842500, 0.915400, 0.001800, 96.0611 },
		{ 580.0, 0.916300, 0.870000, 0.001650, 95.788 },
		{ 585.0, 0.978600, 0.816300, 0.001400, 92.2368 },
		{ 590.0, 1.026300, 0.757000, 0.001100, 88.6856 },
		{ 595.0, 1.056700, 0.694900, 0.001000, 89.3459 },
		{ 600.0, 1.062200, 0.631000, 0.000800, 90.0062 },
		{ 605.0, 1.045600, 0.566800, 0.000600, 89.8026 },
		{ 610.0, 1.002600, 0.503000, 0.000340, 89.5991 },
		{ 615.0, 0.938400, 0.441200, 0.000240, 88.6489 },
		{ 620.0, 0.854450, 0.381000, 0.000190, 87.69871 },
		{ 625.0, 0.751400, 0.321000, 0.000100, 85.4936 },
		{ 630.0, 0.642400, 0.265000, 0.000050, 83.2886 },
		{ 635.0, 0.541900, 0.217000, 0.000030, 83.4939 },
		{ 640.0, 0.447900, 0.175000, 0.000020, 83.6992 },
		{ 645.0, 0.360800, 0.138200, 0.000010, 81.863 },
		{ 650.0, 0.283500, 0.107000, 0.000000, 80.0268 },
		{ 655.0, 0.218700, 0.081600, 0.000000, 80.1207 },
		{ 660.0, 0.164900, 0.061000, 0.000000, 80.2146 },
		{ 665.0, 0.121200, 0.044580, 0.000000, 81.2462 },
		{ 670.0, 0.087400, 0.032000, 0.000000, 82.2778 },
		{ 675.0, 0.063600, 0.023200, 0.000000, 80.281 },
		{ 680.0, 0.046770, 0.017000, 0.000000, 78.2842 },
		{ 685.0, 0.032900, 0.011920, 0.000000, 74.0027 },
		{ 690.0, 0.022700, 0.008210, 0.000000, 69.7213 },
		{ 695.0, 0.015840, 0.005723, 0.000000, 70.6652 },
		{ 700.0, 0.011359, 0.004102, 0.000000, 71.6091 },
		{ 705.0, 0.008111, 0.002929, 0.000000, 72.979 },
		{ 710.0, 0.005790, 0.002091, 0.000000, 74.349 },
		{ 715.0, 0.004109, 0.001484, 0.000000, 67.9765 },
		{ 720.0, 0.002899, 0.001047, 0.000000, 61.604 },
		{ 725.0, 0.002049, 0.000740, 0.000000, 65.7448 },
		{ 730.0, 0.001440, 0.000520, 0.000000, 69.8856 },
		{ 735.0, 0.001000, 0.000361, 0.000000, 72.4863 },
		{ 740.0, 0.000690, 0.000249, 0.000000, 75.087 },
		{ 745.0, 0.000476, 0.000172, 0.000000, 69.3398 },
		{ 750.0, 0.000332, 0.000120, 0.000000, 63.5927 },
		{ 755.0, 0.000235, 0.000085, 0.000000, 55.0054 },
		{ 760.0, 0.000166, 0.000060, 0.000000, 46.4182 },
		{ 765.0, 0.000117, 0.000042, 0.000000, 56.6118 },
		{ 770.0, 0.000083, 0.000030, 0.000000, 66.8054 },
		{ 775.0, 0.000059, 0.000021, 0.000000, 65.0941 },
		{ 780.0, 0.000042, 0.000015, 0.000000, 63.3828 }
	};

	private static double calcScaleFactor() {
	    double k = 0.0;
	    for (int i = 0; i < CIE_DATA.length; ++i)
	        k += CIE_DATA[i][4] * CIE_DATA[i][2];
	    return 1.0 / k;
	}

	private static final double K = calcScaleFactor();
	
    // The default transmission path in cm. Set to typical sample glass width as specified by the BJCP color guide.
    // https://www.bjcp.org/education-training/education-resources/color-guide
	public static final double DEFAULT_PATH = 5.0;

	private static double summate(double[] t, int cmf) {
	    double sum = 0.0;
	    for (int i = 0; i < CIE_DATA.length; ++i)
	        sum += CIE_DATA[i][4] * t[i] * CIE_DATA[i][cmf];
	    return K * sum;
	}

	private static double correctGamma(double t) {
		if (t <= 0.0031308)
			t = t * 12.92;
		else
			t = 1.055 * Math.pow(t, 1.0 / 2.4) - 0.055;
		return Math.max(0.0, Math.min(1.0, t));
	}

	// Implemented according to A. J. de Lange, "Color," in Brewing Materials and Processes, Elsevier, 2016, pp. 199–249.
	// Color spaced is mapped to sRGB, which requires a scaling of 1.0, instead of CIELAB. For sRGB related transformations
	// see C. Poynton, Digital Video and HD: Algorithms and Interfaces, 2nd ed. Morgan Kaufmann, 2014.
	private static double[] beerSDToSRGB(double a430, double l) {
		double[] t = new double[CIE_DATA.length];
		for (int i = 0; i < CIE_DATA.length; ++i)
			t[i] = Math.pow(10.0, -a430 * l * (0.02465 * Math.exp(-(CIE_DATA[i][0] - 430.0) / 17.591) + 0.97535 * Math.exp(-(CIE_DATA[i][0] - 430.0) / 82.122)));

		double x = summate(t, 1);
		double y = summate(t, 2);
		double z = summate(t, 3);

		double r = correctGamma(x * 3.240479 + y * -1.537150 + z * -0.498535);
		double g = correctGamma(x * -0.969256 + y * 1.875992 + z * 0.041556);
		double b = correctGamma(x * 0.055648 + y * -0.204043 + z * 1.057311);
		double[] rgb = { r, g, b };
		return rgb;
	}

	// Determine a color in the sRGB space in relative intensity for a given SRM rating and transmission path in cm (e.g. glass width)
	public static double[] srmToSRGB(double srm, double pathCM) {
		return beerSDToSRGB(srm / 12.7, pathCM);
	}

	// Determine a color in the sRGB space in relative intensity for a given EBC rating and transmission path in cm (e.g. glass width)
	public static double[] ebcToSRGB(double ebc, double pathCM) {
		return beerSDToSRGB(ebc / 25.0, pathCM);
	}

	// Convert a relative intensity RGB triplet into textual hex representation
	public static String rgbToHex(double[] rgb) {
		String text = "#";
		for (int i = 0; i < rgb.length; ++i)
			text += String.format("%02x", Math.round(rgb[i] * 255.0));
		return text;
	}
}
