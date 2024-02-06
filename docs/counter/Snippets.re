// #region app-v1
module App = {
  [@react.component]
  let make = () => <div> {React.string("welcome to my app")} </div>;
};
// #endregion app-v1

// #region use-app-component
let node = ReactDOM.querySelector("#root");
switch (node) {
| Some(root) =>
  let root = ReactDOM.Client.createRoot(root);
  ReactDOM.Client.render(root, <App />);
| None =>
  Js.Console.error("Failed to start React: couldn't find the #root element")
};
// #endregion use-app-component

module Counter = {
  // #region counter-v1
  [@react.component]
  let make = () => {
    let (counter, setCounter) = React.useState(() => 0);

    <div>
      <button onClick={_evt => setCounter(v => v - 1)}>
        {React.string("-")}
      </button>
      {React.string(Int.to_string(counter))}
      <button onClick={_evt => setCounter(v => v + 1)}>
        {React.string("+")}
      </button>
    </div>;
  };
  // #endregion counter-v1
};

module AppV2 = {
  // #region app-v2
  module App = {
    [@react.component]
    let make = () => <Counter />;
  };
  // #endregion app-v2
};

let _ = {
  let (counter, setCounter) = React.useState(() => 0);

  // #region render-with-styling
  <div
    style={ReactDOM.Style.make(
      ~padding="1em",
      ~display="flex",
      ~gridGap="1em",
      (),
    )}>
    <button onClick={_evt => setCounter(v => v - 1)}>
      {React.string("-")}
    </button>
    <span> {counter |> Int.to_string |> React.string} </span>
    <button onClick={_evt => setCounter(v => v + 1)}>
      {React.string("+")}
    </button>
  </div>;
  // #endregion render-with-styling
};
