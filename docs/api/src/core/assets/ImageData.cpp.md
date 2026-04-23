# ImageData.cpp

## Brief Overview

This file contains the implementation of the `Image` class, which is used to load and manage image data. The class utilizes the STB library for image loading.

## Table of Classes/Structs/Enums

### Class: Image

- **Description**: Represents an image loaded from a file.
- **Attributes**:
  - `data`: Pointer to the image data (RGBA format).
  - `width`, `height`: Dimensions of the image.
  - `channels`: Number of color channels in the image.
  - `valid`: Indicates whether the image has been successfully loaded.

## Detailed Methods Section

### Method: `Image::load`

- **Full Signature**:
  ```cpp
  Image Image::load(const std::string& filepath)
  ```

- **Return Type Explanation**: 
  - Returns an instance of the `Image` class. The returned image is initialized with data loaded from the specified file path.

- **Internal Logic/Purpose**:
  - This method attempts to load an image from the given file path.
  - It uses the STB library (`stbi_load`) to read the image data into the member variables of the `Image` class.
  - The `data` attribute is set to point to the loaded image data, and the `width`, `height`, and `channels` attributes are updated accordingly.
  - The `valid` attribute is set to `true` if the image was successfully loaded (i.e., `data` is not `nullptr`). Otherwise, it remains `false`.

### Method: `Image::free`

- **Full Signature**:
  ```cpp
  void Image::free()
  ```

- **Return Type Explanation**: 
  - This method does not return any value (`void`).

- **Internal Logic/Purpose**:
  - This method frees the memory allocated for the image data.
  - It uses `stbi_image_free` to release the memory pointed to by `data`.
  - The `data` attribute is set to `nullptr`, and the `valid` attribute is set to `false`.

### Method: `Image::getFailureReason`

- **Full Signature**:
  ```cpp
  std::string Image::getFailureReason() const
  ```

- **Return Type Explanation**: 
  - Returns a string indicating whether the image is valid or the reason for failure.

- **Internal Logic/Purpose**:
  - This method checks if the `valid` attribute is `true`.
  - If it is, it returns "Image is valid." Otherwise, it returns the reason for the failure using `stbi_failure_reason()`.