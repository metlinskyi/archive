var Datepicker = function () {

    var self = this;

    self.Column = {
        Id: 0,
        Word: 1,
        Is: 2,
        Rel: 3,
        Val: 4
    };

    self.Is = {
        Default: 0,
        Related: 1,
        Present: 2,
        Period: 4,
        Size: 8,
        Month: 16,
        Past: 32,
        Date: 64,
        Time: 128
    };

    self.dictionary = [
        [1, "now", this.Is.Present, null, function () {
                return Date.now();
            }],
        [2, "today", this.Is.Present, null, function () {
                return new Date();
            }],
        [4, "yesterday", this.Is.Past | this.Is.Date, [1, 2], 1],
        [5, "last", this.Is.Past | this.Is.Related, [3, 8, 9, 10, 11, 12, 13, 14, 15, 28], null],
        [6, "ago", this.Is.Past | this.Is.Related, [8, 9, 10, 11, 12, 13, 14, 15, 28], null],
        [7, "half", this.Is.Related, [8, 9, 10, 11, 12, 13, 14, 15, 28], null],
        [8, "millennium", this.Is.Period, null],
        [9, "century", this.Is.Period, null],
        [10, "year", this.Is.Period | this.Is.Related, [6], 365],
        [3, "weekend", this.Is.Period, null],
        [11, "month", this.Is.Period | this.Is.Related, [6]],
        [12, "day", this.Is.Period, null],
        [13, "hour", this.Is.Period, null],
        [14, "minute", this.Is.Period, null],
        [15, "second", this.Is.Period, null],
        [16, "january", this.Is.Month, null, 1],
        [17, "february", this.Is.Month, null, 2],
        [18, "march", this.Is.Month, null, 3],
        [19, "april", this.Is.Month, null, 4],
        [20, "may", this.Is.Month, null, 5],
        [21, "june", this.Is.Month, null, 6],
        [22, "july", this.Is.Month, null, 7],
        [23, "august", this.Is.Month, null, 8],
        [24, "september", this.Is.Month, null, 9],
        [25, "october", this.Is.Month, null, 10],
        [26, "november", this.Is.Month, null, 11],
        [27, "december", this.Is.Month, null, 12],
        [28, "week", this.Is.Period]
    ];
};

Datepicker.prototype.is_number = function (val) {
    return new RegExp('\d+', 'i').test(val);
};

Datepicker.prototype.is_plural = function (val) {
    return new RegExp('s$', 'i').test(val);
};

Datepicker.prototype.dateTime = function (item) {

    var date = new Date();

    var is = item[this.Column.Is];

    if (is & this.Is.Present) {
        return date;
    } else if (is & this.Is.Past) {
        return date.setDate(date.getDate() - item[this.Column.Val]);
    }

    return date;
};

Datepicker.prototype.select = function (callback) {

    if (callback === undefined) {
        throw "Undefined callback for select";
    }

    for (var i = 0; i < this.dictionary.length; i++) {
        var item = this.dictionary[i];
        callback(item);
    }

};

Datepicker.prototype.like = function (word, callback) {

    if (callback === undefined) {
        throw "Undefined callback like";
    }

    var self = this;


    if (self.is_number(word)) {

    } else if (self.is_plural(word)) {
        word += '?';
    }

    var pattern = new RegExp('^' + word + '.*', 'i');

    this.select(function (item) {

        if (pattern.test(item[self.Column.Word])) {

            if (item[self.Column.Is] & self.Is.Related) {
                self.join(item, function (item2) {

                    var date = item[self.Column.Val] !== null
                            ? self.dateTime(item)
                            : self.dateTime(item2);
                    callback(
                            item[self.Column.Word] + ' ' + item2[self.Column.Word] + ' ' + date.toString()

                            );
                });
            } else {
                callback(item[self.Column.Word] + ' ' + self.dateTime(item).toString());
            }
        }

    });
};

Datepicker.prototype.join = function (item, callback) {
    var self = this;
    this.select(function (item_rel) {
        if (item[self.Column.Rel].indexOf(item_rel[self.Column.Id]) !== -1) {
            callback(item_rel);
        }
    });
};

Datepicker.prototype.Autocomplete = function (term) {

    var result = [];

    var words = term.split(" ");

    for (var i = 0; i < words.length; i++) {
        var word = words[i];
        if (word.length > 0) {
            this.like(word, function (item) {
                result.push(item);
            });
        }
    }
    return result;
};