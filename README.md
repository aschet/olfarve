# Øl farve: SRM/EBC sRGB beer color simulation for everyone

<p align="center">
  <img src="/colors.png?raw=true">
</p>

Brewing calculators typically display a color impression for the estimated SRM/EBC value of a beer recipe. The color is determined either by a fixed table or a mathematical function, whereby in both cases the data source is usually the measured colors of a beer color chart. But how are the colors of such a chart put together? In the case of the [BJCP Color Guide](https://www.bjcp.org/education-training/education-resources/color-guide), a mathematical model based on the spectral analysis of 99 beers was used:

$$T(\lambda)=\log^{-1}\left(-\frac{\text{SRM}}{12.7}l\left(0.02465e^{-\frac{\lambda-430}{17.591}}+0.97535e^{-\frac{\lambda-430}{82.122}}\right)\right)$$

This model was presented in great detail by A. J. deLange in [Bamforth's Brewing Materials and Processes](http://dx.doi.org/10.1016/B978-0-12-799954-8.00011-3). Discussions about its implementation have already taken place 10 years ago on the [HomebrewTalk](https://www.homebrewtalk.com/threads/on-the-calculation-of-srm-rgb-values-in-the-srgb-color-space.413581) forum. However, it does not seem to have been widely adopted in the homebrewing community. One reason for this is likely to be the associated implementation complexity. To overcome this difficulty, simplified example implementations written in several programming languages have been provided in this repository. These show how an SRM/EBC value can be converted to an sRGB value by reconstructing a transmission spectrum using the above model. The result is constrained by the gamut of the sRGB color space and the gamut of your monitor, as well as the deviation of the actual beer from the average spectrum of the 99 model beers. Furthermore, with SRM/EBC measurements, it can happen that [two differently colored beer samples give the same measured value](https://www.brewingwithbriess.com/blog/predicting-beer-color-based-on-formulation).

Parameters that affect the perception of beer color include ambient light and the transmittance of light. The larger the width of a glass, the darker the beer inside appears. The deLange model can take this effect into account by means of a scaling factor (path). In the sample programs, this is preset to 5 cm following the BJCP Color Guide.

If you would like to help improve the quality of the code in this repository, please create a pull request!
