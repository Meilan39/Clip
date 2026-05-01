# Clip

Clip is a fast and efficient clipboard manager designed for the command line. It allows you to save, retrieve, and manage text snippets seamlessly using local binary storage.

## Features

- Save clipboard contents to an alias.
- Retrieve and copy snippets directly to your system clipboard.
- Print snippets to the terminal.
- Built-in suggestion engine for typos and fuzzy matches.
- Automated garbage collection to optimize storage space.
- Global path resolution and automated installation.

## Installation

Clip can be installed directly from the source using the provided Makefile. By default, it installs to `/usr/local/bin`.

```bash
make
sudo make install
```

### Uninstallation

To remove Clip from your system:

```bash
sudo make uninstall
```

## Usage

Clip provides several subcommands to interact with your saved snippets. 

### Commands

*   `clip new <alias>` (shortcut: `n`)
    Saves the current text in your system clipboard to the specified `<alias>`. If the alias already exists, it will be overwritten.

*   `clip get <alias>` (shortcut: `g`)
    Retrieves the snippet associated with `<alias>` and copies it back to your system clipboard.

*   `clip cat <alias>` (shortcut: `c`)
    Prints the complete content of the specified `<alias>` to the standard output.

*   `clip peek <alias>` (shortcut: `p`)
    Prints a truncated preview (up to 100 characters) of the specified `<alias>`.

*   `clip delete <alias>` (shortcut: `d`)
    Deletes the snippet associated with `<alias>`. The space is reclaimed for future snippets.

*   `clip rename <old_alias> <new_alias>` (shortcut: `r`)
    Renames an existing alias to a new name.

*   `clip list` (shortcut: `ls`)
    Lists all saved aliases.

*   `clip clean`
    Runs garbage collection to compact the binary storage file and reclaim fragmented disk space left by deleted snippets.

## Storage

Clip automatically creates a hidden directory in your home folder at `~/.clip/` upon first run. 
- `meta.txt`: Contains the metadata index and aliases.
- `data.bin`: Contains the raw text data of the snippets.

## Requirements

- A macOS operating system (relies on `pbcopy` and `pbpaste`).
- GCC or an equivalent C compiler.
- GNU Make.
