using System;
using System.IO.Compression;
using System.Windows.Forms;

namespace UnzipNetFrm
{
    class Program
    {
        static void Main(string[] args)
        {
            string fileName = "testZip" + args[0];
            string startPath = @"..\Zip\" + fileName;
            string zipPath = @"..\Zip\" + fileName + ".zip";
            string extractPath = @"..\Unzip\" + fileName;

            //ZipFile.CreateFromDirectory(startPath, zipPath);
            ZipFile.ExtractToDirectory(zipPath, extractPath);
        }
    }
}
