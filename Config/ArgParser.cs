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
        AddArgument("h", "help", "Display this message");
    }

    private class Argument
    {
        public string flag { get; }
        public string longname { get; }
        private string description { get; set;  }
        public bool required { get; set; }
        public string value { get; set; }

        public Argument(string flag, string longname, string description, bool required)
        {
            this.flag = flag;
            this.longname = longname;
            this.description = description;
            this.required = required;
        }

        public string CommandStr()
        {
            string commandLine = string.Format("-{0}", flag);

            if (!string.IsNullOrEmpty(longname))
            {
                commandLine += string.Format("/--{0}", longname);
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

        public override string ToString()
        {
            if(null != longname)
            {
                return longname;
            }

            return flag;
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

        foreach(Argument arg in arguments)
        {
            commandLine += string.Format(" {0}", arg.CommandStr());
            options += string.Format("\r\n\t{0}", arg.DescriptionStr());
        }

        System.Console.WriteLine("usage: {0}\r\n\r\n", description);
        System.Console.WriteLine("{0}\r\n\r\n", commandLine);
        if(!string.IsNullOrEmpty(options))
        {
            System.Console.WriteLine("options:\t{0}", options);
        }
    }

    public void Parse(string[] args)
    {
        if(0 == args.Length)
        {
            Usage();
            return;
        }

        for(int i = 0; i < args.Length; ++i)
        {
            if(args[i][0].Equals('-'))
            {
                string value = args[i].Substring(1);
                if(value.Length > 0 && value[0].Equals('-'))
                {
                    value = value.Substring(1);
                }
                Argument arg = arguments.Find((s) => s.flag.Equals(value) || s.longname.Equals(value));
                
                if(null == arg)
                {
                    throw new InvalidArgumentException(string.Format("No option for agument: {0}", value));
                }

                try
                {
                    arg.value = args[i + 1];
                    ++i;
                }
                catch(System.IndexOutOfRangeException)
                {
                    throw new InvalidArgumentException(string.Format("No argument provided for flag: {0}", value));
                }
            }
        }

        foreach(Argument arg in arguments)
        {
            if(arg.required && null == arg.value)
            {
                throw new InvalidArgumentException(string.Format("No argument provided for required argument: {0}", arg));
            }
        }
    }

    public string this[string key]
    {
        get
        {
            string value = null;
            Argument arg = (arguments.Find((s) => s.flag.Equals(key)));
            if (null != arg)
            {
                value = arg.value;
            }

            arg = (arguments.Find((s) => s.longname.Equals(key)));
            if(null != arg)
            {
                value = arg.value;
            }

            return value;
        }
    }
}
