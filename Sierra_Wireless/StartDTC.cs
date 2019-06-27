       //Start DTC Services
       //======================================================================================================
        //Summary:
        //   - Start DTC
        //
        //Notes:
        //   - None.
        //=======================================================================================================
        public bool DTC_Running()
        {
            try
            {
                ServiceController sc = new ServiceController("Distributed Transaction Coordinator");
                sc.Start();
                sc.WaitForStatus(System.ServiceProcess.ServiceControllerStatus.Running, new System.TimeSpan(0, 0, 3));
                if (sc.Status != System.ServiceProcess.ServiceControllerStatus.Running)
                {
                    Console.WriteLine("Failed to start DTC");
                    return false;
                }
            }
            catch (Exception e)
            {
                if (e is InvalidOperationException)
                {
                    Console.WriteLine("DTC service could not be found");
                    return false;
                }
                else
                {
                    Console.WriteLine("Fail: " + e);
                    return false;
                }
            }
            Console.WriteLine("Successfully repaired: DTC now running");
            return true;
        }
