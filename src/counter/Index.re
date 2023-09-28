module App = {
  [@react.component]
  let make = () => <Counter />;
};

let node = ReactDOM.querySelector("#root");
switch (node) {
| None => Js.Console.error("Failed to start React: couldn't find the #root element")
| Some(root) => ReactDOM.render(<App />, root)
};
