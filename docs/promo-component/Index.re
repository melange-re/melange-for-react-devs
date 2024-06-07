let items = [];

module Order = {
  [@react.component]
  let make = (~items as _) => <div />;
};

// #region make
[@react.component]
let make = () =>
  <div>
    <h1> {RR.s("Promo")} </h1>
    <Promo items date={Js.Date.fromString("2024-05-10T00:00")} />
    <h1> {RR.s("Order confirmation")} </h1>
    <Order items />
  </div>;
// #endregion make
