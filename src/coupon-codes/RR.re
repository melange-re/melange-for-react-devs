/** use state hook that doesn't use callback functions */
let useStateValue = initial =>
  React.useReducer((_ignored, newState) => newState, initial);
