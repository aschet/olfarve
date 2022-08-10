using System;

internal class Program
{
    static void Main(string[] args)
    {
        Console.WriteLine("SRM,sRGB");
        for (int i = 1; i <= 50; ++i)
            Console.WriteLine($"{i},{OlFarve.RGBToHex(OlFarve.SRMToSRGB(i))}");
    }
}
