let getValueFromEvent = _evt => "";

let s = React.string;

let useStateValue = initial =>
  React.useReducer((_state, newState) => newState, initial);

let list = list => list |> Stdlib.Array.of_list |> React.array;

let currency = value => value |> Js.Float.toFixed(~digits=2) |> React.string;
