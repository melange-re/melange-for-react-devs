// Safe array access function
let get: (array('a), int) => option('a) =
  (array, index) =>
    switch (index) {
    | index when index < 0 || index >= Js.Array.length(array) => None
    | index => Some(Stdlib.Array.get(array, index))
    };

let toList = Stdlib.Array.to_list;
