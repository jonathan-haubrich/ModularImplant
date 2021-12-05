using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

class ArgParser
{
    private List<Argument> arguments;
    private string description;
    public ArgParser(string description)
    {
        this.description = description;
        arguments = new List<Argument>();
    }

    private class Argument
    {
        private string flag { get; set; }
        private string longname { get; set; }
        private string description { get; set;  }
        private bool required { get; set; }
        private bool positional { get; set; }
        private string value { get; set; }

        public Argument(string flag, string longname, string description, bool required)
        {
            this.flag = flag;
            this.longname = longname;
            this.description = description;
            this.required = required;
            
            positional = false;
            if(flag.Contains("-"))
            {
                positional = true;
            }
        }

        public bool IsOptional()
        {
            return !required;
        }

        public bool IsPositional()
        {
            return positional;
        }

        public string CommandStr()
        {
            string commandLine = string.Format("-{0}", flag);

            if (!string.IsNullOrEmpty(longname))
            {
                commandLine += string.Format("/--{0}", longname);
            }

            if (IsOptional())
            {
                commandLine = "[ " + commandLine;
                commandLine += " ]";
            }

            return commandLine;
        }

        public string DescriptionStr()
        {
            string options = string.Format("-{0}", flag);
                
            if(!string.IsNullOrEmpty(longname))
            {
                options += string.Format(", --{0}", longname);
            }

            options += string.Format("\t\t{0}", description);

            return options;
        }
    }

    public void AddArgument(string flag, string description, bool required = false)
    {
        AddArgument(flag, "", description, required);
    }

    public void AddArgument(string flag, string longname, string description, bool required = false)
    {
        Argument arg = new Argument(flag, longname, description, required);
        arguments.Add(arg);
    }

    public void Usage()
    {
        string commandLine = System.AppDomain.CurrentDomain.FriendlyName;
        string options = "";
        string positionals = "";

        foreach(Argument arg in arguments)
        {
            commandLine += string.Format(" {0}", arg.CommandStr());
            if(arg.IsPositional())
            {
                positionals += string.Format("\r\n\t{0}", arg.DescriptionStr());
            }
            else
            {
                options += string.Format("\r\n\t{0}", arg.DescriptionStr());
            }
        }

        System.Console.WriteLine("usage: {0}\r\n\r\n", description);
        System.Console.WriteLine("{0}\r\n\r\n", commandLine);
        if(!string.IsNullOrEmpty(positionals))
        {
            System.Console.WriteLine("positionals:\t{0}", positionals);
        }
        if(!string.IsNullOrEmpty(options))
        {
            System.Console.WriteLine("options:\t{0}", options);
        }
    }

    public void Parse(string[] args)
    {
        
    }
}
