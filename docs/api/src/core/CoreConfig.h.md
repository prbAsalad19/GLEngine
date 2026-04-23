 # CoreConfig.h

## Brief Overview

`CoreConfig.h` is a header file that provides core configuration functionalities for the application. It includes essential libraries and defines various data structures and functions necessary for managing configuration settings.

## Table of Classes/Structs/Enums

### Classes/Structs/Enums

1. **ConfigManager**
   - Manages and loads configuration files.
2. **Configuration**
   - Represents a single configuration setting.
3. **ConfigParser**
   - Parses configuration data from files.

## Detailed Methods Section

### ConfigManager

#### `loadConfiguration(const std::string& filePath)`

- **Full Signature:**
  ```cpp
  bool loadConfiguration(const std::string& filePath);
  ```

- **Return Type Explanation:**
  - Returns `true` if the configuration file was successfully loaded, otherwise `false`.

- **Internal Logic/Purpose:**
  - Opens and reads the specified configuration file.
  - Parses the contents using `ConfigParser`.
  - Loads the parsed settings into a data structure for easy access.

### Configuration

#### `getValue(const std::string& key)`

- **Full Signature:**
  ```cpp
  T getValue(const std::string& key);
  ```

- **Return Type Explanation:**
  - Returns the value associated with the specified `key`. The type `T` is determined by the context in which this method is used.

- **Internal Logic/Purpose:**
  - Retrieves the configuration value for a given key from an internal data structure.
  - Handles type conversions and returns the appropriate value.

### ConfigParser

#### `parse(const std::string& input)`

- **Full Signature:**
  ```cpp
  void parse(const std::string& input);
  ```

- **Return Type Explanation:**
  - This method does not return any value explicitly. It populates an internal data structure with parsed configuration settings.

- **Internal Logic/Purpose:**
  - Takes a string containing configuration data.
  - Splits the string into key-value pairs based on delimiters.
  - Stores each key-value pair in an internal map for later retrieval.

---

This header file is crucial for managing application configurations, ensuring that all necessary settings are loaded and accessible throughout the application.