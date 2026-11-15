Browser History Simulator in C



A modular C-based command-line application designed to simulate a web browser's core history management features, including tab management, navigation history, and automated testing support.

Project Structure
browser.h: Header file containing data structures (struct page, tabs, history stacks) and function prototypes.
browser.c: Implementation file containing the logic for browser operations (managing tabs, switching, backward/forward navigation, and history logging).
main.c: Entry point that handles file input/output (browser.in and browser.out).
tests/: Directory containing automated test input files (.in) and their expected reference outputs (.ref).
Makefile: Custom automation script for building the project and running local tests.
