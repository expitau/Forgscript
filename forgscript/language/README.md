# Forglang Language Support

Adds language support for Forgscript

## Features

- Syntax highlighting
- Underline next step

## Extension Settings

Include if your extension adds any VS Code settings through the `contributes.configuration` extension point.

For example:

This extension contributes the following settings:

* `forgscript.underlineNextStep`: Specifies whether to underline the next step(s) of the program (where the forg will jump next)
* `forgscript.highlightNextAction`: Specifies whether to highlight the next action the program will take (the next command that is not a '.' or ignored)

## Release Notes

### 0.1.1

Support for highlighting next action

### 0.1.0

Initial release
