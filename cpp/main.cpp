#include "olfarve.h"
#include <iostream>

int main(int argc, char* argv[]) {
    std::cout << "SRM,sRGB" << std::endl;
    for (int i = 1; i <= 50; ++i)
        std::cout << i << "," << olfarve::rgbToHex(olfarve::srmToSRGB(i)) << std::endl;
	return 0;
}
