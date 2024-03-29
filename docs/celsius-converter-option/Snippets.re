let _ = {
  let celsius = "";
  let convert = x => x;
  // #region float-of-string-opt
  (
    switch (celsius |> float_of_string_opt) {
    | None => "error"
    | Some(fahrenheit) =>
      (fahrenheit |> convert |> Js.Float.toFixed(~digits=2)) ++ {js|°F|js}
    }
  )
  // #endregion float-of-string-opt
  |> ignore;
};

let _ = {
  let celsius = "";
  let convert = x => x;
  // #region option-map
  (
    switch (
      celsius
      |> float_of_string_opt
      |> Option.map(convert)
      |> Option.map(Js.Float.toFixed(~digits=2))
    ) {
    | None => "error"
    | Some(fahrenheit) => fahrenheit ++ {js|°F|js}
    }
  )
  // #endregion option-map
  |> ignore;
};

let _ = {
  let celsius = "";
  let convert = x => x;
  // #region inner-ternary
  (
    switch (celsius |> float_of_string_opt |> Option.map(convert)) {
    | None => "error"
    | Some(fahrenheit) =>
      fahrenheit > 212.0
        ? {js|Unreasonably hot🥵|js}
        : Js.Float.toFixed(fahrenheit, ~digits=2) ++ {js| °F|js}
    }
  )
  // #endregion inner-ternary
  |> ignore;
};

let _ = {
  let celsius = "";
  let convert = x => x;
  // #region when-guard
  (
    switch (celsius |> float_of_string_opt |> Option.map(convert)) {
    | None => "error"
    | Some(fahrenheit) when fahrenheit > 212.0 => {js|Unreasonably hot🥵|js}
    | Some(fahrenheit) =>
      Js.Float.toFixed(fahrenheit, ~digits=2) ++ {js| °F|js}
    }
  )
  // #endregion when-guard
  |> ignore;
};

let _ = {
  let celsius = "";
  let convert = x => x;
  // #region string-trim
  (
    String.trim(celsius) == ""
      ? {js|?°F|js}
      : (
        switch (celsius |> float_of_string_opt |> Option.map(convert)) {
        | None => "error"
        | Some(fahrenheit) when fahrenheit > 212.0 => {js|Unreasonably hot🥵|js}
        | Some(fahrenheit) =>
          Js.Float.toFixed(fahrenheit, ~digits=2) ++ {js| °F|js}
        }
      )
  )
  // #endregion string-trim
  |> ignore;
};

let _ = {
  let celsius = "";
  let convert = x => x;
  // #region when-guard-cold
  (
    switch (celsius |> float_of_string_opt |> Option.map(convert)) {
    | None => "error"
    | Some(fahrenheit) when fahrenheit < (-128.6) => {js|Unreasonably cold🥶|js}
    | Some(fahrenheit) when fahrenheit > 212.0 => {js|Unreasonably hot🥵|js}
    | Some(fahrenheit) =>
      Js.Float.toFixed(fahrenheit, ~digits=2) ++ {js|°F|js}
    }
  )
  // #endregion when-guard-cold
  |> ignore;
};

// #region float-from-string
let floatFromString = text => {
  let value = Js.Float.fromString(text);
  Js.Float.isNaN(value) ? None : Some(value);
};
// #endregion float-from-string

let _ = {
  let celsius = "";
  let convert = x => x;
  // #region switch-float-from-string
  (
    switch (celsius |> floatFromString |> Option.map(convert)) {
    | None => "error"
    | Some(fahrenheit) when fahrenheit < (-128.6) => {js|Unreasonably cold🥶|js}
    | Some(fahrenheit) when fahrenheit > 212.0 => {js|Unreasonably hot🥵|js}
    | Some(fahrenheit) =>
      Js.Float.toFixed(fahrenheit, ~digits=2) ++ {js|°F|js}
    }
  )
  // #endregion switch-float-from-string
  |> ignore;
};
