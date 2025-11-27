# Configuration Parsing - Reading Textures and Colors

## Table of Contents
1. [Overview](#overview)
2. [The Parsing Pipeline](#the-parsing-pipeline)
3. [File Opening and Validation](#file-opening-and-validation)
4. [Line-by-Line Processing](#line-by-line-processing)
5. [Texture Parsing Deep Dive](#texture-parsing-deep-dive)
6. [Color Parsing Deep Dive](#color-parsing-deep-dive)
7. [The Flag System](#the-flag-system)
8. [Complete Example Walkthrough](#complete-example-walkthrough)

---

## Overview

**Configuration parsing** is the first phase of the parsing system. Its job is to extract the 6 required configuration elements from the `.cub` file:

- **4 Textures:** NO, SO, WE, EA
- **2 Colors:** F (floor), C (ceiling)

### Main Function: `parse_config_file()`

Located in: **`src/parsing/parsing.c`**

This is the **entry point** called from `main()`:

```c
// In main.c:
parse_config_file(&(game.config), argv[1]);
```

---

## The Parsing Pipeline

```mermaid
graph TB
    A[parse_config_file] --> B[Check .cub extension]
    B --> C[Open file]
    C --> D[Initialize config structure]
    D --> E[read_config_lines]
    E --> F{Line contains<br/>map data?}
    F --> |Yes| G[Stop config parsing]
    F --> |No| H[parse_config_line]
    H --> I{Recognize<br/>identifier?}
    I --> |Texture| J[parse_all_textures]
    I --> |Color| K[parse_all_colors]
    I --> |Empty| E
    I --> |Unknown| ERROR1[Error: Invalid identifier]
    J --> L{Parse<br/>successful?}
    K --> M{Parse<br/>successful?}
    L --> |Yes| N[Update flags]
    M --> |Yes| N
    L --> |No| ERROR2[Error: Invalid texture]
    M --> |No| ERROR3[Error: Invalid color]
    N --> O{All flags set?}
    O --> |No| E
    O --> |Yes| G
    G --> P{All 6 elements<br/>parsed?}
    P --> |No| ERROR4[Error: Missing elements]
    P --> |Yes| Q[Close file, proceed to map parsing]
    
    style ERROR1 fill:#ffcccc
    style ERROR2 fill:#ffcccc
    style ERROR3 fill:#ffcccc
    style ERROR4 fill:#ffcccc
    style Q fill:#ccffcc
```

---

## File Opening and Validation

### Step 1: Check File Extension

**File:** `src/parsing/parsing.c`  
**Function:** `parse_config_file()`

```c
int parse_config_file(t_config *config, char *path)
{
    int     fd;
    int     result;
    size_t  len;

    ft_memset(config, 0, sizeof(t_config));
    len = ft_strlen(path);
    if (len < 4 || ft_strcmp(path + len - 4, ".cub") != 0)
        ft_exit(2, "Map file must have .cub extension");
    // ...
}
```

**Explanation:**

```mermaid
graph LR
    A["path = 'maps/level1.cub'"] --> B[ft_strlen]
    B --> C["len = 15"]
    C --> D["path + len - 4"]
    D --> E["Points to '.cub'"]
    E --> F[ft_strcmp with '.cub']
    F --> G{Match?}
    G --> |Yes| H[Continue]
    G --> |No| I[ft_exit: Invalid extension]
    
    style H fill:#ccffcc
    style I fill:#ffcccc
```

**How `path + len - 4` works:**

```
String: "maps/level1.cub"
Index:   0123456789...14
         m a p s / l e v e l 1 . c u b
                                 ^
                                 |
                      path + 15 - 4 = path + 11
                      Points to ".cub"
```

**Why `len < 4`?**
If the filename is shorter than 4 characters (e.g., `"a.c"`), it can't possibly end with `.cub`, and accessing `path + len - 4` would be unsafe.

---

### Step 2: Open the File

```c
fd = open(path, O_RDONLY);
if (fd < 0)
    ft_exit(3, "Failed to open map file");
```

**What is `fd`?**
`fd` (file descriptor) is an integer that represents the opened file. Think of it as a "handle" or "ID" for the file:

- **fd >= 0:** Success! File is open
- **fd < 0:** Error! File doesn't exist or can't be read

**How `open()` works:**

```mermaid
sequenceDiagram
    participant Code
    participant OS as Operating System
    participant Disk
    
    Code->>OS: open("maps/level1.cub", O_RDONLY)
    OS->>Disk: Check if file exists
    Disk-->>OS: File found
    OS->>OS: Check read permissions
    OS-->>Code: Return fd = 3
    
    Note over Code: fd = 3 means success!<br/>Now we can read from file
```

---

### Step 3: Initialize Config Structure

```c
ft_memset(config, 0, sizeof(t_config));
```

**What does this do?**

`ft_memset` sets all bytes in the `config` structure to `0` (zero). This is like resetting everything to a clean state:

```c
// Before memset (random garbage):
config->config_flags = ???
config->textures.north = ???
config->floor.r = ???
// ... etc

// After memset (all zeros):
config->config_flags = 0
config->textures.north = NULL (0 as pointer)
config->floor.r = 0
// ... etc
```

**Why is this important?**
- Pointers set to `NULL` (not pointing to random memory)
- `config_flags` starts at `0` (no elements parsed yet)
- Color values start at `0`
- Ensures predictable starting state

---

## Line-by-Line Processing

### Main Reading Loop: `read_config_lines()`

**File:** `src/parsing/parsing.c`

```c
static int read_config_lines(t_config *config, int fd)
{
    char    *line;
    int     result;

    line = get_next_line(fd);
    while (line != NULL)
    {
        if (is_map_line(line))
        {
            free(line);
            break;
        }
        result = parse_config_line(config, line);
        free(line);
        if (result < 0)
            return (-2);
        line = get_next_line(fd);
    }
    return (0);
}
```

**Step-by-step flow:**

```mermaid
sequenceDiagram
    participant Loop as read_config_lines
    participant GNL as get_next_line
    participant Check as is_map_line
    participant Parse as parse_config_line
    
    Loop->>GNL: Read first line
    GNL-->>Loop: "NO ./textures/north.xpm\n"
    Loop->>Check: Is this a map line?
    Check-->>Loop: No
    Loop->>Parse: Parse this line
    Parse-->>Loop: Success (1)
    Loop->>Loop: free(line)
    
    Loop->>GNL: Read next line
    GNL-->>Loop: "SO ./textures/south.xpm\n"
    Loop->>Check: Is this a map line?
    Check-->>Loop: No
    Loop->>Parse: Parse this line
    Parse-->>Loop: Success (1)
    Loop->>Loop: free(line)
    
    Loop->>GNL: Read next line
    GNL-->>Loop: "111111\n"
    Loop->>Check: Is this a map line?
    Check-->>Loop: Yes! (Map started)
    Loop->>Loop: free(line)
    Loop->>Loop: break (stop reading config)
```

**Understanding `get_next_line()`:**

This function reads **one line at a time** from the file:

```
File content:          get_next_line() returns:
┌────────────────┐     ┌────────────────────────┐
│ NO ./tex/n.xpm │ →   │ "NO ./tex/n.xpm\n"    │ (1st call)
│ SO ./tex/s.xpm │ →   │ "SO ./tex/s.xpm\n"    │ (2nd call)
│ F 220,100,0    │ →   │ "F 220,100,0\n"       │ (3rd call)
│ 111111         │ →   │ "111111\n"            │ (4th call)
└────────────────┘     └────────────────────────┘
```

**Why `free(line)` after each iteration?**

`get_next_line()` allocates (mallocs) memory for each line. We must free it to avoid memory leaks:

```c
line = get_next_line(fd);  // Allocates memory
// ... use line ...
free(line);  // Release memory
```

However, in this project we use **garbage collector**, so `get_next_line` likely uses `gc_malloc`, which means `gc_clean()` will free everything at the end. The explicit `free()` is still good practice!

---

### Determining When Config Ends: `is_map_line()`

**File:** `src/parsing/map_utils.c`

```c
int is_map_line(const char *line)
{
    int i;
    int has_content;

    if (!line || !*line)
        return (0);
    i = 0;
    has_content = 0;
    while (line[i])
    {
        if (!is_map_char(line[i]) && line[i] != '\n')
            return (0);
        if (line[i] != NOT_WALKABLE && line[i] != '\n')
            has_content = 1;
        i++;
    }
    return (has_content);
}
```

**What makes a line a "map line"?**

```mermaid
graph TD
    A[Check line] --> B{Line exists?}
    B --> |No| NO1[Not a map line]
    B --> |Yes| C[Check each character]
    C --> D{Valid map char?}
    D --> |No| NO2[Not a map line]
    D --> |Yes| E{Is it space or newline?}
    E --> |No| F[has_content = 1]
    E --> |Yes| C
    F --> C
    C --> G[End of line]
    G --> H{has_content == 1?}
    H --> |Yes| YES[IS a map line!]
    H --> |No| NO3[Not a map line]
    
    style YES fill:#ccffcc
    style NO1 fill:#ffcccc
    style NO2 fill:#ffcccc
    style NO3 fill:#ffcccc
```

**Examples:**

| Line | Is Map Line? | Why |
|------|--------------|-----|
| `"111111\n"` | ✅ Yes | All characters are valid map chars (1), has content |
| `"10N001\n"` | ✅ Yes | Valid map chars, has content |
| `"NO ./tex/n.xpm\n"` | ❌ No | Contains 'N', 'O' separately (not together as player), spaces, and '.' which aren't valid in that context |
| `"   \n"` | ❌ No | Only spaces, no content |
| `"F 220,100,0\n"` | ❌ No | Contains invalid characters (comma, letters not as player) |

**Helper function:** `is_map_char()`

```c
int is_map_char(char c)
{
    return (c == WALKABLE || c == WALL || c == 'N' || c == 'S'
        || c == 'E' || c == 'W' || c == NOT_WALKABLE);
}

// Expands to:
// return (c == '0' || c == '1' || c == 'N' || c == 'S'
//     || c == 'E' || c == 'W' || c == ' ');
```

---

## Texture Parsing Deep Dive

### The Dispatcher: `parse_config_line()`

**File:** `src/parsing/parsing_config.c`

```c
int parse_config_line(t_config *config, const char *line)
{
    int result;

    result = parse_all_textures(config, line);
    if (result != 0)
        return (result);
    result = parse_all_colors(config, line);
    if (result != 0)
        return (result);
    if (has_non_empty_content(line))
        return (-1);
    return (0);
}
```

**How dispatching works:**

```mermaid
graph TD
    A["Line: 'NO ./tex/n.xpm'"] --> B[parse_config_line]
    B --> C[parse_all_textures]
    C --> D[parse_texture_no]
    D --> E{Match 'NO'?}
    E --> |Yes| F[set_texture]
    E --> |No| G[Try parse_texture_so]
    G --> H{Match 'SO'?}
    H --> |No| I[Try parse_texture_we]
    I --> J{Match 'WE'?}
    J --> |No| K[Try parse_texture_ea]
    K --> L{Match 'EA'?}
    L --> |No| M[Return 0: not a texture]
    F --> |Success| N[Return 1: parsed!]
    F --> |Error| O[Return -1: error!]
    
    M --> P[parse_all_colors]
    
    style N fill:#ccffcc
    style O fill:#ffcccc
```

**Return value meanings:**

| Return Value | Meaning | Action |
|--------------|---------|--------|
| `1` | Successfully parsed | Continue to next line |
| `0` | Not this identifier, try next | Try next parser |
| `-1` | Error (duplicate or invalid) | Exit program |

---

### Texture Parser: `parse_texture_no()`

**File:** `src/parsing/utils_parsing.c`

```c
int parse_texture_no(t_config *config, const char *line)
{
    char path[256];

    if (ft_sscanf(line, "NO %s", path) == 1)
        return set_texture(config, &config->textures.north, path, FLAG_NO);
    return (0);
}
```

**Understanding `ft_sscanf()`:**

`ft_sscanf` is like scanf, but reads from a string instead of stdin:

```c
// Format: "NO %s"
//          ^  ^
//          |  |
//        Match exactly "NO"
//             Match any string (no spaces)

ft_sscanf("NO ./textures/north.xpm", "NO %s", path)
//         ^                                    ^
//         Input string                         Output: path = "./textures/north.xpm"
//         Returns: 1 (successfully read 1 item)

ft_sscanf("SO ./textures/south.xpm", "NO %s", path)
//         Doesn't match "NO"
//         Returns: 0 (read 0 items)
```

**Flow diagram:**

```mermaid
sequenceDiagram
    participant Line as Line: "NO ./tex/n.xpm"
    participant Parser as parse_texture_no
    participant SScanf as ft_sscanf
    participant Setter as set_texture
    
    Line->>Parser: Parse this
    Parser->>SScanf: Try format "NO %s"
    SScanf->>SScanf: Match "NO"? YES
    SScanf->>SScanf: Read string "./tex/n.xpm"
    SScanf-->>Parser: Return 1 (success), path="./tex/n.xpm"
    Parser->>Setter: set_texture(..., path, FLAG_NO)
    Setter-->>Parser: Return result
    Parser-->>Line: Return result
```

---

### Texture Validation: `set_texture()`

**File:** `src/parsing/utils_parsing.c`

```c
int set_texture(t_config *config, char **texture, char *path, int flag)
{
    int fd;

    if (config->config_flags & flag)
        return (-1);
    if (!is_xpm_file(path))
        return (-1);
    fd = open(path, O_RDONLY);
    if (fd < 0)
        return (-1);
    close(fd);
    *texture = gc_strdup(path);
    if (!*texture)
        return (-1);
    config->config_flags |= flag;
    return (1);
}
```

**Step-by-step what each check does:**

```mermaid
graph TD
    A[set_texture called] --> B{Flag already set?}
    B --> |Yes| ERR1[Return -1: Duplicate]
    B --> |No| C{Path ends with .xpm?}
    C --> |No| ERR2[Return -1: Wrong extension]
    C --> |Yes| D[Try to open file]
    D --> E{File opened?}
    E --> |No| ERR3[Return -1: File doesn't exist]
    E --> |Yes| F[Close file]
    F --> G[Duplicate string with gc_strdup]
    G --> H{Memory allocated?}
    H --> |No| ERR4[Return -1: Malloc failed]
    H --> |Yes| I[Store path in texture pointer]
    I --> J[Set flag bit]
    J --> K[Return 1: Success!]
    
    style K fill:#ccffcc
    style ERR1 fill:#ffcccc
    style ERR2 fill:#ffcccc
    style ERR3 fill:#ffcccc
    style ERR4 fill:#ffcccc
```

#### Check 1: Duplicate Detection

```c
if (config->config_flags & flag)
    return (-1);
```

**How bitwise AND works for duplicate detection:**

```
Example: Checking if NO is already set

config_flags = 0b00000001  (NO already set)
FLAG_NO      = 0b00000001

config_flags & FLAG_NO = 0b00000001 (nonzero, means already set!)

---

Another example: NO not set yet

config_flags = 0b00000000  (nothing set)
FLAG_NO      = 0b00000001

config_flags & FLAG_NO = 0b00000000 (zero, means not set)
```

#### Check 2: File Extension

```c
if (!is_xpm_file(path))
    return (-1);
```

**Function:** `is_xpm_file()` from `utils_parsing.c`

```c
int is_xpm_file(const char *path)
{
    int len;

    len = ft_strlen(path);
    if (len < 4)
        return (0);
    if (ft_strncmp(path + len - 4, ".xpm", 4) != 0)
        return (0);
    return (1);
}
```

**Visual:**

```
Path: "./textures/north.xpm"
                        ^^^^
                        Compare last 4 chars with ".xpm"
                        
ft_strncmp(".xpm", ".xpm", 4) == 0  ✓ Match!
```

#### Check 3: File Existence

```c
fd = open(path, O_RDONLY);
if (fd < 0)
    return (-1);
close(fd);
```

**Why open and immediately close?**

We're just **testing** if the file exists and is readable. We don't actually need to read it now (that happens later when loading textures for the game).

```mermaid
sequenceDiagram
    participant Code
    participant OS
    
    Code->>OS: open("./textures/north.xpm")
    OS-->>Code: fd = 3 (success)
    Code->>OS: close(3)
    Note over Code: File exists and is readable ✓
```

#### Check 4: Store Path

```c
*texture = gc_strdup(path);
if (!*texture)
    return (-1);
```

**What is `gc_strdup()`?**

It duplicates a string (allocates memory and copies):

```c
// Input: path = "./textures/north.xpm"
// Output: new string in heap memory with garbage collector tracking

char *new_str = gc_strdup(path);
// new_str points to a new copy of "./textures/north.xpm"
```

**Why duplicate instead of just storing the pointer?**

The `path` array is **local** to the parsing function and will be overwritten on the next line. We need a **permanent copy**:

```c
void example()
{
    char path[256];  // Local array, on stack
    
    ft_sscanf(line1, "NO %s", path);  // path = "./tex/north.xpm"
    config->textures.north = path;  // ❌ WRONG! Path is local
    
    ft_sscanf(line2, "SO %s", path);  // path now = "./tex/south.xpm"
    // config->textures.north also points to path...
    // now both point to "./tex/south.xpm"! BUG!
    
    // ✅ CORRECT way:
    config->textures.north = gc_strdup(path);  // Permanent copy
}
```

#### Check 5: Set Flag

```c
config->config_flags |= flag;
```

**Bitwise OR to set a flag:**

```
Before: config_flags = 0b00000000 (nothing set)
        FLAG_NO      = 0b00000001

After:  config_flags |= FLAG_NO
        config_flags = 0b00000001 (NO is set)

---

Next texture:
Before: config_flags = 0b00000001 (NO set)
        FLAG_SO      = 0b00000010

After:  config_flags |= FLAG_SO
        config_flags = 0b00000011 (NO and SO are set)
```

---

## Color Parsing Deep Dive

### Color Parser: `parse_color_floor()`

**File:** `src/parsing/utils_parsing2.c`

```c
int parse_color_floor(t_config *config, const char *line)
{
    int  r, g, b;
    char c;

    c = '\0';
    if (ft_sscanf(line, "F %d,%d,%d %c", &r, &g, &b, &c) != 3)
        return (0);
    if (!validate_color(r, g, b))
        return (-1);
    config->floor = (t_color){r, g, b};
    return set_color(config, &config->floor, FLAG_F);
}
```

**Understanding the format string:** `"F %d,%d,%d %c"`

```mermaid
graph LR
    A["F 220,100,0"] --> B[F]
    A --> C[space]
    A --> D[220]
    A --> E[,]
    A --> F[100]
    A --> G[,]
    A --> H[0]
    
    I["Format: 'F %d,%d,%d %c'"] --> J['F' literal]
    I --> K['%d' = integer]
    I --> L[','literal]
    I --> M['%d' = integer]
    I --> N[',' literal]
    I --> O['%d' = integer]
    I --> P[' %c' = optional char]
    
    B -.matches.-> J
    C -.space between.-> K
    D -.reads into.-> K
    E -.matches.-> L
    F -.reads into.-> M
    G -.matches.-> N
    H -.reads into.-> O
    
    style K fill:#ccffcc
    style M fill:#ccffcc
    style O fill:#ccffcc
    style P fill:#ffcccc
```

**Why `%c` at the end?**

This is a validation trick! We expect **exactly 3 values** (R, G, B). The `%c` tries to read an extra character:

```c
// Valid: "F 220,100,0\n"
// ft_sscanf returns 3 (read R, G, B, but no extra character)

// Invalid: "F 220,100,0 extra\n"
// ft_sscanf returns 4 (read R, G, B, and 'e' into c)

if (ft_sscanf(..., &r, &g, &b, &c) != 3)
    return (0);  // Must be exactly 3!
```

---

### Color Validation: `validate_color()`

**File:** `src/parsing/utils_parsing.c`

```c
int validate_color(int r, int g, int b)
{
    if (r < 0 || r > 255)
        return (0);
    if (g < 0 || g > 255)
        return (0);
    if (b < 0 || b > 255)
        return (0);
    return (1);
}
```

**Why 0-255?**

RGB color components are typically stored as **8-bit unsigned integers**:

```
8 bits can represent: 2^8 = 256 values
Range: 0 to 255

Binary:
00000000 = 0   (minimum, black)
11111111 = 255 (maximum, brightest)
```

**Visual representation:**

```
R = 220 = 11011100 (binary) → High red
G = 100 = 01100100 (binary) → Medium green  
B = 0   = 00000000 (binary) → No blue

Result: Orange color! 🟧
```

---

### Setting Color Flag: `set_color()`

**File:** `src/parsing/utils_parsing2.c`

```c
int set_color(t_config *config, t_color *color, int flag)
{
    (void)color;  // Unused parameter
    if (config->config_flags & flag)
        return (-1);
    config->config_flags |= flag;
    return (1);
}
```

**Why is `color` unused?**

The color values are **already stored** before this function is called:

```c
// In parse_color_floor():
config->floor = (t_color){r, g, b};  // ← Store color here
return set_color(config, &config->floor, FLAG_F);  // ← Just set flag
```

So `set_color()` only needs to:
1. Check if flag is already set (duplicate detection)
2. Set the flag

---

## The Flag System

### Understanding Bit Flags

Flags are a clever way to track multiple boolean states using a single integer:

```c
#define FLAG_NO  0x01  // 0b00000001
#define FLAG_SO  0x02  // 0b00000010
#define FLAG_WE  0x04  // 0b00000100
#define FLAG_EA  0x08  // 0b00001000
#define FLAG_F   0x10  // 0b00010000
#define FLAG_C   0x20  // 0b00100000
#define FLAG_ALL 0x3F  // 0b00111111
```

### Visual Representation

```
config_flags as parsing progresses:

Initial state:
┌─┬─┬─┬─┬─┬─┬─┬─┐
│0│0│0│0│0│0│0│0│ = 0x00 (nothing parsed)
└─┴─┴─┴─┴─┴─┴─┴─┘
 │ │ │ │ │ │ │ └─ Bit 0: NO
 │ │ │ │ │ │ └─── Bit 1: SO
 │ │ │ │ │ └───── Bit 2: WE
 │ │ │ │ └─────── Bit 3: EA
 │ │ │ └───────── Bit 4: F
 │ │ └─────────── Bit 5: C
 └─└───────────── (unused)

After parsing "NO":
┌─┬─┬─┬─┬─┬─┬─┬─┐
│0│0│0│0│0│0│0│1│ = 0x01
└─┴─┴─┴─┴─┴─┴─┴─┘

After parsing "SO":
┌─┬─┬─┬─┬─┬─┬─┬─┐
│0│0│0│0│0│0│1│1│ = 0x03
└─┴─┴─┴─┴─┴─┴─┴─┘

After parsing all 6:
┌─┬─┬─┬─┬─┬─┬─┬─┐
│0│0│1│1│1│1│1│1│ = 0x3F = FLAG_ALL ✓
└─┴─┴─┴─┴─┴─┴─┴─┘
```

### Operations on Flags

```mermaid
graph TD
    A[Flag Operations] --> B[Set a Flag: OR]
    A --> C[Check a Flag: AND]
    A --> D[Check All Flags: AND with FLAG_ALL]
    
    B --> B1["flags |= FLAG_NO<br/>Sets bit 0 to 1"]
    C --> C1["flags & FLAG_NO<br/>Returns non-zero if set"]
    D --> D1["flags & FLAG_ALL == FLAG_ALL<br/>True if all 6 bits set"]
    
    style B1 fill:#e1f5ff
    style C1 fill:#ffe1e1
    style D1 fill:#ffe1ff
```

#### Setting a Flag (OR Operation)

```c
config->config_flags |= FLAG_NO;
```

**Bitwise explanation:**

```
config_flags = 0b00000000
FLAG_NO      = 0b00000001
              ─────────────  OR (|)
Result       = 0b00000001

The | operator sets bit 0 to 1, keeps others unchanged
```

#### Checking a Flag (AND Operation)

```c
if (config->config_flags & FLAG_NO)
    // Flag is set!
```

**Bitwise explanation:**

```
config_flags = 0b00000001  (NO is set)
FLAG_NO      = 0b00000001
              ─────────────  AND (&)
Result       = 0b00000001  (non-zero, TRUE)

---

config_flags = 0b00000000  (NO not set)
FLAG_NO      = 0b00000001
              ─────────────  AND (&)
Result       = 0b00000000  (zero, FALSE)
```

#### Checking All Flags

```c
if ((config->config_flags & FLAG_ALL) != FLAG_ALL)
    ft_exit(5, "Missing identifier element/s");
```

**Bitwise explanation:**

```
All set:
config_flags = 0b00111111
FLAG_ALL     = 0b00111111
              ─────────────  AND (&)
Result       = 0b00111111  (equals FLAG_ALL, all present!)

Missing C:
config_flags = 0b00011111  (C bit not set)
FLAG_ALL     = 0b00111111
              ─────────────  AND (&)
Result       = 0b00011111  (NOT equal to FLAG_ALL, missing elements!)
```

---

## Complete Example Walkthrough

Let's trace the parsing of this simple config:

```
NO ./textures/north.xpm
SO ./textures/south.xpm
WE ./textures/west.xpm
EA ./textures/east.xpm
F 220,100,0
C 225,30,0
```

### Step-by-Step Execution

```mermaid
sequenceDiagram
    participant M as main()
    participant P as parse_config_file()
    participant R as read_config_lines()
    participant G as get_next_line()
    participant PCL as parse_config_line()
    participant PNO as parse_texture_no()
    participant ST as set_texture()
    
    M->>P: parse_config_file(&config, "map.cub")
    P->>P: Check extension ✓
    P->>P: Open file ✓
    P->>P: ft_memset(&config, 0)
    P->>R: read_config_lines(&config, fd)
    
    Note over R: Line 1: "NO ./textures/north.xpm"
    R->>G: get_next_line(fd)
    G-->>R: "NO ./textures/north.xpm\n"
    R->>R: is_map_line? No
    R->>PCL: parse_config_line(config, line)
    PCL->>PNO: parse_texture_no(config, line)
    PNO->>PNO: ft_sscanf("NO %s") = 1
    PNO->>ST: set_texture(..., FLAG_NO)
    ST->>ST: flags & FLAG_NO? No (not set)
    ST->>ST: is_xpm_file? Yes
    ST->>ST: open file? Yes
    ST->>ST: gc_strdup path
    ST->>ST: flags |= FLAG_NO
    ST-->>PNO: Return 1
    PNO-->>PCL: Return 1
    PCL-->>R: Return 1
    R->>R: free(line)
    
    Note over R: config_flags = 0b00000001
    
    Note over R: Line 2: "SO ./textures/south.xpm"
    R->>G: get_next_line(fd)
    G-->>R: "SO ./textures/south.xpm\n"
    R->>R: is_map_line? No
    R->>PCL: parse_config_line(config, line)
    Note over PCL: ... similar process ...
    PCL-->>R: Return 1
    R->>R: free(line)
    
    Note over R: config_flags = 0b00000011
    
    Note over R: Lines 3-6 parsed similarly...
    
    Note over R: config_flags = 0b00111111 = FLAG_ALL ✓
    
    R-->>P: Return 0 (success)
    P->>P: Check flags & FLAG_ALL == FLAG_ALL ✓
    P->>P: close(fd)
    P->>P: Proceed to map parsing...
```

### Memory State After Config Parsing

```
t_config config = {
    .textures = {
        .north = "0x1234" → "./textures/north.xpm"
        .south = "0x2345" → "./textures/south.xpm"
        .west  = "0x3456" → "./textures/west.xpm"
        .east  = "0x4567" → "./textures/east.xpm"
    },
    .floor = {
        .r = 220,
        .g = 100,
        .b = 0
    },
    .ceiling = {
        .r = 225,
        .g = 30,
        .b = 0
    },
    .map = {
        /* Not filled yet, done in map parsing phase */
    },
    .config_flags = 0b00111111  // All 6 flags set
}
```

---

## Summary Diagram

```mermaid
graph TB
    A[parse_config_file] --> B[Initialize: memset config to 0]
    B --> C[config_flags = 0b00000000]
    
    C --> D[read_config_lines loop]
    D --> E{Line type?}
    
    E --> |Texture NO| F[parse_texture_no]
    E --> |Texture SO| G[parse_texture_so]
    E --> |Texture WE| H[parse_texture_we]
    E --> |Texture EA| I[parse_texture_ea]
    E --> |Color F| J[parse_color_floor]
    E --> |Color C| K[parse_color_ceiling]
    E --> |Empty| D
    E --> |Map line| L[Stop config parsing]
    E --> |Invalid| M[Error: exit]
    
    F --> N[set_texture]
    G --> N
    H --> N
    I --> N
    
    N --> O{Valid?}
    O --> |Yes| P[Store path, set flag]
    O --> |No| M
    
    J --> Q[validate_color]
    K --> Q
    Q --> R{Valid?}
    R --> |Yes| S[Store color, set flag]
    R --> |No| M
    
    P --> T[config_flags updated]
    S --> T
    T --> U{All flags set?}
    U --> |No| D
    U --> |Yes| L
    
    L --> V{flags & FLAG_ALL == FLAG_ALL?}
    V --> |Yes| W[Config parsing complete!]
    V --> |No| X[Error: Missing elements]
    
    style W fill:#ccffcc
    style M fill:#ffcccc
    style X fill:#ffcccc
```

---

**Continue to:** [Map Parsing →](04_map_parsing.md)
