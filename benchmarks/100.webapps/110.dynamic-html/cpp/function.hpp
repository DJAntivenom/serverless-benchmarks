#ifndef FUNCTION_HPP
#define FUNCTION_HPP

#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <chrono>
#include <sstream>
#include <iomanip>

// Requires nlohmann/json and inja (Jinja2 for C++)
#include <nlohmann/json.hpp>
#include <inja/inja.hpp>

using json = nlohmann::json;

std::string render_benchmark(const std::string& username, int random_len) {
    // Generate Random Numbers
    std::vector<int> random_numbers;
    random_numbers.reserve(random_len);
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, 1000000);

    for (int i = 0; i < random_len; ++i) {
        random_numbers.push_back(distrib(gen));
    }

    // Get Current Time
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    auto micros = std::chrono::duration_cast<std::chrono::microseconds>(
        now.time_since_epoch()
    ) % 1000000;
    
    std::stringstream ss;
    ss << std::put_time(std::localtime(&now_c), "%Y-%m-%d %H:%M:%S"); 
    ss << '.' << std::setfill('0') << std::setw(6) << micros.count();
    
    // Prepare Data for Template
    json data;
    data["username"] = username;
    data["cur_time"] = ss.str();
    data["random_numbers"] = random_numbers;

    // Render Template
    inja::Environment env;
    
    // We assume 'templates/template.html' is relative to the execution working directory
    try {
        return env.render_file("templates/template.html", data);
    } catch (const std::exception& e) {
        std::cerr << "Template rendering error: " << e.what() << std::endl;
        return "Error rendering template";
    }
}

#endif