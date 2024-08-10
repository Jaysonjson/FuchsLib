/* Jayson */

using System.Diagnostics;
using System.Runtime.InteropServices;

namespace FuchsLib;

public static class Utility
{
    public static void OpenDirectoryInFileManager(string directory)
    {
        if (RuntimeInformation.IsOSPlatform(OSPlatform.Linux))
        {
            Process.Start("gio open " + directory);
        } else if (RuntimeInformation.IsOSPlatform(OSPlatform.Windows))
        {
            Process.Start("explorer \"" + directory + "\"");
        } else if (RuntimeInformation.IsOSPlatform(OSPlatform.OSX))
        {
            Process.Start("open \"" + directory + "\"");
        }
    }

    public static void OpenURLInBrowser(string url)
    {
        RuntimeInformation.IsOSPlatform(OSPlatform.Linux)
        if (RuntimeInformation.IsOSPlatform(OSPlatform.Linux))
        {
            Process.Start("xdg-open " + url);
        } else if (RuntimeInformation.IsOSPlatform(OSPlatform.Windows))
        {
            Process.Start("start " + url);
        } else if (RuntimeInformation.IsOSPlatform(OSPlatform.OSX))
        {
            Process.Start("open " + url);
        }
    }
}