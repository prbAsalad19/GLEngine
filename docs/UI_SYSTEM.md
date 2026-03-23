# GLEngine — UI System

This document describes the UI layout system used in GLEngine.  
UI layouts are defined in **JSON files** and loaded at runtime via `UICanvas`.  
Alternatively, layouts can be created visually using the **GLEngine UI Editor** (in development).

---

## Table of Contents

- [Overview](#overview)
- [File Structure](#file-structure)
- [Elements](#elements)
  - [panel](#panel)
  - [label](#label)
  - [progressbar](#progressbar)
  - [icon](#icon)
- [Geometry](#geometry)
  - [Simple Mode](#simple-mode)
  - [Free Mode](#free-mode)
- [Colors](#colors)
- [Dynamic Bindings](#dynamic-bindings)
- [Templates](#templates)

---

## Overview

A UI layout is a `.json` file containing an array of elements.  
Each element has a type, a color, a geometry, and optionally a text value or a dynamic binding key.

Loading a layout:

```cpp
UICanvas canvas;
canvas.loadUI("assets/ui/hud.json");
```

Rendering every frame:

```cpp
UIrenderer.render(canvas);
```

---

## File Structure

```json
{
  "elements": [
    {
      "id":       "unique_id",
      "type":     "panel | label | progressbar | icon",
      "color":    "#RRGGBBAA",
      "geometry": { ... },

      // optional
      "text":     "static text",
      "valueKey": "binding_key",
      "fontSize": 24.0
    }
  ]
}
```

| Field      | Type     | Required | Description                                              |
|------------|----------|----------|----------------------------------------------------------|
| `id`       | string   | ✅        | Unique identifier used for bindings and lookups          |
| `type`     | string   | ✅        | Element type — see [Elements](#elements)                 |
| `color`    | string   | ✅        | RGBA hex color — see [Colors](#colors)                   |
| `geometry` | object   | ✅        | Position and size — see [Geometry](#geometry)            |
| `text`     | string   | ❌        | Static text (label only)                                 |
| `valueKey` | string   | ❌        | Key for dynamic binding — see [Dynamic Bindings](#dynamic-bindings) |
| `fontSize` | float    | ❌        | Font size override in pixels (0 = use atlas default)     |

---

## Elements

### `panel`

A solid colored rectangle. Used for backgrounds, cards, and containers.

```json
{
  "id":    "main_panel",
  "type":  "panel",
  "color": "#1A1A2ECC",
  "geometry": {
    "position": [20, 20],
    "size":     [300, 120]
  }
}
```

---

### `label`

A text element. Supports both static text and dynamic bindings.

```json
{
  "id":    "title",
  "type":  "label",
  "color": "#FFFFFFFF",
  "text":  "GLEngine",
  "geometry": {
    "position": [30, 30],
    "size":     [280, 40]
  }
}
```

For dynamic text, use `valueKey` and register a binding in code:

```json
{
  "id":       "fps_counter",
  "type":     "label",
  "color":    "#00FF88FF",
  "valueKey": "fps",
  "geometry": {
    "position": [30, 70],
    "size":     [280, 28]
  }
}
```

```cpp
canvas.bindString("fps", [&]() {
    return "FPS: " + std::to_string(currentFps);
});
```

---

### `progressbar`

A rectangle intended to represent a progress value. The fill logic is driven by a float binding in `[0, 1]`.

```json
{
  "id":       "health_bar",
  "type":     "progressbar",
  "color":    "#FF4444FF",
  "valueKey": "health",
  "geometry": {
    "position": [20, 150],
    "size":     [200, 20]
  }
}
```

```cpp
canvas.bindFloat("health", &playerHealth);  // float in [0, 1]
```

---

### `icon`

A textured quad. Intended for image-based UI elements such as icons and sprites.

```json
{
  "id":    "crosshair",
  "type":  "icon",
  "color": "#FFFFFFFF",
  "geometry": {
    "position": [620, 340],
    "size":     [40, 40]
  }
}
```

---

## Geometry

Every element requires a `geometry` block. Two modes are supported.

### Simple Mode

Defined by a **position** (top-left corner in pixels) and a **size**.  
An optional **rotation** field rotates the element around its center, in degrees.

```json
"geometry": {
  "position": [50, 50],
  "size":     [300, 80],
  "rotation": 0.0
}
```

Two-point shorthand — define top-left (`first`) and bottom-right (`last`) corners:

```json
"geometry": {
  "first": [50, 50],
  "last":  [350, 130]
}
```

| Field      | Type          | Description                                  |
|------------|---------------|----------------------------------------------|
| `position` | [float, float]| Top-left corner in screen pixels             |
| `size`     | [float, float]| Width and height in pixels                   |
| `rotation` | float         | Rotation in degrees, default 0               |
| `first`    | [float, float]| Top-left corner (alternative to position)    |
| `last`     | [float, float]| Bottom-right corner (alternative to size)    |

---

### Free Mode

Defines the quad with **4 explicit corners** in screen pixels.  
Useful for trapezoids, perspective quads, or any non-rectangular shape.

```json
"geometry": {
  "points": [
    [50,  50],
    [350, 50],
    [330, 130],
    [70,  130]
  ]
}
```

Points are ordered: **top-left, top-right, bottom-right, bottom-left**.

---

## Colors

Colors are expressed as **8-digit hex strings** in `#RRGGBBAA` format.

```
#FFFFFFFF   → white,  fully opaque
#00000080   → black,  50% transparent
#FF4444FF   → red,    fully opaque
#1A1A2ECC   → dark blue, ~80% opaque
#00FF88FF   → green,  fully opaque
```

| Component | Position | Range  |
|-----------|----------|--------|
| R (red)   | 1–2      | 00–FF  |
| G (green) | 3–4      | 00–FF  |
| B (blue)  | 5–6      | 00–FF  |
| A (alpha) | 7–8      | 00–FF  |

Alpha `FF` = fully opaque, `00` = fully transparent.

---

## Dynamic Bindings

Labels and progress bars can display values that update every frame.  
Bindings connect a `valueKey` in the JSON to a C++ lambda or pointer.

### String bindings — for labels

```cpp
// Lambda
canvas.bindString("fps", [&]() {
    return "FPS: " + std::to_string(currentFps);
});

// Pointer to std::string
canvas.bindString("player_name", &playerName);
```

### Float bindings — for progress bars

```cpp
// Lambda
canvas.bindFloat("health", [&]() {
    return player.getHealth() / player.getMaxHealth();
});

// Pointer to float
canvas.bindFloat("stamina", &playerStamina);
```

Bindings are resolved automatically every frame during `render()`.  
If a `label` has both `text` and `valueKey`, the binding takes priority when active.

---

## Templates

### Minimal HUD

```json
{
  "elements": [
    {
      "id":    "hud_bg",
      "type":  "panel",
      "color": "#00000080",
      "geometry": {
        "position": [10, 10],
        "size":     [200, 60]
      }
    },
    {
      "id":       "fps",
      "type":     "label",
      "color":    "#00FF88FF",
      "valueKey": "fps",
      "geometry": {
        "position": [20, 20],
        "size":     [180, 28]
      }
    }
  ]
}
```

---

### Panel with title and subtitle

```json
{
  "elements": [
    {
      "id":    "card",
      "type":  "panel",
      "color": "#1A1A2EFF",
      "geometry": {
        "position": [50, 50],
        "size":     [400, 120]
      }
    },
    {
      "id":    "card_title",
      "type":  "label",
      "color": "#FFFFFFFF",
      "text":  "GLEngine",
      "geometry": {
        "position": [66, 65],
        "size":     [380, 40]
      }
    },
    {
      "id":    "card_subtitle",
      "type":  "label",
      "color": "#AAAAААFF",
      "text":  "OpenGL 4.6 Renderer",
      "geometry": {
        "position": [66, 108],
        "size":     [380, 28]
      }
    }
  ]
}
```

---

### Health and stamina bars

```json
{
  "elements": [
    {
      "id":    "bars_bg",
      "type":  "panel",
      "color": "#000000AA",
      "geometry": {
        "position": [20, 620],
        "size":     [260, 70]
      }
    },
    {
      "id":    "health_label",
      "type":  "label",
      "color": "#FFFFFFFF",
      "text":  "HP",
      "geometry": {
        "position": [30, 628],
        "size":     [40, 24]
      }
    },
    {
      "id":       "health_bar",
      "type":     "progressbar",
      "color":    "#FF4444FF",
      "valueKey": "health",
      "geometry": {
        "position": [75, 630],
        "size":     [190, 20]
      }
    },
    {
      "id":    "stamina_label",
      "type":  "label",
      "color": "#FFFFFFFF",
      "text":  "SP",
      "geometry": {
        "position": [30, 658],
        "size":     [40, 24]
      }
    },
    {
      "id":       "stamina_bar",
      "type":     "progressbar",
      "color":    "#44AAFFFF",
      "valueKey": "stamina",
      "geometry": {
        "position": [75, 660],
        "size":     [190, 20]
      }
    }
  ]
}
```

---

### Skewed element — free mode

```json
{
  "elements": [
    {
      "id":    "skewed_panel",
      "type":  "panel",
      "color": "#FF444488",
      "geometry": {
        "points": [
          [100, 100],
          [400, 100],
          [370, 160],
          [130, 160]
        ]
      }
    }
  ]
}
```

---

> Layouts can also be created and exported visually using the **GLEngine UI Editor**.  
> The editor produces JSON files fully compatible with this format.
