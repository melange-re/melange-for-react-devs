/** Use state hook that doesn't use callback functions */
let useStateValue = initial =>
  React.useReducer((_ignored, newState) => newState, initial);

/** Get string value from the given event's target */
let getValueFromEvent = (evt): string => React.Event.Form.target(evt)##value;

/** Alias for [React.string] */
let s = React.string;

/** Render a list of [React.element]s */
let list = list => list |> Stdlib.Array.of_list |> React.array;
