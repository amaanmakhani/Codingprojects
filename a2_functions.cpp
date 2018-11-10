/*
Amaan Makhani
V00883520
created for October 14, 2018
CSC 116 Assignment 2
This file contains functions to help analzye data inputted on ferry routes
and to complete analysis related to different stats.
*/

#include <iostream> //For std::cout and std::cin
#include <fstream> //Needed by read_sailings to work with files
#include <stdexcept> //Needed by read_sailings to handle exceptions
#include <iomanip> //Needed by print_sailing to format output
#include <vector>
#include <string>
#include "a2_functions.hpp"


/* Required Functions */

bool parse_sailing(const std::string& input_line, Sailing& result) {
	std::string info;													//makes a string called information to push back data into
	std::string leftover;												//to store remaining parts of the string
	int comma{};														//counts the number of commas
	comma = 0;
	try {																//able to catch exceptions
		for (auto information : input_line) {							//pushes back information into a string as long as commas isnt at 10
			if (information != ','&&comma != 10) {
				info.push_back(information);
			}

			if (information == ',') {									//places information based on the amount of commas
				comma++;
				if (comma == 1) {
					result.route_number = std::stoi(info);
				}
				if (comma == 2) {
					result.source_terminal = info;
				}
				if (comma == 3) {
					result.dest_terminal = info;
				}
				if (comma == 4) {
					result.departure_date.year = std::stoi(info);
				}
				if (comma == 5) {
					result.departure_date.month = std::stoi(info);
				}
				if (comma == 6) {
					result.departure_date.day = std::stoi(info);
				}
				if (comma == 7) {
					result.scheduled_departure_time.hour = std::stoi(info);
				}
				if (comma == 8) {
					result.scheduled_departure_time.minute = std::stoi(info);
				}
				if (comma == 9) {
					result.vessel_name = info;
				}
				if (comma == 10) {
					result.expected_duration = std::stoi(info);
				}

				info.clear();
				continue;
			}

			if (information != ','&& comma == 10) {							//leftover after comma 10 is dealt with
				leftover.push_back(information);
			}
		}
		result.actual_duration = std::stoi(leftover);
		if (comma != 10) {													//if not enough info throw exception
			throw std::runtime_error("Not enough data");
		}
		return true;														//if nothing goes wrong return true
	}
	catch(std::exception const& e){											//if exception is thrown return false											
	return false;
	}
}


std::vector<RouteStatistics> performance_by_route(const std::vector<Sailing>& sailings) {
	std::vector<RouteStatistics> unique_routes;								//create vector of routestats to return
														//use struct to add info into return vector
	bool late = false;														//flag to show if late
	int routenumber;														//to hold routenumber
	bool notexisting;														//to prove existence
	
	for (unsigned int i = 0; i <= sailings.size()- 1; i++) {
		routenumber = 0;													//reset routenumber
		late = false;														//reset late flag
		notexisting = true;													//reset existence 
		routenumber=sailings.at(i).route_number;							//routenumber equals current vector routenumber
	
		if (sailings.at(i).actual_duration >= sailings.at(i).expected_duration + 5) {		//see if sailing was late
			late = true;
		}

		
		for (auto & elem : unique_routes) {									//for all elements in the unique vector see if the current sailing is in there
			if (elem.route_number == routenumber) {							//check if the route is equal to the current route
				elem.total_sailings += 1;									//add one to sailing
				if (late) {
					elem.late_sailings += 1;								//if late add one late sailing
				}
				notexisting = false;										//show it exists and exit loop
				break;
			}
			
		}

		if (notexisting) {													//if not existing
			RouteStatistics route;
			route.route_number = routenumber;								//make route number equal to route number and add data to a struct
			route.total_sailings = 1;
			if (late) {
				route.late_sailings = 1;
			}
			unique_routes.push_back(route);									//push struct back into vector of structs
		}
	}	
	return unique_routes;
}


