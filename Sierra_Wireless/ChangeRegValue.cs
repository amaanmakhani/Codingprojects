//This code takes in a key, a name, and a value to change it in the registry
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Microsoft.Win32;

namespace ConsoleApplication1
{
    class Program
    {
        static void fixkey(string strSubkey, string KeyPairName, int ExpectedValue = 1)
        {
            try
            {
                RegistryKey subKey;
                // Read from 32-bit or 64-bit regristry table
                if (Environment.Is64BitOperatingSystem)
                {
                    subKey = RegistryKey.OpenBaseKey(RegistryHive.LocalMachine, RegistryView.Registry64).OpenSubKey(strSubkey, true);
                }
                else
                {
                    subKey = RegistryKey.OpenBaseKey(RegistryHive.LocalMachine, RegistryView.Registry32).OpenSubKey(strSubkey);
                }
                int KeyPairValue = (int)subKey.GetValue(KeyPairName);

                if (KeyPairValue == ExpectedValue)
                {
                    Console.WriteLine(KeyPairName + " set correctly");
                }
                else
                {
                    subKey.SetValue(KeyPairName, ExpectedValue.ToString(), RegistryValueKind.DWord);
                    Console.WriteLine(KeyPairName + " value fixed to "+ ExpectedValue.ToString());
                }
                subKey.Close();
            }
            catch (Exception e)
            {
                Console.WriteLine("Exception: " + e);
            }
        }

        static void Main()
        {
            const string strSubkey = @"SOFTWARE\Microsoft\MSDTC\Security";
            const string KeyPairName="NetworkDtcAccessClients";
            int ExpectedValue=1;
            fixkey(strSubkey,KeyPairName);
            Console.ReadKey();
        }
    }
}
