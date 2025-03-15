#include "json_facade.h"

// Standard library imports
#include <fstream>
#include <stdexcept>


Json::Json() 
    : impl(std::make_unique<nlohmann::json>()) 
{}

Json::Json(const Json& other) 
    : impl(std::make_unique<nlohmann::json>(*other.impl)) 
{}

Json::~Json() = default;

Json::Json(const nlohmann::json& json) 
    : impl(std::make_unique<nlohmann::json>(json)) 
{}

bool Json::contains(const std::string& key) const 
{
    return impl->contains(key);
}

Json Json::operator[](const std::string& key) const 
{
    return Json((*impl)[key]);
}

int Json::as_int() const 
{
    return impl->get<int>();
}

float Json::as_float() const 
{
    return impl->get<float>();
}

std::string Json::as_string() const 
{
    return impl->get<std::string>();
}

bool Json::as_bool() const 
{
    return impl->get<bool>();
}

Json Json::make_object() 
{
    Json result;
    *result.impl = nlohmann::json::object();
    return result;
}

void Json::set(const std::string& key, int value) 
{
    (*impl)[key] = value;
}

void Json::set(const std::string& key, float value) 
{
    (*impl)[key] = value;
}

void Json::set(const std::string& key, const std::string& value) 
{
    (*impl)[key] = value;
}

void Json::set(const std::string& key, bool value) 
{
    (*impl)[key] = value;
}

void Json::set(const std::string& key, const Json& value)
{
    (*impl)[key] = *value.impl;
}

Json Json::make_array() 
{
    Json result;
    *result.impl = nlohmann::json::array();
    return result;
}

size_t Json::size() const 
{
    return impl->size();
}

Json Json::at(size_t index) const 
{
    return Json(impl->at(index));
}

void Json::push_back(const Json& value) 
{
    impl->push_back(*value.impl);
}

void Json::push_back(int value) 
{
    impl->push_back(value);
}

void Json::push_back(unsigned int value)
{
    impl->push_back(value);
}

void Json::push_back(float value) 
{
    impl->push_back(value);
}

void Json::push_back(const std::string& value) 
{
    impl->push_back(value);
}

// JsonFacade implementation
Json JsonFacade::load_from_file(const std::string& filepath) 
{
    std::ifstream file(filepath);
    if (!file.is_open()) 
    {
        throw std::runtime_error("Could not open file: " + filepath);
    }
    
    try 
    {
        const nlohmann::json json_data = nlohmann::json::parse(file);
        return Json(json_data);
    } 
    catch (const std::exception& e) 
    {
        throw std::runtime_error("Failed to parse JSON file: " + std::string(e.what()));
    }
}

void JsonFacade::save_to_file(const std::string& filepath, const Json& json, bool pretty_print/*= true*/) 
{
    std::ofstream file(filepath);
    if (!file.is_open()) 
    {
        throw std::runtime_error("Could not open file for writing: " + filepath);
    }
    
    try 
    {
        if (pretty_print) 
        {
            file << json.impl->dump(4); // 4-space indentation for pretty printing
        } 
        else 
        {
            file << json.impl->dump();
        }
    } 
    catch (const std::exception& e) 
    {
        throw std::runtime_error("Failed to write JSON file: " + std::string(e.what()));
    }
}

std::string JsonFacade::serialize(const Json& json, bool pretty_print/*= false*/) 
{
    if (pretty_print) 
    {
        return json.impl->dump(4);
    } 
    else 
    {
        return json.impl->dump();
    }
}

Json JsonFacade::parse(const std::string& json_string) 
{
    try 
    {
        const nlohmann::json parsed = nlohmann::json::parse(json_string);
        return Json(parsed);
    } 
    catch (const std::exception& e) 
    {
        throw std::runtime_error("Failed to parse JSON string: " + std::string(e.what()));
    }
}
