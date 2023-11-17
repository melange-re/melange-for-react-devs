module App = {
  [@react.component]
  let make = () => <h1> {React.string("Welcome to my app!")} </h1>;
};

let node = ReactDOM.querySelector("#root");
switch (node) {
| None => Js.Console.error("Failed to start React: couldn't find the #root element")
| Some(root) => ReactDOM.render(<App />, root)
};
