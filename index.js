'use strict';

var nativeClipboard

if (process.env.DEBUG) {
    nativeClipboard = require('./build/Debug/clipboard.node')
} else {
    nativeClipboard = require('./build/Release/clipboard.node')
}

module.exports.writePaths = function (paths) {
    // nativeClipboard.writeFiles(['Test1', 'Test2'])
    nativeClipboard.writeFiles(paths)
}

module.exports.readPaths = function () {
    return [
        'C:\\Test.txt',
        'D:\\TestDir'
    ]
}