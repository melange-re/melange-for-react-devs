module App = {
  [@react.component]
  let make = () =>
    <div>
      <h1> {React.string("Celsius Converter")} </h1>
      <CelsiusConverter />
    </div>;
};

let node = ReactDOM.querySelector("#root");
switch (node) {
| None =>
  Js.Console.error("Failed to start React: couldn't find the #root element")
| Some(root) =>
  let root = ReactDOM.Client.createRoot(root);
  ReactDOM.Client.render(root, <App />);
};
