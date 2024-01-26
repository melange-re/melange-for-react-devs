let get = (index, array) =>
  switch (Array.unsafe_get(array, index)) {
  | exception _ => None
  | elem => Some(elem)
  };
