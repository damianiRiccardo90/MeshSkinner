#pragma once

// Standard library imports
#include <string>
#include <vector>

// Third-party imports
#include "nlohmann/json.h"


/**
 * @brief A lightweight wrapper around JSON data that exposes only what we need.
 *
 * This class provides a simplified interface for working with JSON data,
 * abstracting away the complexity of the underlying nlohmann::json library.
 * It supports common operations for JSON arrays, objects, and primitive values.
 */
class Json 
{
public:

    /**
     * @brief Default constructor creates an empty JSON object.
     */
    Json();

    /**
     * @brief Copy constructor (deep copies underlying JSON).
     * @param other The Json object to copy.
     */
    Json(const Json& other);

    /**
     * @brief Destructor.
     */
    ~Json();
    
    // -------------------------------------------------------------------------
    // Object operations
    // -------------------------------------------------------------------------
    
    /**
     * @brief Checks if the JSON object contains the specified key.
     * @param key The key to check for.
     * @return true if the key exists; otherwise false.
     */
    bool contains(const std::string& key) const;

    /**
     * @brief Accesses the value associated with the specified key.
     * @param key The key of the element to access.
     * @return The Json value associated with the key.
     * @note Unlike at(), this operator creates a new element if the key doesn't exist.
     */
    Json operator[](const std::string& key) const;
    
    // -------------------------------------------------------------------------
    // Value extraction
    // -------------------------------------------------------------------------
    
    /**
     * @brief Converts the JSON value to an integer.
     * @return The integer value.
     * @throws nlohmann::json::type_error if the value cannot be converted to an integer.
     */
    int as_int() const;

    /**
     * @brief Converts the JSON value to a float.
     * @return The float value.
     * @throws nlohmann::json::type_error if the value cannot be converted to a float.
     */
    float as_float() const;

    /**
     * @brief Converts the JSON value to a string.
     * @return The string value.
     * @throws nlohmann::json::type_error if the value cannot be converted to a string.
     */
    std::string as_string() const;

    /**
     * @brief Converts the JSON value to a boolean.
     * @return The boolean value.
     * @throws nlohmann::json::type_error if the value cannot be converted to a boolean.
     */
    bool as_bool() const;
    
    // -------------------------------------------------------------------------
    // Construction helpers
    // -------------------------------------------------------------------------
    
    /**
     * @brief Creates a new JSON object.
     * @return A new Json object instance.
     */
    static Json make_object();
    
    /**
     * @brief Sets an integer value for the specified key in a JSON object.
     * @param key The key for the value.
     * @param value The integer value to set.
     * @throws nlohmann::json::type_error if the current value is not an object.
     */
    void set(const std::string& key, int value);

    /**
     * @brief Sets a float value for the specified key in a JSON object.
     * @param key The key for the value.
     * @param value The float value to set.
     * @throws nlohmann::json::type_error if the current value is not an object.
     */
    void set(const std::string& key, float value);

    /**
     * @brief Sets a string value for the specified key in a JSON object.
     * @param key The key for the value.
     * @param value The string value to set.
     * @throws nlohmann::json::type_error if the current value is not an object.
     */
    void set(const std::string& key, const std::string& value);

    /**
     * @brief Sets a boolean value for the specified key in a JSON object.
     * @param key The key for the value.
     * @param value The boolean value to set.
     * @throws nlohmann::json::type_error if the current value is not an object.
     */
    void set(const std::string& key, bool value);

    /**
     * @brief Sets a Json value for the specified key in a JSON object.
     * @param key The key for the value.
     * @param value The Json value to set.
     * @throws nlohmann::json::type_error if the current value is not an object.
     */
    void set(const std::string& key, const Json& value);

    // -------------------------------------------------------------------------
    // Array operations
    // -------------------------------------------------------------------------

    /**
     * @brief Creates a new JSON array.
     * @return A new Json array instance.
     */
    static Json make_array();

    /**
     * @brief Gets the size of a JSON array.
     * @return The number of elements in the array.
     * @throws nlohmann::json::type_error if the value is not an array.
     */
    size_t size() const;

    /**
     * @brief Accesses the element at the specified index in a JSON array.
     * @param index The index of the element to access.
     * @return The Json value at the specified index.
     * @throws nlohmann::json::out_of_range if the index is out of range.
     */
    Json at(size_t index) const;

    /**
     * @brief Adds a value to the end of a JSON array.
     * @param value The Json value to add.
     * @throws nlohmann::json::type_error if the value is not an array.
     */
    void push_back(const Json& value);

    /**
     * @brief Adds an integer to the end of a JSON array.
     * @param value The integer value to add.
     * @throws nlohmann::json::type_error if the value is not an array.
     */
    void push_back(int value);

    /**
     * @brief Adds an unsigned integer to the end of a JSON array.
     * @param value The unsigned integer value to add.
     * @throws nlohmann::json::type_error if the value is not an array.
     */
    void push_back(unsigned int value);

    /**
     * @brief Adds a float to the end of a JSON array.
     * @param value The float value to add.
     * @throws nlohmann::json::type_error if the value is not an array.
     */
    void push_back(float value);

    /**
     * @brief Adds a string to the end of a JSON array.
     * @param value The string value to add.
     * @throws nlohmann::json::type_error if the value is not an array.
     */
    void push_back(const std::string& value);

private:

    // The actual implementation, hidden from clients
    std::unique_ptr<nlohmann::json> impl;
    
    /**
     * @brief Constructor from the alias type (for internal use).
     * @param json The underlying nlohmann::json instance.
     */
    explicit Json(const nlohmann::json& json);
    
    // Give JsonFacade direct access to impl if needed
    friend class JsonFacade;
};

/**
 * @brief Facade for JSON parsing and serialization operations.
 *
 * This class provides a simplified interface for loading, saving,
 * and manipulating JSON data, abstracting away the complexity of
 * the underlying JSON library.
 */
class JsonFacade 
{
public:

    /**
     * @brief Loads JSON data from a file.
     * @param filepath The path to the JSON file.
     * @return A Json object containing the parsed data.
     * @throws std::runtime_error if the file cannot be loaded or parsed.
     */
    static Json load_from_file(const std::string& filepath);
    
    /**
     * @brief Saves JSON data to a file.
     * @param filepath The path where the JSON file will be written.
     * @param json The Json object to serialize.
     * @param pretty_print Whether to format the output with indentation and line breaks.
     * @throws std::runtime_error if the file cannot be written.
     */
    static void save_to_file(const std::string& filepath, const Json& json, bool pretty_print = true);
    
    /**
     * @brief Converts a Json object to a string.
     * @param json The Json object to serialize.
     * @param pretty_print Whether to format the output with indentation and line breaks.
     * @return A string containing the serialized JSON.
     */
    static std::string serialize(const Json& json, bool pretty_print = false);
    
    /**
     * @brief Parses a JSON string into a Json object.
     * @param json_string The string containing JSON data.
     * @return A Json object containing the parsed data.
     * @throws std::runtime_error if the string cannot be parsed as valid JSON.
     */
    static Json parse(const std::string& json_string);
};
