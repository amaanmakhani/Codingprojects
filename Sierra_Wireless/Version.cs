/* This code uses Windows Registry to get the driver version of a specific driver.
   Here is what was removed:
   !- Driver version
   !!- Sierra Wireless key
   !!!-Driver name
*/

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Management;
using Microsoft.Win32;

namespace TestDriver
{
    class Driver
    {
        public static void Main()
        {//Declare the string to hold the version wanted
            string WantedSoftware = "!";
            string CurrentVersion = null;
            //The registry key:
            string SoftwareKey = @"!!";
            using (RegistryKey rk = Registry.LocalMachine.OpenSubKey(SoftwareKey))
            {//Go through the registry keys and get the info we need:
                foreach (string skName in rk.GetSubKeyNames())
                {
                    using (RegistryKey sk = rk.OpenSubKey(skName))
                    {//Here is where the software keys are stored
                        try
                        {
                            //If the key has value and it is the Sierra Wireless package, continue, if not, skip it:
                            if (!(sk.GetValue("DisplayName") == null) && sk.GetValue("DisplayName").ToString() == "!!!")
                            {
                                Console.WriteLine("Key: {0}, Version: {1}", sk.GetValue("DisplayName"), sk.GetValue("DisplayVersion"));
                                CurrentVersion = sk.GetValue("DisplayVersion").ToString();
                            }
                        }
                        catch (Exception ex)
                        {
                            Console.WriteLine("Failed to get version of driver, exception:{0}", ex.Message);
                            //If in function return false
                        }
                    }
                }
            }
            if (WantedSoftware != CurrentVersion)
            {
                Console.WriteLine("Versions don't match, unable to use");
                //If in function return false
            }
            else
            {
                Console.WriteLine("Versions match, good to go");
                //If in function return true
            }
            Console.ReadKey();
        }
    }

}