std::vector<Sailing> fastest_sailings(const std::vector<Sailing>& sailings) {
	float speed;						//store speed
	std::vector<Sailing> fastest;		//vector of struct sailing and return vector
	std::vector<float> speeds;			//vector to store speeds
	std::vector<int> index;				//vector to store index of fastest sailings
	
	for (unsigned int i = 0; i <= sailings.size() - 1; i++) {
	speed = (float)sailings.at(i).expected_duration / (float)sailings.at(i).actual_duration;		//calculate speed of each sailing
	speeds.push_back(speed);																		//pushback into vector
	}

	float max = speeds.at(0);																		//set max at first element
	for (unsigned int i = 0; i <= speeds.size() - 1; i++) {
		if (speeds.at(i)>= max) {																	//if speed is greater than max it now becomes max		
			max = speeds.at(i);	
		}
	}
	for (unsigned int i = 0; i <= speeds.size() - 1; i++) {											//store index of all sailings with max speed
		if (speeds.at(i) == max) {
		index.push_back(i);
		}
	}

	for (auto elem : index) {																		//pushback element of vector at max spped indexes and return it
		fastest.push_back(sailings.at(elem));
	}

	return fastest;
}


std::vector<Sailing> slowest_sailings(const std::vector<Sailing>& sailings) {		//same as previous function
	float speed;
	std::vector<Sailing> slowest;
	std::vector <float> speeds;
	std::vector<int> index;

	for (unsigned int i = 0; i <= sailings.size() - 1; i++) {
		speed = (float)sailings.at(i).expected_duration / (float)sailings.at(i).actual_duration;
		speeds.push_back(speed);
	}

	float min = speeds.at(0);
	for (unsigned int i = 0; i <= sailings.size() - 1; i++) {
		if (speeds.at(i) <= min) {
			min = speeds.at(i);
		}
	}
	for (unsigned int i = 0; i <= sailings.size() - 1; i++) {
		if (speeds.at(i) == min) {
		index.push_back(i);
		}
	}
	

	for (auto elem : index) {
		slowest.push_back(sailings.at(elem));
	}

	return slowest;
}


std::vector<DayStatistics> best_days(const std::vector<Sailing>& sailings) {
	std::vector<DayStatistics> unique_days;							//vector of type daystatistics for sailings each day											
	bool late = false;												//flag to see if late
	int month;														//store month,day and year in their own variables 
	int date;
	int year;
	bool notexisting;												//if existing flag

	std::vector<DayStatistics> bestdays;							//vector of type daystats to hold best sailings days
	float bestdaysratio;											//store ratio and vector to store ration of late/total
	std::vector<float> ratios;
	std::vector<int> index;											//store indexes of best days
	DayStatistics trip;												//struct to pusbhb back into the vector

	for (unsigned int i = 0; i <= sailings.size() - 1; i++) {
		late = false;							//reset flag of existence and late
		notexisting = true;
		
		month = sailings.at(i).departure_date.month;				//set month to month of current iteration
		date= sailings.at(i).departure_date.day;					//set date to date of current iteration
		year = sailings.at(i).departure_date.year;					//set year to year of current iteration

		if (sailings.at(i).actual_duration >= (sailings.at(i).expected_duration + 5)) {		//see if sailing is late if it is change flag
			late = true;
		}

		for (auto & elem : unique_days) {													//for every element in unique vector check if its in there
			if (elem.date.day==date && elem.date.month==month && elem.date.year==year) {		//if element in vector is same day as current iteration change totals
				elem.total_sailings += 1;				//add one to total sailing
				if (late) {								//if late add one to late sailing
				elem.late_sailings += 1;
				}
				notexisting = false;					//flag shows it doesn't exist
				break;
			}

		}
		
		if (notexisting) {	
			DayStatistics route;						//if it doesnt exist make a struct to store new day sailing info
			route.date.day = date;
			route.date.month = month;
			route.date.year = year;
			route.total_sailings = 1;					//set sailing to one and if late make sailing to one to
			if (late) {
				route.late_sailings = 1;
			}
			unique_days.push_back(route);				//push back struct into vector of structs
		}
	}
	

	for (auto elem : unique_days) {			//calc ratio and store in a vector
		bestdaysratio = (float)elem.late_sailings / (float)elem.total_sailings;
		ratios.push_back(bestdaysratio);
	}

	float min = ratios.at(0);			//set min to first element

	for (unsigned int i = 0; i <= ratios.size() - 1; i++) {			//find min of ratios
		if (ratios.at(i) <= min) {
			min = ratios.at(i);
		}
	}

	for (unsigned int i = 0; i <= ratios.size() - 1; i++) {			//find all days with min ratio and store indexes
		if (ratios.at(i) == min) {
			index.push_back(i);
		}
	}

	for (auto elem : index) {									//add info of the days that have the lowest ratio and push back into vector and return it
	 	trip.date.day = unique_days.at(elem).date.day;
		trip.date.month = unique_days.at(elem).date.month;
		trip.date.year = unique_days.at(elem).date.year;
		trip.total_sailings = unique_days.at(elem).total_sailings;
		trip.late_sailings = unique_days.at(elem).late_sailings;
	 	bestdays.push_back(trip);
	}

	return bestdays;
}


