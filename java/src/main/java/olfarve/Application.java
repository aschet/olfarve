package olfarve;

public class Application {
	public static void main(String[] args) {
		System.out.println("SRM,sRGB");
        for (int i = 1; i <= 50; ++i)
        	System.out.println(Integer.toString(i) + "," + OlFavre.rgbToHex(OlFavre.srmToSRGB(i, OlFavre.DEFAULT_PATH)));
	}
}
