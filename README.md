# Loremipsum
**`loremipsum` is a lightweight, customizable command-line interface (CLI) tool that generates random placeholder text in the classic "Lorem Ipsum" style directly to `stdout`.**

It uses a pool of 48 predefined sentences containing only uppercase/lowercase alphabetic characters, periods, commas, and exclamation marks. It is ideal for developers, testers, and system administrators who need a quick, dependency-free text generator.  
Each sentence may begin with a sequential number (counter).  

## Features

- **Infinite Streaming:** Generates a continuous stream of text by default.
- **Custom Formatting:** Optional newline separation after each sentence.
- **Reproducible Outputs:** Supports random seeds for deterministic text generation.
- **High Performance:** Written in pure C with zero external dependencies, perfect for piping and scripting.

## Installation & Compilation

To compile the program from source, ensure you have a C compiler like `gcc` installed, then run:

```bash
gcc -O3 loremipsum.c -o loremipsum
```

## Usage

```bash
./loremipsum [[start] [options]]
```

### Options

| Option | Description | Default |
| :--- | :--- | :--- |
| `-newline` | Write a newline after every sentence | No newline (continuous block) |
| `-seed <number>` | Initialize the random generator to a specific seed | Random seed |
| `-sentences <number>` | Write a specific number of sentences | Infinite |
| `-counter` | Write a sequential number at the beginning of the sentences | No counter|
| *No arguments* | Prints the help message | - |

## Common Use Cases & Examples

### 1. UI/UX & Web Development
Populate frontend layouts, database fields, or mock APIs with a specific amount of readable text to test typography and responsive design:
```bash
# Generate exactly 20 sentences structured in lines
./loremipsum start -sentences 20 -newline
```

### 2. Mock File Generation
Create dummy files of any size for software deployment testing or file transfer validation using standard output redirection:
```bash
# Create a text file with 100 placeholder sentences
./loremipsum start -newline -sentences 100 > dummy_placeholder.txt
```

### 3. CLI Pipe & Stream Testing
Use the default infinite stream behavior to test command-line pipes, debug data streams, or benchmark terminal performance:
```bash
# Pipe a stream of text into another command (e.g., word count)
./loremipsum start -newline -sentences 1000 -counter | wc
```

### 4. Automated Testing & CI/CD
Leverage the `-seed` option to generate the exact same text pattern every time, making it perfect for deterministic integration tests:
```bash
# Generates a reproducible text sequence for automated tests
./loremipsum start -newline -sentences 5 -seed 12345
```

## License

Copyright (C) Matteo Zapparoli.  
Licensed under the **Apache License, Version 2.0** (the "License").  
You may obtain a copy of the License at [http://apache.org](http://apache.org).