std::vector<DayStatistics> worst_days(const std::vector<Sailing>& sailings) {
	std::vector<DayStatistics> unique_days;
	bool late = false;
	int month;
	int date;
	int year;
	bool notexisting;

	std::vector<DayStatistics> bestdays;
	float bestdaysratio;
	std::vector<float> ratios;
	std::vector<int> index;
	DayStatistics trip;

	for (unsigned int i = 0; i <= sailings.size() - 1; i++) {
		late = false;
		notexisting = true;
		month = sailings.at(i).departure_date.month;
		date = sailings.at(i).departure_date.day;
		year = sailings.at(i).departure_date.year;

		if (sailings.at(i).actual_duration >=sailings.at(i).expected_duration + 5) {
			late = true;
		}

		for (auto & elem : unique_days) {
			if (elem.date.day == date && elem.date.month == month && elem.date.year == year) {
				elem.total_sailings += 1;
				if (late) {
					elem.late_sailings += 1;
				}
				notexisting = false;
				break;
			}

		}

		if (notexisting) {
			DayStatistics route;
			route.date.day = date;
			route.date.month = month;
			route.date.year = year;
			route.total_sailings = 1;
			if (late) {
				route.late_sailings = 1;
			}
			unique_days.push_back(route);
		}
	}


	for (auto elem : unique_days) {
		bestdaysratio = (float)elem.late_sailings / (float)elem.total_sailings;
		ratios.push_back(bestdaysratio);
	}

	float max = ratios.at(0);

	for (unsigned int i = 0; i <= ratios.size() - 1; i++) {
		if (ratios.at(i) >= max) {
			max = ratios.at(i);
		}
	}

	for (unsigned int i = 0; i <= ratios.size() - 1; i++) {
		if (ratios.at(i) == max) {
			index.push_back(i);
		}
	}

	for (auto elem : index) {
		trip.date.day = unique_days.at(elem).date.day;
		trip.date.month = unique_days.at(elem).date.month;
		trip.date.year = unique_days.at(elem).date.year;
		trip.total_sailings = unique_days.at(elem).total_sailings;
		trip.late_sailings = unique_days.at(elem).late_sailings;
		bestdays.push_back(trip);
	}

	return bestdays;
}



/* Provided functions (do not modify) */
std::vector<Sailing> read_sailings(const std::string& input_filename) {
	std::vector<Sailing> all_sailings;
	std::ifstream input_file;
	input_file.open(input_filename);

	int valid_sailings{ 0 };
	int invalid_sailings{ 0 };

	if (input_file.is_open()) {

		std::string line;
		while (std::getline(input_file, line)) {
			Sailing s;
			if (parse_sailing(line, s)) {
				valid_sailings++;
				all_sailings.push_back(s);
			}
			else {
				invalid_sailings++;
			}
		}
		input_file.close();

	}
	else {
		throw std::runtime_error("Unable to open input file");
	}
	std::cout << "Read " << valid_sailings << " records." << std::endl;
	std::cout << "Skipped " << invalid_sailings << " invalid records." << std::endl;
	return all_sailings;
}

void print_sailing(const Sailing& sailing) {
	std::cout << "Route " << sailing.route_number;
	std::cout << " (" << sailing.source_terminal << " -> " << sailing.dest_terminal << "): ";
	std::cout << sailing.departure_date.year << "-";
	std::cout << std::setfill('0') << std::setw(2) << sailing.departure_date.month << "-";
	std::cout << std::setfill('0') << std::setw(2) << sailing.departure_date.day << " ";
	std::cout << std::setfill('0') << std::setw(2) << sailing.scheduled_departure_time.hour << ":";
	std::cout << std::setfill('0') << std::setw(2) << sailing.scheduled_departure_time.minute << " ";
	std::cout << "[Vessel: " << sailing.vessel_name << "] ";
	std::cout << sailing.actual_duration << " minutes (" << sailing.expected_duration << " expected)" << std::endl;
}