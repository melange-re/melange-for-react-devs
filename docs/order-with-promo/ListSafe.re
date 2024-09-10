// #region humanize
/** Take a list of strings and return a human-readable string */
let humanize =
  fun
  | [] => ""
  | [x] => x
  | [x, y] => x ++ " and " ++ y
  | [first, ...rest] =>
    rest
    |> List.rev
    |> List.mapi((i, s) => i == 0 ? "and " ++ s : s)
    |> List.rev
    |> List.fold_left((acc, s) => acc ++ ", " ++ s, first);
// #endregion humanize

let _ =
  (rest, first) => {
    // #region alternate
    rest
    |> List.rev
    |> List.mapi((i, s) => ", " ++ (i == 0 ? "and " ++ s : s))
    |> List.rev
    |> List.fold_left((++), first);
                                   // #endregion alternate
  };
