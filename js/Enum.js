/*
Enum:
var x = new Enum({ A: 1, B: 2, C: 'Z' });
console.log(x.A.toString());    // = A
console.log(x.A());             // = 1
*/

var Enum = function(obj){

    this.toEnum = function (x){
      for(var i in x){
          let value = x[i];
          let name = i.toString();
          var prop = function(){
              return value; 
          };
          prop.toString = function(){
              return name;
          };
          x[i] = prop;
      }
      return x;
    };
   
    return this.toEnum(obj);
};
