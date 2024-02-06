module App = {
  let items: Order.t = [|Sandwich, Burger, Sandwich, Hotdog|];

  [@react.component]
  let make = () =>
    <div>
      <h1> {React.string("Order confirmation")} </h1>
      <Order items />
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
