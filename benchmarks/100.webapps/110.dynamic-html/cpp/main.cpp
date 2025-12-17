#include <aws/core/Aws.h>
#include <aws/core/utils/json/JsonSerializer.h>
#include <iostream>

#include "function.hpp"

// Standard SeBS AWS C++ SDK handler signature
Aws::Utils::Json::JsonValue function(Aws::Utils::Json::JsonView request)
{
    // arse Input
    Aws::String username = "default_user";
    if (request.ValueExists("username") && request.GetObject("username").IsString()) {
        username = request.GetString("username");
    }
    
    int random_len = 0;
    if (request.ValueExists("random_len")) {
        if (request.GetObject("random_len").IsIntegerType()) {
            random_len = request.GetInteger("random_len");
        } else if (request.GetObject("random_len").IsString()) {
             try {
                random_len = std::stoi(request.GetString("random_len").c_str());
             } catch(...) {
                random_len = 0;
             }
        }
    }

    // Execute Logic
    std::string html_result = render_benchmark(username.c_str(), random_len);

    // Form Response
    Aws::Utils::Json::JsonValue val;
    if (html_result.empty()) {
        val.WithString("error", "Failed to render template");
    } else {
        val.WithString("result", html_result);
    }
    
    return val;
}