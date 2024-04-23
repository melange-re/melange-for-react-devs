module Menu = {
  [@mel.module "./Menu.jsx"] [@react.component]
  external make: unit => React.element = "default";
};

module App = {
  let items: Order.t = [
    // Sandwich(Portabello),
    Sandwich(Unicorn),
    Sandwich(Ham),
    Sandwich(Turducken),
    Hotdog,
    Burger({lettuce: true, tomatoes: true, onions: 3, cheese: 2, bacon: 6}),
    Burger({lettuce: false, tomatoes: false, onions: 0, cheese: 0, bacon: 0}),
    // Burger({lettuce: true, tomatoes: false, onions: 1, cheese: 1, bacon: 1}),
    // Burger({lettuce: false, tomatoes: false, onions: 1, cheese: 0, bacon: 0}),
    // Burger({lettuce: false, tomatoes: false, onions: 0, cheese: 1, bacon: 0}),
  ];

  [@react.component]
  let make = () => <div> <Menu /> </div>;
};

let node = ReactDOM.querySelector("#root");
switch (node) {
| None =>
  Js.Console.error("Failed to start React: couldn't find the #root element")
| Some(root) =>
  let root = ReactDOM.Client.createRoot(root);
  ReactDOM.Client.render(root, <App />);
};
