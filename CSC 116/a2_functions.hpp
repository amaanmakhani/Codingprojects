/* a2_functions.h
   CSC 116 - Fall 2018

   Prototypes for the A2 data processing types and functions.

   Do not modify any of the contents of this file (any such modifications will
   be discarded during marking).

   B. Bird - 09/16/2018
*/

#include <vector>
#include <string>


/* A structure type to represent a year/month/day combination */
struct Date {
	int day{ 0 };
	int month{ 0 };
	int year{ 0 };
};

/* A structure type to represent an hour:minute pair */
struct TimeOfDay {
	int hour{ 0 };
	int minute{ 0 };
};

/* A structure type to store the parameters of a particular sailing */
struct Sailing {
	int route_number{ 0 };
	std::string source_terminal{ "" };
	std::string dest_terminal{ "" };
	std::string vessel_name{ "" };

	Date departure_date{};
	TimeOfDay scheduled_departure_time{};

	int expected_duration{ 0 };
	int actual_duration{ 0 };
};

/* A structure type to store aggregated performance data for a single
   route. */
struct RouteStatistics {
	int route_number{ 0 };
	int total_sailings{ 0 };
	int late_sailings{ 0 };
};

/* A structure type to store aggregated performance data for a single
   day. */
struct DayStatistics {
	Date date{};
	int total_sailings{ 0 };
	int late_sailings{ 0 };
};

/* Function prototypes */
/* Put your implementations of these functions in a2_functions.cpp */

/* Functions to implement */

/* parse_sailing(input_line, result)
   Given a string containing a line from the input file, convert the
   data in the line to a Sailing instance and store it in the Sailing
   struct provided.

   Return value:
	 - true if parsing succeeds (and the result struct is valid).
	 - false if parsing fails for any reason.
   This function must not throw any exceptions (directly or indirectly).
*/
bool parse_sailing(const std::string& input_line, Sailing& result);


/* performance_by_route(sailings)
   Given a vector of Sailing instances (in no particular order), return
   a vector of RouteStatistics instances, with exactly one entry in the result
   vector for each unique route number in the input vector.

   To be clear, there may be many sailings in the input vector with a particular
   route number (e.g. 3), but each route number must appear in only one entry
   of the result vector.

   Each RouteStatistics object will also contain the total number of sailings
   and total number of late sailings on each route.

   Recall from the specification that a "late" sailing is any sailing
   that took at least five minutes longer than expected. That is, a sailing
   where
	 actual_duration >= expected_duration + 5

   Return value:
	 A vector of RouteStatistics objects, in any order, where each route number
	 appearing in the input vector is represented by exactly one RouteStatistics
	 instance in the result.

*/
std::vector<RouteStatistics> performance_by_route(const std::vector<Sailing>& sailings);

/* fastest_sailings(sailings)
   Define the "speed" of a sailing to be the quantity
	 expected_duration/actual_duration
   Under this definition, when speed > 1, the sailing took less time than expected,
   and when speed < 1, the sailing was slower than expected.

   This function takes a vector of Sailing objects and returns a vector containing
   all Sailing objects which have the _maximum_ speed value. Note that there may
   be multiple sailings which all have the maximum speed (so it's impossible to
   assume that there will be a single fastest sailing).

   Return value:
	 A vector of Sailing objects (which will be copies of some of the Sailing
	 objects in the input vector) which have the maximum "speed" value as
	 defined above.
*/
std::vector<Sailing> fastest_sailings(const std::vector<Sailing>& sailings);


/* slowest_sailings(sailings)
   This function takes a vector of Sailing objects and returns a vector of all
   Sailing objects which have the _minimum_ speed value.
   See the description of fastest_sailings for details.

   Return value:
	 A vector of Sailing objects (which will be copies of some of the Sailing
	 objects in the input vector) which have the maximum "speed" value as
	 defined above.
*/
std::vector<Sailing> slowest_sailings(const std::vector<Sailing>& sailings);


/* best_days(sailings)
   Recall that a sailing is considered "late" if
	actual_duration >= expected_duration + 5

   Generally, a day with a large number of late sailings is a "bad day" for
   travellers.

   The "best days" are any days in the dataset for which the ratio
	 late_sailings/total_sailings
   is the lowest (in the best possible case, there will be zero late sailings,
   and the ratio will equal zero).

   Given a vector of Sailing objects, this function returns a vector of
   DayStatistics objects for all of the "best days" in the dataset, which
   are all days in the dataset that have the lowest ratio of late sailings
   to total sailings.
   Note that there may be multiple such days.
*/
std::vector<DayStatistics> best_days(const std::vector<Sailing>& sailings);


/* worst_days(sailings)
   See the description of best_days().

   The "worst days" are any days in the dataset for which the ratio
	 late_sailings/total_sailings
   is the highest (in the worst possible case, every sailing is late,
   and the ratio will equal one).

   Given a vector of Sailing objects, this function returns a vector of
   DayStatistics objects for all of the "worst days" in the dataset,which
   are all days in the dataset that have the highest ratio of late sailings
   to total sailings.
   Note that there may be multiple such days.
*/
std::vector<DayStatistics> worst_days(const std::vector<Sailing>& sailings);


/* Provided functions (already implemented in a2_functions.cpp) */
/* You do not have to understand or modify these functions (although they
   are of the same level of difficulty as the other parts of the assignment) */
std::vector<Sailing> read_sailings(const std::string& input_filename);
void print_sailing(const Sailing& sailing);