/*
Description: This file contains functions to help analyze data inputted on ferry routes
			 and to complete analysis related to different route statistics.
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
	std::string info;													
	std::string leftover;												
	int comma{};														
	comma = 0;                                                          
	try {																
		for (auto information : input_line) {							
			if (information != ','&&comma != 10) {                      
				info.push_back(information);                            
			}                                                           
			if (information == ',') {									
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
			if (information != ','&& comma == 10) {		
				leftover.push_back(information);                       
			}                                                          
		}                                                              
		result.actual_duration = std::stoi(leftover);                  
		if (comma != 10) {								
			throw std::runtime_error("Not enough data");               
		}                                                              
		return true;									
	}                                                                  
	catch(std::exception const& e){															
	return false;
	}
}


std::vector<RouteStatistics> performance_by_route(const std::vector<Sailing>& sailings) {
	std::vector<RouteStatistics> unique_routes;				
	bool late = false;										
	int routenumber;										
	bool notexisting;										
	for (unsigned int i = 0; i <= sailings.size()- 1; i++) {            
		routenumber = 0;									
		late = false;										
		notexisting = true;									
		routenumber=sailings.at(i).route_number;			
		if (sailings.at(i).actual_duration >= sailings.at(i).expected_duration + 5) {
			late = true;
		}
		for (auto & elem : unique_routes) {		
			if (elem.route_number == routenumber) {
				elem.total_sailings += 1;
				if (late) {
					elem.late_sailings += 1;
				}
				notexisting = false;
				break;
			}
			
		}
		if (notexisting) {
			RouteStatistics route;
			route.route_number = routenumber;
			route.total_sailings = 1;
			if (late) {
				route.late_sailings = 1;
			}
			unique_routes.push_back(route);
	}	
	return unique_routes;
}


std::vector<Sailing> fastest_sailings(const std::vector<Sailing>& sailings) {
	float speed;
	std::vector<Sailing> fastest;
	std::vector<float> speeds;
	std::vector<int> index;
	for (unsigned int i = 0; i <= sailings.size() - 1; i++) {
	speed = (float)sailings.at(i).expected_duration / (float)sailings.at(i).actual_duration;
	speeds.push_back(speed);
	}
	float max = speeds.at(0);
	for (unsigned int i = 0; i <= speeds.size() - 1; i++) {
		if (speeds.at(i)>= max) {	
			max = speeds.at(i);	
		}
	}
	for (unsigned int i = 0; i <= speeds.size() - 1; i++) {
		if (speeds.at(i) == max) {
		index.push_back(i);
		}
	}
	for (auto elem : index) {																		
		fastest.push_back(sailings.at(elem));
	}
	return fastest;
}


std::vector<Sailing> slowest_sailings(const std::vector<Sailing>& sailings) {
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
		date= sailings.at(i).departure_date.day;					
		year = sailings.at(i).departure_date.year;					
		if (sailings.at(i).actual_duration >= (sailings.at(i).expected_duration + 5)) {
			late = true;
		}
		for (auto & elem : unique_days) {
			if (elem.date.day==date && elem.date.month==month && elem.date.year==year) {
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
	float min = ratios.at(0);
	for (unsigned int i = 0; i <= ratios.size() - 1; i++) {
		if (ratios.at(i) <= min) {
			min = ratios.at(i);
		}
	}
	for (unsigned int i = 0; i <= ratios.size() - 1; i++) {
		if (ratios.at(i) == min) {
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
			} else {
				invalid_sailings++;
			}
		}
		input_file.close();

	} else {
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
