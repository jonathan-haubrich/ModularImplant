using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

public class InvalidArgumentException : Exception
{
    public InvalidArgumentException(string message) : base(message) { }
}
