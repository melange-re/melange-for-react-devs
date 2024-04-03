// Safe array access function
let get = (array: array('a), index): option('a) =>
  switch (index) {
  | index when index < 0 || index >= Js.Array.length(array) => None
  | index => Some(Stdlib.Array.get(array, index))
  };
