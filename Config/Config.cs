using System;

public class Config
{
    public static void Main(string[] args)
    {
        int idx = Array.FindIndex(args, s => s == "-s");

        try
        {
            Console.WriteLine("Found arg: {0}", args[idx + 1]);
        }
        catch(System.IndexOutOfRangeException)
        {
            Console.Error.WriteLine("No arg provided to -s flag");
        }
    }
}