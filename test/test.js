'use strict';

var expect = require('chai').expect;
var clipboard = require('../index');

describe('#numFormatter', function() {
    it('should have 2 entries', function() {
        var result = clipboard.readPaths();
        expect(result.length).to.equal(2);
    });

    it('should still have 2 entries', function() {
        clipboard.writePaths([
            'C:\\Test1',
            'C:\\Test2'
        ]);
        var result = clipboard.readPaths();
        expect(result.length).to.equal(2);
    });
});