/** Return the nth element encased in Some; if it doesn't exist, return None */
let nth = (n, list) => n < 0 ? None : List.nth_opt(list, n);

/** Return the head encased in Some; if empty, return None */
let head =
  fun
  | [] => None
  | [hd, ..._tail] => Some(hd);
