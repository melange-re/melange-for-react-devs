module Voice = SpeechSynthesis.Voice;

let getVoices = () => {
  SpeechSynthesis.getVoices()
  |> Promise.map(voices =>
       voices
       |> Array.toList
       |> List.filter(v =>
            v |> Voice.getLang |> Js.String.startsWith(~prefix="en-")
          )
     );
};

module Inner = {
  [@react.component]
  let make = (~voice as initVoice: Voice.t, ~voices: list(Voice.t)) => {
    let (counter, setCounter) = React.useState(() => 30);
    let (voice, setVoice) = RR.useStateValue(initVoice);
    let (intervalId, setIntervalId) = RR.useStateValue(None);

    React.useEffect1(
      () => {
        if (counter == 0) {
          intervalId
          |> Option.iter(intervalId => {
               Js.Global.clearInterval(intervalId);
               setIntervalId(None);
             });
        };
        None;
      },
      [|counter|],
    );

    <div>
      <NumberInput
        disabled={intervalId |> Option.is_some}
        value=counter
        onChange={newCounter => setCounter(_ => newCounter)}
      />
      <select
        value={Voice.getName(voice)}
        onChange={evt => {
          let name = RR.getValueFromEvent(evt);
          voices
          |> List.find_opt(v => Voice.getName(v) == name)
          |> Option.iter(setVoice);
        }}>
        {voices
         |> List.map(v => {
              let name = v |> Voice.getName;
              <option key=name value=name> {name |> RR.s} </option>;
            })
         |> RR.list}
      </select>
      <button
        onClick={_evt =>
          switch (intervalId) {
          | Some(intervalId) =>
            Js.Global.clearInterval(intervalId);
            setIntervalId(None);
          | None =>
            Js.Global.setInterval(1000, ~f=() => setCounter(v => v - 1))
            |> Option.some
            |> setIntervalId
          }
        }>
        {(intervalId == None ? "Start" : "Stop") |> RR.s}
      </button>
    </div>;
  };
};

[@react.component]
let make = () => {
  let (voices, setVoices) = RR.useStateValue([]);

  React.useEffect0(() => {
    getVoices() |> Promise.iter(setVoices);
    None;
  });

  switch (voices) {
  | [] => React.null
  | [voice, ..._] => <Inner voice voices />
  };
};
