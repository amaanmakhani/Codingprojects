# This report is meant to use the studentdb on linux.csc.uvic.ca server
# By using the command below on the server:
# python3 A4.py 'Nolan, Christopher (I)' > OutputReportGenerated.html

# You will be prompted to enter the following information:
# Enter your name: 
# <Enter your username here>
# Enter your password: 
# <Enter your password here>

# If the login is successful OutputReportGenerated.html is generated
# This code uses strategies from https://realpython.com/prevent-python-sql-injection/ to prevent an sql injection attack

import psycopg2
import sys

if len(sys.argv) != 2:
    print("Missing argument refering to pid")
    sys.exit()

print("Enter your name: ", file=sys.stderr)
name = input()
print("Enter your password: ", file=sys.stderr)
passwd = input()

try:
    connection = psycopg2.connect(user = str(name),
                                  password = str(passwd),
                                  host = "studentdb.csc.uvic.ca",
                                  port = "5432",
                                  database = "imdb")

    cursor = connection.cursor()
    # Used execute's additional parameter to escape any queries hidden in passed in string
    # This prevents an sql injection attack
    cursor.execute("SELECT * FROM am_udfA4(%(pid)s);", {'pid': sys.argv[1]})
    result = cursor.fetchall()

    print("<html>","<head>","<style>","table, td {","border: 1px solid black;","border-collapse: collapse;","}","th{background-color: #4CAF50;}","tr:nth-child(even){background-color: #f2f2f2;}","tr:hover {background-color: #ddd;}","</style>","</head>","<body>",sep="\n")
    print("<p>","Director:", sys.argv[1],"</p>")
    print("<br>")
    print("<table style=\"width:100%\">","<tr>","<th>id</th>","<th>year</th>","<th>rank</th>","<th>votes</th>","</tr>",sep="\n")
    for row in result:
        print("<tr>")
        print("<td>", row[0], "</td>")
        print("<td style=\"text-align:right\">", row[1], "</td>")
        if row[2] != None:
            print("<td style=\"text-align:right\">", row[2], "</td>")
        else:
            print("<td style=\"text-align:right\">", "</td>")
        if row[3] != None:  
            print("<td style=\"text-align:right\">", row[3], "</td>")
        else:
            print("<td style=\"text-align:right\">", "</td>")
        print("</tr>")
    print("</table>")
    print("<br>")
    print("<p>","Total:", len(result),"movies","</p>")
    print("</body>")
    print("</html>")
except (Exception, psycopg2.Error) as error :
    print ("Error while connecting to PostgreSQL", error)
finally:
    #closing database connection.
    if(connection):
        cursor.close()
        connection.close()
        #Connection closed