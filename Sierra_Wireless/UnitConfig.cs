/* This code takes in two configuration files and updates their data to be configured for the tests that are about to take place,
   Here is what has been removed:
   !- List of all embbedded modules we produce
   !!- A case for every module that associates it to a special identifier, stored in num
   !!!- Path to config file
*/

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using System.Text.RegularExpressions;

namespace UnitConfig
{
    class Program
    {
        //update file
        static bool update(string unit, string num)
        {
            try
            {
                String[] lines1 = new string[14];
                String[] lines2 = new string[14];
                //update model number first
                //open file
                using (var fileStream = new FileStream(@!!!, FileMode.Open))
                {
                    using (var streamReader = new StreamReader(fileStream, Encoding.UTF8, true, 1024))
                    {
                        // read from file store in array
                        int i = 0;
                        while ((lines1[i] = streamReader.ReadLine()) != null)
                        {
                            i += 1;
                        }

                        //find items needing replacing
                        i = 0;
                        foreach (String line in lines1)
                        {
                            if (line != null)
                            {
                                Regex rex = new Regex(@"(<ProductType>!)(\d{2}-?\d?)(</ProductType>)");
                                Match match = rex.Match(line);
                                if (match.Success)
                                {   //if found replace in array
                                    lines1[i] = match.Groups[1].ToString() + unit + match.Groups[3].ToString();
                                    i += 1;
                                }
                                else
                                {   //if not keep current line
                                    lines1[i] = line;
                                    i += 1;
                                }
                            }
                        }
                    }
                }
                //print result
                using (StreamWriter newTask = new StreamWriter(@ !!!, false))
                {
                    foreach (String line in lines1)
                    {
                        if (line != null)
                        {
                            newTask.WriteLine(line);
                        }
                    }
                }

                //find 
                using (var fileStream = new FileStream(@ !!!, FileMode.Open))
                {
                    using (var streamReader = new StreamReader(fileStream, Encoding.UTF8, true, 1024))
                    {
                        // read from file or write to file
                        int i = 0;
                        while ((lines2[i] = streamReader.ReadLine()) != null)
                        {
                            i += 1;
                        }

                        //find items needing replacing
                        i = 0;
                        foreach (String line in lines2)
                        {
                            if (line != null)
                            {
                                Regex rex = new Regex(@"(<string>)(\w{2}||\w\d)(.*)");
                                Match match = rex.Match(line);
                                if (match.Success)
                                {   //if found replace in array
                                    lines2[i] = match.Groups[1].ToString() + num + match.Groups[3].ToString();
                                    i += 1;
                                }
                                else
                                {   //if not keep current line
                                    lines2[i] = line;
                                    i += 1;
                                }
                            }
                        }
                    }
                }

                //print result
                using (StreamWriter newTask = new StreamWriter(@ !!!, false))
                {
                    foreach (String line in lines2)
                    {
                        if (line != null)
                        {
                            newTask.WriteLine(line);
                        }
                    }
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex);
                return false;
            }
            return true;
        }

        static void Main(string[] args)
        {
            //ask for unit to test
            Console.WriteLine("Which unit are you testing?");
            //store variables
            string name = "";
            var valid = false;
            //options
            string[] arr = new string[] { ! };
            //get user to input one of the above options otherwise reject it
            while (!valid)
            {
                Console.Write("Enter number: ");
                name = Console.ReadLine();
                valid = arr.Contains(name);

                if (!valid)
                    Console.WriteLine("Please enter a valid unit");
            }

            //find the option and get the fsn
            switch (name)
            {
                !!
                default:
                    break;
            }
            //attempt to update config files
            if (update(name, num))
            {
                Console.WriteLine("Close tester and restart, config updated");
                Console.WriteLine("Press Enter to exit.......");
                Console.ReadLine();
            }
            else
            {
                Console.WriteLine("Config failed to update, close and try agian");
                Console.WriteLine("Press Enter to exit.......");
                Console.ReadLine();
            }
        }
    }
}
