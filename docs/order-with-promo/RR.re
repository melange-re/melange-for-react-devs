let getValueFromEvent = _evt => "";

let s = React.string;

let useStateValue = initial =>
  React.useReducer((_state, newState) => newState, initial);

// #region use-effect-1
/** Helper for [React.useEffect1] */
let useEffect1 = (func, dep) => React.useEffect1(func, [|dep|]);
// #endregion use-effect-1
