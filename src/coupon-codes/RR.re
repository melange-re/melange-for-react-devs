/** Use state hook that doesn't use callback functions */
let useStateValue = initial =>
  React.useReducer((_state, newState) => newState, initial);

/** Get string value from the given event's target */
let getValueFromEvent = (evt): string => React.Event.Form.target(evt)##value;

/** Alias for [React.string] */
let s = React.string;

/** Render a list of [React.element]s */
let list = list => list |> Stdlib.Array.of_list |> React.array;

/** Render a float as currency */
let currency = value => value |> Js.Float.toFixed(~digits=2) |> React.string;

/** Render option(React.element) */
let option = (fn, opt) =>
  opt |> Option.map(fn) |> Option.value(~default=React.null);
