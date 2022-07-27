# Øl farve: beautiful beer colors for everyone

![Beer Colors](/colors.png?raw=true)

Brewing calculators typically display a color impression for the estimated SRM/EBC value of a beer recipe. The color is determined either by a fixed table or a mathematical function, whereby in both cases the data source is usually the measured colors of a beer color chart. But how are the colors of such a chart put together? In the case of the [BJCP Color Guide](https://www.bjcp.org/education-training/education-resources/color-guide), a mathematical model based on the spectral analysis of 99 beers was used:

![Model](/model.png?raw=true)

This model was presented in great detail by A.J. deLange in [Bamforth's Brewing Materials and Processes](http://dx.doi.org/10.1016/B978-0-12-799954-8.00011-3). Discussions about its implementation have already taken place 10 years ago on the [HomebrewTalk](https://www.homebrewtalk.com/threads/on-the-calculation-of-srm-rgb-values-in-the-srgb-color-space.413581) forum. However, it does not seem to have been widely adopted in the homebrewing community. One reason for this is likely to be the associated implementation complexity. To overcome this difficulty, simplified example implementations written in several programming languages have been provided in this repository. These demonstrate how, using the model shown, an sRGB value can be reconstructed from an SRM/EBC value. The quality of the color reconstruction depends on the spectral properties of the beer in question. If these correspond to the model mean, a good result can tend to be expected. However, due to the nature of SRM/EBC measurements, [two beer samples with different colors can generate the same reading](https://www.brewingwithbriess.com/blog/predicting-beer-color-based-on-formulation). The bad news: The sRGB color space and thus computer monitors can only reproduce beer colors inadequately.

Parameters that affect the perception of beer color include ambient light and the transmittance of light. The larger the diameter of a glass, the darker the beer inside appears. The model of deLange et al. can take this effect into account by a scaling factor (path). In the example programs, it is preset to 5 cm according to the BJCP Color Guide.

If you would like to help improve the quality of the code in this repository, please create a pull request!
