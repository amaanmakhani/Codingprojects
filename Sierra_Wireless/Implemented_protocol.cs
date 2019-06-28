        /*
        This code checks all the communication protocols needed for server connection. Backstory, this can be done easily
        with Microsoft.SqlServer.Management.Smo.Wmi,however, there was a repated error that could not be resolved. That
        lead to this convuluted code.
        Here is what was removed:
        !-SQL Paths
        */

        //======================================================================================================
        //Summary:
        //   - Check Inter-process communication protocols is configured to enable Shared Memory, Named Pipes, and TCP/IP protocols.
        //
        //Notes:
        //   - Return true if protocols is configured correctly otherwise false.
        //=======================================================================================================
        /*Naming Convention are as follows:
        * Protocol Specific variables are named with the named and pourpose following
        * Shared_(pourpose):Shared protocol testing
        * Named_(pourpose): Named protocol testing
        * Tcp_(pourpose):TCP/IP protocol testing
        */
        //Create a class variable for process, Error message, and timer(create them later before use), this happens for each protocol
        static Process Shared_Process = new Process();
        static Process Named_Process = new Process();
        static Process Tcp_Process = new Process();
        static char[] Shared_ErrorMessage = new char[6];
        static char[] Named_ErrorMessage = new char[6];
        static char[] Tcp_ErrorMessage = new char[6];
        static Timer Shared_Timer = null;
        static Timer Named_Timer = null;
        static Timer Tcp_Timer = null;
        //Holds all messages for the end
        static int[] StatusMessage = new int[3];
        //Create flags so yu know when async done
        static bool Shared_Finished = false;
        static bool Tcp_Finished = false;
        static bool Named_finished = false;
        //Create varaibe to store const computer, name and TCP port number 
        static string computer = Environment.MachineName;
        static string tcp_port = null;

        //create timed event handler for protocols above, respectivley
        private static void Shared_TimedEvent(Object source, ElapsedEventArgs E)
        {
            //stop the timer and get rid of it
            Shared_Timer.Stop();
            Shared_Timer.Dispose();

            //check if it is empty or null
            if (Shared_ErrorMessage[0] != '0')
            {   //If standard error has something failed to connect
                StatusMessage[0] = 0;
                Shared_Finished = true;
                return;
            }

            //If empty connectd using protocol
            StatusMessage[0] = 1;
            Shared_Finished = true;
            return;
        }

        private static void Named_TimedEvent(Object source, ElapsedEventArgs E)
        {
            //stop the timer and get rid of it
            Named_Timer.Stop();
            Named_Timer.Dispose();

            //check if it is empty or null
            if (Named_ErrorMessage[0] != '0')
            {   //If standard error has something failed to connect
                StatusMessage[1] = 0;
                Named_finished = true;
                return;
            }

            //If empty connectd using protocol
            StatusMessage[1] = 1;
            Named_finished = true;
            return;
        }

        private static void Tcp_TimedEvent(Object source, ElapsedEventArgs E)
        {
            //stop the timer and get rid of it
            Tcp_Timer.Stop();
            Tcp_Timer.Dispose();

            //check if it is empty or null
            if (Tcp_ErrorMessage[0] != '0')
            {   //If standard error has something failed to connect
                StatusMessage[2] = 0;
                Tcp_Finished = true;
                return;
            }

            //If empty connectd using protocol
            StatusMessage[2] = 1;
            Tcp_Finished = true;
            return;
        }

        public bool Check_SQL_Protocol(Action<String> log_messages)
        {    //Shared protocol testing
            try
            {
                //create process and settings
                ProcessStartInfo startInfo = new ProcessStartInfo();
                startInfo.FileName = "cmd.exe";
                //command to connect using protocol
                startInfo.Arguments = "/C sqlcmd -l 1 -S lpc:" + computer + !;
                startInfo.RedirectStandardOutput = true;
                startInfo.RedirectStandardError = true;
                startInfo.UseShellExecute = false;
                startInfo.CreateNoWindow = true;
                Shared_Process.StartInfo = startInfo;
                //create timer and configure
                Shared_Timer = new Timer(2000);
                Shared_Timer.AutoReset = false;
                Shared_Timer.Enabled = true;
                Shared_Timer.Elapsed += new ElapsedEventHandler(Shared_TimedEvent);
                //start process and timer
                Shared_ErrorMessage[0] = '0';
                Shared_Timer.Start();
                Shared_Process.Start();
                Shared_Process.StandardError.ReadAsync(Shared_ErrorMessage, 0, 5);
            }
            catch (Exception ex)
            {
                // Log the exception
                log_messages("Fail: Exception shared protocol testing: " + ex.Message);
                StatusMessage[0] = 0;
            }

            //Named protocol testing
            try
            {   //create process and settings
                ProcessStartInfo startInfon = new ProcessStartInfo();
                startInfon.FileName = "cmd.exe";
                //command to connect using protocol
                startInfon.Arguments = "/C sqlcmd -l 1 -S np:\\\\" + computer + !;
                startInfon.RedirectStandardOutput = true;
                startInfon.RedirectStandardError = true;
                startInfon.UseShellExecute = false;
                startInfon.CreateNoWindow = true;
                Named_Process.StartInfo = startInfon;
                //create timer and configure
                Named_Timer = new Timer(2000);
                Named_Timer.AutoReset = false;
                Named_Timer.Enabled = true;
                Named_Timer.Elapsed += new ElapsedEventHandler(Named_TimedEvent);
                //start process and timer
                Named_ErrorMessage[0] = '0';
                Named_Timer.Start();
                Named_Process.Start();
                Named_Process.StandardError.ReadAsync(Named_ErrorMessage, 0, 5);
            }
            catch (Exception ex)
            {   // Log the exception
                log_messages("Fail: Exception named protocol testing: " + ex.Message);
                StatusMessage[1] = 0;
            }

            //GET TCP port number through query
            try
            {   //store result in list
                List<string> Result = new List<string>();
                //query and connection string variables
                string Command = "DECLARE @portNo NVARCHAR(10) EXEC   xp_instance_regread @rootkey = 'HKEY_LOCAL_MACHINE', @key = 'Software\\Microsoft\\Microsoft SQL Server\\MSSQLServer\\SuperSocketNetLib\\Tcp\\IpAll', @value_name = 'TcpDynamicPorts', @value = @portNo OUTPUT SELECT [PortNumber] = @portNo;";
                string ConnectionString = "server=(local) ! ;Trusted_Connection=yes";
                //create connection class to sql server
                using (SqlConnection mConnection = new SqlConnection(ConnectionString))
                {   //open connection 
                    mConnection.Open();
                    //run command store query result in result
                    using (SqlCommand cmd = new SqlCommand(Command, mConnection))
                    {   //read output
                        using (SqlDataReader reader = cmd.ExecuteReader())
                        {
                            while (reader.Read())
                            {
                                Result.Add(reader.GetValue(0).ToString());
                            }
                        }
                    }
                }
                //remove duplicates
                Result = Result.Distinct().ToList();
                foreach (string elem in Result)
                {   //output numbers produced
                    log_messages("tcp port: "+ elem);
                }
                if (Result.Count != 0)
                {   //assign it to tcp_port
                    tcp_port = Result[0];
                }
                else
                {
                    StatusMessage[2] = 0;
                    log_messages("FAILED TO CONNECT USING TCP PROTOCOL: UNABLE TO GET TCP PORT");
                    Tcp_Finished = true;
                }
            }
            catch (Exception ex)
            {   //log the exception
                Tcp_Finished = true;
                StatusMessage[2] = 0;
                log_messages("Fail: Exception when finding TCP port: " + ex.Message);
            }

            //TCP protocol testing
            try
            {
                if (!Tcp_Finished)
                {
                    //create process and settings
                    ProcessStartInfo startInfotcp = new ProcessStartInfo();
                    startInfotcp.FileName = "cmd.exe";
                    //command to connect using protocol
                    startInfotcp.Arguments = "/C sqlcmd -l 1 -S tcp:" + computer + "," + tcp_port;
                    startInfotcp.RedirectStandardOutput = true;
                    startInfotcp.RedirectStandardError = true;
                    startInfotcp.UseShellExecute = false;
                    startInfotcp.CreateNoWindow = true;
                    Tcp_Process.StartInfo = startInfotcp;
                    //create timer and configure
                    Tcp_Timer = new Timer(2000);
                    Tcp_Timer.AutoReset = false;
                    Tcp_Timer.Enabled = true;
                    Tcp_Timer.Elapsed += new ElapsedEventHandler(Tcp_TimedEvent);
                    //start process and timer
                    Tcp_ErrorMessage[0] = '0';
                    Tcp_Timer.Enabled = true;
                    Tcp_Timer.Start();
                    Tcp_Process.Start();
                    Tcp_Process.StandardError.ReadAsync(Tcp_ErrorMessage, 0, 5);
                }
            }
            catch (Exception ex)
            {   // Log the exception
                log_messages("Fail: Exception TCP protocol testing: " + ex.Message);
            }

            //wait for finish of Timed Events
            while (!Shared_Finished && !Named_finished && !Tcp_Finished) { }
            //ensure written in array
            System.Threading.Thread.Sleep(2500);
            //Identify and failures
            if (StatusMessage[0] == 0)
            {
                log_messages("Fail: Failed to connect via shared protocol");
            }
            if (StatusMessage[1] == 0)
            {
                log_messages("Fail: Failed to connect via named protocol");
            }
            if (StatusMessage[2] == 0)
            {
                log_messages("Fail: Failed to connect via TCP protocol");
            }
            //Cases
            if (StatusMessage[0] == 1 && StatusMessage[1] == 1 && StatusMessage[2] == 1)
            {
                log_messages("Protocols configured correctly");
                return true;
            }
            else
            {
                log_messages("Fail: Protocols configured incorrectly");
                return false;
            }
        }
