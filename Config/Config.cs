using System;

public class Config
{
    public static void Main(string[] args)
    {
        ArgParser parser = new ArgParser("Configures an implant or module");

        parser.AddArgument("l", "loader", "File name of the loader module to be embedded");
        parser.AddArgument("c", "comms", "File name of the comms module to be embedded");

        parser.Usage();
    }
}