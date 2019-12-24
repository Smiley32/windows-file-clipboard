'use strict';

var expect = require('chai').expect;
var clipboard = require('../index');

describe('#numFormatter', function() {
    it('One entry', function() {
        clipboard.writePaths([
            'It\'s not a path, but it doesn\'t matter'
        ]);
        var result = clipboard.readPaths();
        expect(result.length).to.equal(1);
    });

    it('Two entries', function() {
        clipboard.writePaths([
            'C:\\Test1',
            'C:\\Test2'
        ]);
        var result = clipboard.readPaths();
        expect(result.length).to.equal(2);
    });

    it('One same entry before and after', function() {
        var path1 = '/Test 1/complex file.tar.gz '
        clipboard.writePaths([
            path1
        ]);
        var result = clipboard.readPaths();
        expect(result.length).to.equal(1);
        expect(result[0]).to.equal(path1);
    });

    it('Two sames entries before and after', function() {
        var path1 = 'C:\\Test1\\complex file.tar.gz'
        var path2 = 'C:\\Test2\\Just\\A\\Dir\\'
        clipboard.writePaths([
            path1,
            path2
        ]);
        var result = clipboard.readPaths();
        expect(result.length).to.equal(2);
        expect(result[0]).to.equal(path1);
        expect(result[1]).to.equal(path2);
    });
});