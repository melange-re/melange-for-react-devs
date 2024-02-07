// This module shadows Stdlib.Array

// Safe array access function
let get = (array, index) =>
  switch (index) {
  | index when index < 0 || index >= Js.Array.length(array) => None
  | index => Some(Js.Array.unsafe_get(array, index))
  };
