//This code takes in properties and sets them to the wanted values passed in through the wmi query.
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Management;

namespace ConsoleApplication3
{
    class Program
    {

        static bool fix_through_wmi(string query, string property, string value, string method){
            ManagementScope scope = new ManagementScope(@"\root\cimv2");
            scope.Connect();
            ManagementObjectSearcher searcher = new ManagementObjectSearcher(query);
            foreach (ManagementObject obj in searcher.Get())
            {
                ManagementBaseObject inParams = obj.GetMethodParameters(method);
                inParams[Property]=Value;
                ManagementBaseObject outParams= obj.InvokeMethod(method, inParams, null);
                Console.WriteLine(outParams["ReturnValue"]);
            }
            return false;
        }

        static void Main()
        {
            string property="StartMode";
            string query = "Select * from Win32_Service where Name='MSDTC'";
            string value="Auto";
            string method="ChangeStartMode";
            fix_through_wmi(query, property, value, method);
            Console.ReadKey();
        }
    }
}
