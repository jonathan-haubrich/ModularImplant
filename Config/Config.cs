using System;
using System.IO;
using System.Runtime.InteropServices;
using System.Text;

public class Config
{
    public static void Main(string[] args)
    {
        ArgParser parser = new ArgParser("Configures an implant or module");

        parser.AddArgument("t", "target", "The implant or module to be configued", true);
        parser.AddArgument("r", "resource-id", "Resource id for the associated value", true);
        parser.AddArgument("f", "file", "File contents to be embedded");
        parser.AddArgument("s", "string", "String to be embedded");

        parser.Parse(args);

        if(null == parser["f"] && null == parser["s"])
        {
            Console.Error.WriteLine("File contents or string required for embedding");
            return;
        }

        IntPtr hResource = ResourceApi.BeginUpdateResource(parser["t"], false);
        if(IntPtr.Zero == hResource)
        {
            Console.WriteLine("BeginUpdateResource failed: {0}\r\n", Marshal.GetLastWin32Error());
            return;
        }

        IntPtr data = IntPtr.Zero;
        int dataLen = 0;

        if(null != parser["f"])
        {
            byte[] bytes = File.ReadAllBytes(parser["f"]);
            data = Marshal.AllocHGlobal(bytes.Length);
            Marshal.Copy(bytes, 0, data, bytes.Length);
            dataLen = bytes.Length;
        }
        else if(null != parser["s"])
        {
            byte[] wstr = Encoding.Unicode.GetBytes(parser["s"]);
            dataLen = wstr.Length;
            data = Marshal.AllocHGlobal(dataLen);
            Marshal.Copy(wstr, 0, data, dataLen);
        }

        if(false == ResourceApi.UpdateResource(hResource,
            (IntPtr)ResourceApi.RESOURCE_DEFINES.RT_RCDATA,
            ResourceApi.MAKEINTRESOURCE(int.Parse(parser["r"])),
            (ushort)ResourceApi.RESOURCE_DEFINES.LANG_SYSTEM_DEFAULT,
            data,
            (uint)dataLen))
        {
            Console.WriteLine("UpdateResource failed: {0}\r\n", Marshal.GetLastWin32Error());
            Marshal.FreeHGlobal(data);
            return;
        }

        Marshal.FreeHGlobal(data);

        if(false == ResourceApi.EndUpdateResource(hResource, false))
        {
            Console.WriteLine("EndUpdateResource failed: {0}\r\n", Marshal.GetLastWin32Error());
            return;
        }
    }
}