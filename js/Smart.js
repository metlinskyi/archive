Array.prototype.forEach = function (callback, context) {
    for (var index in this) {
        var item = this[index];
        if (!callback(index, item, context)) {
            break;
        }
    }
};
Array.prototype.first = function () {
    return this[0];
};
Array.prototype.last = function () {
    return this[this.length - 1];
};

var Smart = Smart || {};