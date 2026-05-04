# memmap 

A memory forensics tool written in C that reads and parses `/proc/[pid]/maps`
to inspect the virtual address space of running Linux processes.

Built as a learning project alongside K&R C.

## Status

Work in progress. Being built incrementally.

- [x] Step 1 — Read and print `/proc/self/maps` raw
- [x] Step 2 — Parse each line into structured fields
- [x] Step 3 — Categorise and label regions
- [x] Step 4 — Target arbitrary process by PID
- [x] Step 5 — Read memory contents via `/proc/[pid]/mem`
- [ ] Step 6 — Diff two snapshots over time

## Background

Linux exposes live kernel data through the `/proc` filesystem. Every running
process has a directory at `/proc/[pid]/` containing its memory map, status,
and raw memory contents. This tool reads and interprets that data.

Key concepts explored:
- Virtual memory layout (text, data, stack, heap, anonymous mappings)
- Linux process permissions and DAC
- W^X memory protection (NX/DEP)
- The ptrace syscall


## Build and Run

### Dependencies

- gcc
- make

### Build

```bash
make
```

To force a clean rebuild:

```bash
make clean && make
```

### Run

```bash
# Inspect your own process
./memmap

# Inspect another process by PID
./memmap <pid>

# Inspecting root-owned processes requires sudo
sudo ./memmap 1
```

