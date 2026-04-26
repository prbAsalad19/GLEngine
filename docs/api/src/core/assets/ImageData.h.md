## ImageData.h

### Brief Overview

This header file provides a class `Image` for handling image data. The class includes methods to load and manage image files, ensuring memory management is handled correctly.

### Table of Classes/Structs/Enums

1. **struct Image**
   - Represents an image with its data, dimensions, and validity status.
   - Includes static method for loading images and member functions for freeing memory and retrieving failure reasons.

### Detailed Methods Section

#### Method: `Image::load`

- **Full Signature**:
  ```cpp
  static Image load(const std::string& filepath);
  ```
  
- **Return Type Explanation**:
  - **Type**: `Image`
  - **Explanation**: Returns an instance of the `Image` class that contains the loaded image data.

- **Internal Logic/Purpose**:
  - This method uses a library (presumably STB) to load an image from the specified filepath.
  - It always converts the image to have four channels (RGBA).
  - If the image is successfully loaded, it sets `valid` to true and fills in the `data`, `width`, `height`, and `channels` fields of the returned `Image` instance.

#### Method: `Image::free`

- **Full Signature**:
  ```cpp
  void free();
  ```
  
- **Return Type Explanation**:
  - **Type**: `void`
  - **Explanation**: Does not return any value.
  
- **Internal Logic/Purpose**:
  - This method is responsible for freeing the memory allocated by the STB library for storing image data.
  - It sets the `data` pointer to null and resets the width, height, and channels fields to zero.
  - If the image data was valid, it also sets the `valid` field to false.

#### Method: `Image::getFailureReason`

- **Full Signature**:
  ```cpp
  std::string getFailureReason() const;
  ```
  
- **Return Type Explanation**:
  - **Type**: `std::string`
  - **Explanation**: Returns a string that describes the reason for the failure to load an image.
  
- **Internal Logic/Purpose**:
  - This method returns the reason why the last attempt to load an image failed.
  - If no failure occurred (i.e., the image is valid), it returns an empty string.