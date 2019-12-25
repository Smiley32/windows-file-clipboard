# windows-file-clipboard

[![Build Status](https://travis-ci.com/Smiley32/windows-file-clipboard.svg?branch=master)](https://travis-ci.com/Smiley32/windows-file-clipboard) 


`windows-file-clipboard` is a NodeJS module, that allows to read the paths written in the clipboard, and to write paths in the clipboard.

This module is for Windows only.

## Installation

Install it via npm:
```
npm install windows-file-clipboard
```

Then, make sure `electron-rebuild` is installed, and Microsoft Visual Studio (`windows-build-tools` should also work)

You must run
```
.\node_modules\.bin\electron-rebuild.cmd
```
in order to compile my plugin on your machine.

## Getting Started

It may require windows build tools, or Microsoft Visual Studio to compile the native C++ code needed.

```
const windowsClipboard = require('windows-file-clipboard')

windowsClipbard.writePaths([
  'The\\First\\Path',
  'Path\\To\\a.file'
])

// Ctrl+V in the Windows explorer should paste these files/directory

var paths = windowsClipboard.readPaths()

// paths should contains ['The\\First\\Path', 'Path\\To\\a.file'],
// or the names of the files copied from the explorer

windowsClipboard.emptyPaths()
// To remove all paths from the clipboard
```

## Contribute

I'm not an expert in Windows, nor C++ nor NodeJS, so please feel free to add issues, to improve this module.

