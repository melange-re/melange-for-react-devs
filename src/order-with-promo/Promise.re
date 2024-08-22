type t('a) = Js.Promise.t('a);

let iter = (f, p) =>
  p
  |> Js.Promise.then_(v => {
       f(v);
       Js.Promise.resolve();
     })
  |> ignore;

let map = (f, p) => p |> Js.Promise.then_(v => v |> f |> Js.Promise.resolve);
