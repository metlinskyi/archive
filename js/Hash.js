/*
Hash:
new Hash("Hello, World!"); // = -284335608
*/
var Hash = function(value, callback, type) {

    this.toString = function(){
        return this.value.toString();
    };
    
    this.string = function(value, action) {
        var 
            hash = 5381,
            i = value.length;
        while (i) {
            hash = (hash * 33) ^ action(value.charCodeAt(--i));
        }
        return hash; 
    };
    
    this.array = this.object = function(value, action){
        var 
            hash = value && value.length;
        if (hash) {
            for (var i = 0; i !== value.length; i++) {
                hash ^= new Hash(action(value[i]));
            }
        }
        return hash; 
    };
       
    this.ip = function(value) {
        var 
            octets = value.split('.'),
            offset = (octets.length - 1) * 8;
        return [0]
                .concat(octets.map(function (x) { return parseInt(x); }))
                .reduce(function (x, y) { 
                    var res = (x || 0) + y * Math.pow(2, offset); 
                    offset -= 8; 
                    return res;
                });
    };
            
    var 
        func = this[type || typeof(value)],
        action = callback || function(i) { return i; };
    
    this.value = func ? func(value, action) : value;
};
