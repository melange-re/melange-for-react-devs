let items: Order.t = [
  Sandwich(Portabello),
  Sandwich(Unicorn),
  Sandwich(Ham),
  Sandwich(Turducken),
  Hotdog,
  Burger({lettuce: true, tomatoes: true, onions: 3, cheese: 2, bacon: 6}),
  Burger({lettuce: false, tomatoes: false, onions: 0, cheese: 0, bacon: 0}),
  Burger({lettuce: true, tomatoes: false, onions: 1, cheese: 1, bacon: 1}),
  Burger({lettuce: false, tomatoes: false, onions: 1, cheese: 0, bacon: 0}),
  Burger({lettuce: false, tomatoes: false, onions: 0, cheese: 1, bacon: 0}),
];

[@react.component]
let make = () => {
  let (date, setDate) =
    RR.useStateValue(Js.Date.fromString("2024-05-28T00:00"));

  <div>
    <h1> {RR.s("Order confirmation")} </h1>
    <DateInput date onChange=setDate />
    <h2> {RR.s("Promo")} </h2>
    <Promo items date />
    <h2> {RR.s("Order")} </h2>
    <Order items date />
  </div>;
};
